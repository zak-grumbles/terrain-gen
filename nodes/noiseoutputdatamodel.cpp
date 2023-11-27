#include "noiseoutputdatamodel.h"

#include "qlabel.h"

NoiseOutputDataModel::NoiseOutputDataModel()
    : output_data_{nullptr}, view_{nullptr} {
    noise_view_ = new QPixmap();
}

QJsonObject NoiseOutputDataModel::save() const {
    QJsonObject model = QtNodes::NodeDelegateModel::save();

    return model;
}

void NoiseOutputDataModel::load(QJsonObject const& model) {
    // TODO
}

unsigned int NoiseOutputDataModel::nPorts(QtNodes::PortType port_type) const {
    unsigned int result = 0;

    if (port_type == QtNodes::PortType::In) {
        result = 1;
    }

    return result;
}

QtNodes::NodeDataType NoiseOutputDataModel::dataType(
    QtNodes::PortType port_type, QtNodes::PortIndex port_index
) const {
    QtNodes::NodeDataType type;

    if (port_type == QtNodes::PortType::In ||
        port_type == QtNodes::PortType::Out) {
        type = HeightData().type();
    }

    return type;
}

void NoiseOutputDataModel::setInData(
    std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port_index
) {
    auto height_data = std::dynamic_pointer_cast<HeightData>(data);

    // Data is null when connection is removed, which is valid
    // If connection is created, and height_data is not null,
    // we have valid height data
    if (data == nullptr || height_data != nullptr) {
        output_data_ = height_data;

        if (noise_view_ != nullptr) {
            delete noise_view_;
        }
        noise_view_ = output_data_->AsBitmap(0, 0, 256, 256);
        view_->setPixmap(*noise_view_);

        emit dataUpdated(port_index);
    }
    // This indicates data is not null, but is also not a valid HeightData
    else {
        emit dataInvalidated(port_index);
    }
}

std::shared_ptr<QtNodes::NodeData> NoiseOutputDataModel::outData(
    QtNodes::PortIndex
) {
    return output_data_;
}

QWidget* NoiseOutputDataModel::embeddedWidget() {
    if (view_ == nullptr) {
        view_ = new QLabel();
        view_->setFixedSize(128, 128);
        view_->setPixmap(*noise_view_);
    }

    return view_;
}
