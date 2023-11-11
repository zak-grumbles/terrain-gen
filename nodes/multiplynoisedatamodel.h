#ifndef MULTIPLYNOISEDATAMODEL_H
#define MULTIPLYNOISEDATAMODEL_H

#include "nodes/data/noisedata.h"
#include <QObject>

#include <QtNodes/NodeDelegateModel>

class MultiplyNoiseDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    MultiplyNoiseDataModel();
    virtual ~MultiplyNoiseDataModel() {}

    QString caption() const override { return QStringLiteral("Multiply"); }

    bool captionVisible() const override { return true; }

    QString name() const override { return QStringLiteral("MultiplyNoise"); }

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
    std::shared_ptr<NoiseData> inputA_ = nullptr;
    std::shared_ptr<NoiseData> inputB_ = nullptr;

    std::shared_ptr<NoiseData> output_ = nullptr;
};

#endif // MULTIPLYNOISEDATAMODEL_H
