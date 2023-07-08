#include "noiseoutputdatamodel.h"
#include "qlabel.h"

#include "data/heightdata.h"

NoiseOutputDataModel::NoiseOutputDataModel()
    : noise_data_{nullptr}, view_{nullptr}
{
    noise_view_ = new QPixmap();
}

QJsonObject NoiseOutputDataModel::save() const
{
    QJsonObject model = QtNodes::NodeDelegateModel::save();

    return model;
}

void NoiseOutputDataModel::load(QJsonObject const& model)
{
    // TODO
}

unsigned int NoiseOutputDataModel::nPorts(QtNodes::PortType port_type) const
{
    unsigned int result = 0;

    if(port_type == QtNodes::PortType::In)
    {
        result = 1;
    }

    return result;
}

QtNodes::NodeDataType NoiseOutputDataModel::dataType(
        QtNodes::PortType port_type,
        QtNodes::PortIndex port_index) const
{
    QtNodes::NodeDataType type;

    /*
     * This is a bit hacky. NoiseData will
     * return HeightData::type() because it inherits
     * and does not override. can't call HeightData
     * directly because it is an abstract class.
     */
    if(port_type == QtNodes::PortType::In ||
        port_type == QtNodes::PortType::Out)
    {
        type = NoiseData().type();
    }

    return type;
}

void NoiseOutputDataModel::setInData(
        std::shared_ptr<QtNodes::NodeData> data,
        QtNodes::PortIndex port_index)
{
    if(data != nullptr)
    {
        auto height_data = std::dynamic_pointer_cast<HeightData>(data);
        if(height_data != nullptr)
        {
            HeightDataType data_type = height_data->GetHeightDataType();

            if(data_type == HeightDataType::kNoiseSource)
            {
                auto noise = std::dynamic_pointer_cast<NoiseData>(height_data);
                noise_data_ = noise;

                if(noise_view_ != nullptr)
                {
                    delete noise_view_;
                }
                noise_view_ = noise_data_->AsBitmap(0, 0, 256, 256);
                view_->setPixmap(*noise_view_);

                emit dataUpdated(port_index);
            }
            else if(data_type == HeightDataType::kImageSource)
            {
                qDebug() << "AAAAA";
            }
        }
        else
        {
            emit dataInvalidated(port_index);
        }
    }
    else
    {
        emit dataInvalidated(port_index);
    }
}

std::shared_ptr<QtNodes::NodeData> NoiseOutputDataModel::outData(
        QtNodes::PortIndex)
{
    return noise_data_;
}

QWidget* NoiseOutputDataModel::embeddedWidget()
{
    if(view_ == nullptr)
    {
        view_ = new QLabel();
        view_->setFixedSize(128, 128);
        view_->setPixmap(*noise_view_);
    }

    return view_;
}
