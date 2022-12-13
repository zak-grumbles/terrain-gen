#include "noisesourcedatamodel.h"

NoiseSourceDataModel::NoiseSourceDataModel() :
    noise_data_(std::make_shared<NoiseData>()),
    combo_box_{nullptr}
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
        result = 0;
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
    return noise_data_->type();
}

std::shared_ptr<QtNodes::NodeData> NoiseSourceDataModel::outData(
        QtNodes::PortIndex port)
{
    return noise_data_;
}

QWidget* NoiseSourceDataModel::embeddedWidget()
{
    if(combo_box_ == nullptr)
    {
        QStringList opts;
        opts.append("Perlin");
        opts.append("Value");
        opts.append("Open Simplex2");
        opts.append("Open Simplex2S");

        combo_box_ = new QComboBox();
        combo_box_->addItems(opts);
        combo_box_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
    return combo_box_;
}

void NoiseSourceDataModel::OnNoiseTypeChanged_(NoiseSourceType new_type)
{
    // TODO
}
