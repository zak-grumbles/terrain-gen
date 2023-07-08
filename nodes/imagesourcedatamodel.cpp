#include "imagesourcedatamodel.h"

#include <QFileDialog>

ImageSourceDataModel::ImageSourceDataModel()
    : data_(std::make_shared<ImageSourceData>())
{
    view_ = new ImageSourceViewWidget();
}

ImageSourceDataModel::~ImageSourceDataModel()
{
    if(view_ != nullptr)
    {
        delete view_;
        view_ = nullptr;
    }
}

QJsonObject ImageSourceDataModel::save() const
{
    return QJsonObject();
}

void ImageSourceDataModel::load(QJsonObject const& obj)
{

}

unsigned int ImageSourceDataModel::nPorts(QtNodes::PortType port_type) const
{
    unsigned int result = 0;

    if(port_type == QtNodes::PortType::Out)
        result = 1;

    return result;
}

QtNodes::NodeDataType ImageSourceDataModel::dataType(
    QtNodes::PortType port_type,
    QtNodes::PortIndex port_index) const
{
    return data_->type();
}

std::shared_ptr<QtNodes::NodeData> ImageSourceDataModel::outData(
    QtNodes::PortIndex port)
{
    return data_;
}

QWidget* ImageSourceDataModel::embeddedWidget()
{
    return view_;
}
