#include "noisepropertiespopupwidget.h"

NoisePropertiesPopupWidget::NoisePropertiesPopupWidget(
    std::shared_ptr<NoiseData> noise,
    QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{

    seed_ = new QSpinBox();
    seed_->setRange(-9999, 9999);
    seed_->setValue(noise->GetNoiseSeed());

    frequency_ = new QDoubleSpinBox();
    frequency_->setRange(0.0, 10.0);
    frequency_->setDecimals(2);
    frequency_->setValue(noise->GetFrequency());
    frequency_->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

    layout_ = new QVBoxLayout(this);
    layout_->addWidget(seed_);
    layout_->addWidget(frequency_);

    connect(
        seed_, &QSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnSeedSpinBoxChange
    );
    connect(
        frequency_, &QDoubleSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnFreqSpinBoxChange
    );
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget()
{
}

void NoisePropertiesPopupWidget::OnSeedSpinBoxChange(int new_seed)
{
    emit SeedChanged(new_seed);
}

void NoisePropertiesPopupWidget::OnFreqSpinBoxChange(double new_freq)
{
    emit FrequencyChanged((float)new_freq);
}
