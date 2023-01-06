#include "numbersourcedatamodel.h"

NumberSourceDataModel::NumberSourceDataModel() :
    data_(std::make_shared<IntegerData>(0)),
    spin_box_{nullptr}
{
}

QJsonObject NumberSourceDataModel::save() const
{
    // TODO: Serialization
    return QJsonObject();
}

void NumberSourceDataModel::load(QJsonObject const& obj)
{
    // TODO: Deserialization
}

unsigned int NumberSourceDataModel::nPorts(QtNodes::PortType port_type) const
{
    unsigned int result = 0;

    if(port_type == QtNodes::PortType::Out)
        result = 1;

    return result;
}

QtNodes::NodeDataType NumberSourceDataModel::dataType(
        QtNodes::PortType port_type,
        QtNodes::PortIndex port_index) const
{
    return data_->type();
}

std::shared_ptr<QtNodes::NodeData> NumberSourceDataModel::outData(
        QtNodes::PortIndex port)
{
    return data_;
}

QWidget* NumberSourceDataModel::embeddedWidget()
{
    if(spin_box_ == nullptr)
    {
        spin_box_ = new QSpinBox();
        spin_box_->setSingleStep(1);
        spin_box_->setMinimumSize(100, 25);
        spin_box_->setRange(0, 9999);

        connect(spin_box_, &QSpinBox::valueChanged,
                this, &NumberSourceDataModel::OnValueChanged_);

        spin_box_->setValue(data_->value());
    }

    return spin_box_;
}

void NumberSourceDataModel::OnValueChanged_(int new_val)
{
    data_->SetValue(new_val);
    emit dataUpdated(0);
}
