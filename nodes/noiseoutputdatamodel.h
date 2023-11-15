#ifndef NOISEOUTPUTDATAMODEL_H
#define NOISEOUTPUTDATAMODEL_H

#include <QObject>
#include <QtNodes/NodeDelegateModel>

#include "nodes/data/heightdata.h"
#include "qlabel.h"

class NoiseOutputDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    NoiseOutputDataModel();
    virtual ~NoiseOutputDataModel() {}

    QString caption() const override { return QStringLiteral("Noise Output"); }

    bool captionVisible() const override { return true; }

    QString name() const override { return QStringLiteral("NoiseOutput"); }

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
            QtNodes::PortIndex index) override;

    QWidget* embeddedWidget() override;

protected:
    std::shared_ptr<HeightData> output_data_;

    QPixmap* noise_view_ = nullptr;
    QLabel* view_ = nullptr;
};

#endif // NOISEOUTPUTDATAMODEL_H
