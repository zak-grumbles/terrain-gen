#include "noisetypeselectorwidget.h"
#include "qboxlayout.h"

NoiseTypeSelectorWidget::NoiseTypeSelectorWidget(QWidget* parent /*= nullptr*/)
    : QWidget{parent}
{
    properties_btn_ = new QPushButton();
    properties_btn_->setText("...");

    noise_picker_ = new QComboBox();

    QStringList opts;
    opts.append("Perlin");
    opts.append("Value");
    opts.append("Open Simplex2");
    opts.append("Open Simplex2S");

    noise_picker_->addItems(opts);
    noise_picker_->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(noise_picker_, &QComboBox::currentIndexChanged,
            this, &NoiseTypeSelectorWidget::OnSelectorChanged_);

    connect(properties_btn_, &QPushButton::clicked,
            this, &NoiseTypeSelectorWidget::OnPropertiesBtnClicked_);

    layout_ = new QHBoxLayout(this);
    layout_->addWidget(noise_picker_);
    layout_->addWidget(properties_btn_);

    this->setAttribute(Qt::WA_TranslucentBackground);
}

void NoiseTypeSelectorWidget::OnSelectorChanged_(int new_type)
{
    NoiseSelectorType type = static_cast<NoiseSelectorType>(new_type);
    emit NoiseTypeChanged(ToFastNoiseEnum_(type));
}

void NoiseTypeSelectorWidget::OnPropertiesBtnClicked_(bool checked /*= false*/)
{
    emit OpenPropertiesWindow();
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
