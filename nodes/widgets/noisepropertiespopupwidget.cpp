#include "noisepropertiespopupwidget.h"

NoisePropertiesPopupWidget::NoisePropertiesPopupWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    seed_ = new QSpinBox(this);
    frequency_ = new QSpinBox(this);
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget()
{
}
