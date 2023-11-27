#ifndef NOISESAMPLERDATAMODEL_H
#define NOISESAMPLERDATAMODEL_H

#include <QComboBox>
#include <QtNodes/NodeDelegateModel>

#include "nodes/data/noisesamplerdata.h"
#include "nodes/widgets/noisepropertiespopupwidget.h"
#include "nodes/widgets/noisetypeselectorwidget.h"

class NoiseSamplerDataModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    NoiseSamplerDataModel();

    virtual ~NoiseSamplerDataModel() {}

    QString caption() const override { return QStringLiteral("Noise Sampler"); }

    bool captionVisible() const override { return true; }

    QString name() const override { return QStringLiteral("NoiseSampler"); }

    QJsonObject save() const override;

    void load(QJsonObject const& obj) override;

    unsigned int nPorts(QtNodes::PortType port_type) const override;

    QtNodes::NodeDataType dataType(
        QtNodes::PortType port_type, QtNodes::PortIndex port_index
    ) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port
    ) override;

    void setInData(
        std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex index
    ) override;

    QWidget* embeddedWidget() override;

protected slots:
    void OnOpenPropertiesWindow_();
    void OnNoiseSettingsChanged_();

protected:
    std::shared_ptr<NoiseSamplerData> noise_data_;

    NoiseTypeSelectorWidget* selector_          = nullptr;
    NoisePropertiesPopupWidget* properties_dlg_ = nullptr;
};

#endif  // NOISESAMPLERDATAMODEL_H
