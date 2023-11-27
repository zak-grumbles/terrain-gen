#include "noisepropertiespopupwidget.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qspinbox.h"

NoisePropertiesPopupWidget::NoisePropertiesPopupWidget(
    std::shared_ptr<NoiseData> noise,
    QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(CreateGeneralSettings_(noise));
    layout->addWidget(CreateFractalSettings_(noise));

    setLayout(layout);
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget()
{
}

void NoisePropertiesPopupWidget::OnSeedSpinBoxChange_(int new_seed)
{
    emit SeedChanged(new_seed);
}

void NoisePropertiesPopupWidget::OnFreqSpinBoxChange_(double new_freq)
{
    emit FrequencyChanged((float)new_freq);
}

void NoisePropertiesPopupWidget::OnRotationTypeChange_(int new_index)
{
    if(new_index != -1) {
        FastNoiseLite::RotationType3D new_type = rotation_type_
            ->itemData(new_index).value<FastNoiseLite::RotationType3D>();
        emit RotationType3DChanged(new_type);
    }
}

void NoisePropertiesPopupWidget::OnFractalTypeChange_(int new_index)
{
    if(new_index != -1)
    {
        FastNoiseLite::FractalType new_type = fractal_type_
              ->itemData(new_index).value<FastNoiseLite::FractalType>();

    }
}

QGroupBox* NoisePropertiesPopupWidget::CreateGeneralSettings_(std::shared_ptr<NoiseData> noise)
{
    QGroupBox* general_settings = new QGroupBox(tr("General"));

    QSpinBox* seed = new QSpinBox();
    seed->setRange(-9999, 9999);
    seed->setValue(noise->GetNoiseSeed());

    QDoubleSpinBox* frequency = new QDoubleSpinBox();
    frequency->setRange(0.0, 10.0);
    frequency->setDecimals(3);
    frequency->setValue(noise->GetFrequency());
    frequency->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

    rotation_type_ = new QComboBox();
    rotation_type_->addItem(tr("None"), QVariant(FastNoiseLite::RotationType3D_None));
    rotation_type_->addItem(tr("Improve XY Planes"), QVariant(FastNoiseLite::RotationType3D_ImproveXYPlanes));
    rotation_type_->addItem(tr("Improve XZ Planes"), QVariant(FastNoiseLite::RotationType3D_ImproveXZPlanes));

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Seed")), 0, 0);
    layout->addWidget(seed, 0, 1);

    layout->addWidget(new QLabel(tr("Frequency")), 1, 0);
    layout->addWidget(frequency, 1, 1);

    layout->addWidget(new QLabel(tr("Rotation Type 3D")), 2, 0);
    layout->addWidget(rotation_type_, 2, 1);

    general_settings->setLayout(layout);

    connect(
        seed, &QSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnSeedSpinBoxChange_
    );
    connect(
        frequency, &QDoubleSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnFreqSpinBoxChange_
    );
    connect(
        rotation_type_, &QComboBox::currentIndexChanged,
        this, &NoisePropertiesPopupWidget::OnRotationTypeChange_
    );

    return general_settings;
}

QGroupBox* NoisePropertiesPopupWidget::CreateFractalSettings_(std::shared_ptr<NoiseData> noise)
{
    QGroupBox* fractal_settings = new QGroupBox(tr("Fractal"));

    // Fractal type
    fractal_type_ = new QComboBox();
    fractal_type_->addItem(tr("None"), QVariant(FastNoiseLite::FractalType_None));
    fractal_type_->addItem(tr("Domain Warp Independent"),
                           QVariant(FastNoiseLite::FractalType_DomainWarpIndependent));
    fractal_type_->addItem(tr("Domain Warp Progressive"),
                           QVariant(FastNoiseLite::FractalType_DomainWarpProgressive));
    fractal_type_->addItem(tr("FBm"),
                           QVariant(FastNoiseLite::FractalType_FBm));
    fractal_type_->addItem(tr("Ping Pong"),
                           QVariant(FastNoiseLite::FractalType_PingPong));
    fractal_type_->addItem(tr("Ridged"),
                           QVariant(FastNoiseLite::FractalType_Ridged));

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Type")), 0, 0);
    layout->addWidget(fractal_type_, 0, 1);

    fractal_settings->setLayout(layout);

    connect(
        fractal_type_, &QComboBox::currentIndexChanged,
        this, &NoisePropertiesPopupWidget::OnFractalTypeChange_
    );

    return fractal_settings;
}
