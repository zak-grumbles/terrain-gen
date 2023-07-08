#ifndef IMAGESOURCEDATAMODEL_H
#define IMAGESOURCEDATAMODEL_H

#include <QtNodes/NodeDelegateModel>

#include <QLabel>
#include <QPushButton>

#include "nodes/data/imagesourcedata.h"
#include "nodes/widgets/imagesourceviewwidget.h"

class ImageSourceDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    ImageSourceDataModel();

    virtual ~ImageSourceDataModel();

    QString caption() const override { return QStringLiteral("Image Source"); }

    bool captionVisible() const override { return true; }

    QString name() const override { return QStringLiteral("ImageSource"); }

    QJsonObject save() const override;

    void load(QJsonObject const& obj) override;

    unsigned int nPorts(QtNodes::PortType port_type) const override;

    QtNodes::NodeDataType dataType(
            QtNodes::PortType port_type,
            QtNodes::PortIndex port_index) const override;

    std::shared_ptr<QtNodes::NodeData> outData(
            QtNodes::PortIndex port) override;

    void setInData(
        std::shared_ptr<QtNodes::NodeData> data,
        QtNodes::PortIndex index) override {}

    QWidget* embeddedWidget() override;

protected:
    std::shared_ptr<ImageSourceData> data_ = nullptr;

    ImageSourceViewWidget* view_ = nullptr;
};

#endif // IMAGESOURCEDATAMODEL_H
