#include "noisetypeselectorwidget.h"

NoiseTypeSelectorWidget::NoiseTypeSelectorWidget(QWidget* parent /*= nullptr*/)
    : QWidget{parent}
{
    noise_picker_ = new QComboBox(this);

    QStringList opts;
    opts.append("Perlin");
    opts.append("Value");
    opts.append("Open Simplex2");
    opts.append("Open Simplex2S");

    noise_picker_->addItems(opts);
    noise_picker_->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(noise_picker_, &QComboBox::currentIndexChanged,
            this, &NoiseTypeSelectorWidget::OnSelectorChanged_);

}

void NoiseTypeSelectorWidget::OnSelectorChanged_(int new_type)
{
    NoiseSelectorType type = static_cast<NoiseSelectorType>(new_type);
    emit NoiseTypeChanged(ToFastNoiseEnum_(type));
}

FastNoiseLite::NoiseType NoiseTypeSelectorWidget::ToFastNoiseEnum_(NoiseSelectorType type) const
{
    FastNoiseLite::NoiseType result = FastNoiseLite::NoiseType_Perlin;
    switch(type)
    {
    case NoiseSelectorType::kValue:
        result = FastNoiseLite::NoiseType_Value;
        break;
    case NoiseSelectorType::kOpenSimplex2:
        result = FastNoiseLite::NoiseType_OpenSimplex2;
        break;
    case NoiseSelectorType::kOpenSimplex2S:
        result = FastNoiseLite::NoiseType_OpenSimplex2S;
        break;
    case NoiseSelectorType::kPerlin:
    default:
        break;
    }
    return result;
}
