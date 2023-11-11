#include "multiplynoisedatamodel.h"

MultiplyNoiseDataModel::MultiplyNoiseDataModel()
{

}

QJsonObject MultiplyNoiseDataModel::save() const
{
    QJsonObject model = QtNodes::NodeDelegateModel::save();
    return model;
}


void MultiplyNoiseDataModel::load(QJsonObject const& obj)
{
    // TODO
}


unsigned int MultiplyNoiseDataModel::nPorts(QtNodes::PortType port_type) const
{
    unsigned int result = 0;

    if(port_type == QtNodes::PortType::In)
    {
        result = 2;
    }
    else if(port_type == QtNodes::PortType::Out)
    {
        result = 1;
    }

    return result;
}


QtNodes::NodeDataType MultiplyNoiseDataModel::dataType(
    QtNodes::PortType port_type,
    QtNodes::PortIndex port_index) const
{
    QtNodes::NodeDataType type;

    if(port_type == QtNodes::PortType::In ||
        port_type == QtNodes::PortType::Out)
    {
        type = NoiseData().type();
    }

    return type;
}


std::shared_ptr<QtNodes::NodeData> MultiplyNoiseDataModel::outData(
        QtNodes::PortIndex port)
{
    return output_;
}


void MultiplyNoiseDataModel::setInData(
        std::shared_ptr<QtNodes::NodeData> data,
        QtNodes::PortIndex index)
{
    auto noiseIn = std::dynamic_pointer_cast<NoiseData>(data);

    if(noiseIn != nullptr)
    {
        if(index == 0)
        {
            inputA_ = noiseIn;
        }
        else if(index == 1)
        {
            inputB_ = noiseIn;
        }

        emit dataUpdated(index);
    }
    else
    {
        emit dataInvalidated(index);
    }
}


QWidget* MultiplyNoiseDataModel::embeddedWidget()
{
    return nullptr;
}
