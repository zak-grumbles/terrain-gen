#include "noisesamplerdatamodel.h"

#include "data/integerdata.h"

NoiseSamplerDataModel::NoiseSamplerDataModel()
    : noise_data_(std::make_shared<NoiseSamplerData>()) {}

QJsonObject NoiseSamplerDataModel::save() const {
    QJsonObject modelJson = QtNodes::NodeDelegateModel::save();

    // TODO: Serialization logic

    return modelJson;
}

void NoiseSamplerDataModel::load(QJsonObject const& model) {
    // TODO: Deserialization logic
}

unsigned int NoiseSamplerDataModel::nPorts(QtNodes::PortType port_type) const {
    unsigned int result = 1;

    switch (port_type) {
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

QtNodes::NodeDataType NoiseSamplerDataModel::dataType(
    QtNodes::PortType port_type, QtNodes::PortIndex port_index
) const {
    QtNodes::NodeDataType type = noise_data_->type();

    if (port_type == QtNodes::PortType::In) {
        type      = IntegerData().type();
        type.name = "Seed (int)";
    }

    return type;
}

void NoiseSamplerDataModel::setInData(
    std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port_index
) {
    auto seed_data = std::dynamic_pointer_cast<IntegerData>(data);

    if (seed_data != nullptr) {
        noise_data_->GetNoiseSettings()->seed = seed_data->value();
        noise_data_->NoiseSettingsChanged();
        emit dataUpdated(port_index);
    } else {
        emit dataInvalidated(port_index);
    }
}

std::shared_ptr<QtNodes::NodeData> NoiseSamplerDataModel::outData(
    QtNodes::PortIndex port
) {
    return noise_data_;
}

QWidget* NoiseSamplerDataModel::embeddedWidget() {
    if (selector_ == nullptr) {
        selector_ = new NoiseTypeSelectorWidget();
        connect(
            selector_, &NoiseTypeSelectorWidget::OpenPropertiesWindow, this,
            &NoiseSamplerDataModel::OnOpenPropertiesWindow_
        );
    }
    return selector_;
}

void NoiseSamplerDataModel::OnOpenPropertiesWindow_() {
    if (properties_dlg_ == nullptr) {
        properties_dlg_ = new NoisePropertiesPopupWidget(noise_data_->GetNoiseSettings());
        properties_dlg_->setWindowFlag(Qt::Dialog);
        properties_dlg_->setWindowFlag(Qt::FramelessWindowHint);

        connect(
            properties_dlg_, &NoisePropertiesPopupWidget::NoiseSettingsChanged,
            this, &NoiseSamplerDataModel::OnNoiseSettingsChanged_
        );
    }
    properties_dlg_->show();
}

void NoiseSamplerDataModel::OnNoiseSettingsChanged_() {
    noise_data_->NoiseSettingsChanged();
    emit dataUpdated(0);
}
