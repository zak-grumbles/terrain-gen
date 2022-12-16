#include "noisesourcedatamodel.h"
#include "data/integerdata.h"

NoiseSourceDataModel::NoiseSourceDataModel() :
    noise_data_(std::make_shared<NoiseData>()),
    view_{nullptr}
{
}

QJsonObject NoiseSourceDataModel::save() const
{
    QJsonObject modelJson = QtNodes::NodeDelegateModel::save();

    // TODO: Serialization logic

    return modelJson;
}

void NoiseSourceDataModel::load(QJsonObject const& model)
{
    // TODO: Deserialization logic
}

unsigned int NoiseSourceDataModel::nPorts(QtNodes::PortType port_type) const
{
    unsigned int result = 1;

    switch(port_type)
    {
    case QtNodes::PortType::In:
        result = 1;
        break;
    case QtNodes::PortType::Out:
        result = 1;
    default:
        break;
    }

    return result;
}

QtNodes::NodeDataType NoiseSourceDataModel::dataType(
        QtNodes::PortType port_type,
        QtNodes::PortIndex port_index) const
{
    QtNodes::NodeDataType type = noise_data_->type();

    if(port_type == QtNodes::PortType::In)
    {
        type = IntegerData().type();
    }

    return type;
}

void NoiseSourceDataModel::setInData(
        std::shared_ptr<QtNodes::NodeData> data,
        QtNodes::PortIndex port_index)
{
    auto seed_data = std::dynamic_pointer_cast<IntegerData>(data);

    if(seed_data != nullptr)
    {
        noise_data_->SetNoiseSeed(seed_data->value());
        emit dataUpdated(port_index);
    }
    else
    {
        emit dataInvalidated(port_index);
    }
}

std::shared_ptr<QtNodes::NodeData> NoiseSourceDataModel::outData(
        QtNodes::PortIndex port)
{
    return noise_data_;
}

QWidget* NoiseSourceDataModel::embeddedWidget()
{
    if(view_ == nullptr)
    {
        view_ = new NoiseViewWidget(noise_data_);
        connect(view_, &NoiseViewWidget::NoiseTypeChanged,
                this, &NoiseSourceDataModel::OnNoiseTypeChanged_);
    }
    return view_;
}


void NoiseSourceDataModel::OnNoiseTypeChanged_(FastNoiseLite::NoiseType new_type)
{
    noise_data_->SetNoiseType(new_type);
    view_->UpdateNoise();
    emit dataUpdated(0);
}
