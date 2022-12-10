#ifndef NOISESOURCEDATAMODEL_H
#define NOISESOURCEDATAMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QComboBox>

#include "noisedata.h"

enum NoiseSourceType
{
    kPerlin = 0,
    kValue,
    kOpenSimplex2,
    kOpenSimplex2S
};

class NoiseSourceDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    NoiseSourceDataModel();

    virtual ~NoiseSourceDataModel() {}

    QString caption() const override { return QStringLiteral("Noise Source"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("NoiseSource"); }

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
            QtNodes::PortIndex index) override
    {}

    QWidget* embeddedWidget() override;

protected slots:
    void OnNoiseTypeChanged_(NoiseSourceType new_type);

protected:
    std::shared_ptr<NoiseData> noise_data_;

    QComboBox* combo_box_;
};

#endif // NOISESOURCEDATAMODEL_H
