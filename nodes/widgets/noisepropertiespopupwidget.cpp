#include "noisepropertiespopupwidget.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qspinbox.h"

NoisePropertiesPopupWidget::NoisePropertiesPopupWidget(
    std::shared_ptr<NoiseData> noise, QWidget* parent /*= nullptr*/
) : QWidget(parent) {
    noise_settings_ = noise;

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(CreateGeneralSettings_());
    layout->addWidget(CreateFractalSettings_());

    setLayout(layout);
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget() {}

void NoisePropertiesPopupWidget::OnSeedSpinBoxChange_(int new_seed) {
    noise_settings_->seed = new_seed;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnFreqSpinBoxChange_(double new_freq) {
    noise_settings_->frequency = new_freq;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnRotationTypeChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::RotationType3D new_type =
            rotation_type_->itemData(new_index)
                .value<FastNoiseLite::RotationType3D>();
        noise_settings_->rotation_type = new_type;
        emit NoiseSettingsChanged();
    }
}

void NoisePropertiesPopupWidget::OnFractalTypeChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::FractalType new_type = fractal_type_->itemData(new_index)
            .value<FastNoiseLite::FractalType>();

        // We are switching to None from some non-None type
        if(new_type == FastNoiseLite::FractalType_None &&
            noise_settings_->fractal_type != FastNoiseLite::FractalType_None) {
            SetEnabled_(false);
        }
        // We are switching to a non-None type from None
        else if(noise_settings_->fractal_type == FastNoiseLite::FractalType_None){
            SetEnabled_(true);
        }
        noise_settings_->fractal_type = new_type;

        emit NoiseSettingsChanged();
    }
}

QGroupBox* NoisePropertiesPopupWidget::CreateGeneralSettings_() {
    QGroupBox* general_settings = new QGroupBox(tr("General"));

    QSpinBox* seed = new QSpinBox();
    seed->setRange(-9999, 9999);
    seed->setValue(noise_settings_->seed);

    QDoubleSpinBox* frequency = new QDoubleSpinBox();
    frequency->setRange(0.0, 10.0);
    frequency->setDecimals(3);
    frequency->setValue(noise_settings_->frequency);
    frequency->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

    rotation_type_ = new QComboBox();
    rotation_type_->addItem(
        tr("None"), QVariant(FastNoiseLite::RotationType3D_None)
    );
    rotation_type_->addItem(
        tr("Improve XY Planes"),
        QVariant(FastNoiseLite::RotationType3D_ImproveXYPlanes)
    );
    rotation_type_->addItem(
        tr("Improve XZ Planes"),
        QVariant(FastNoiseLite::RotationType3D_ImproveXZPlanes)
    );

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Seed")), 0, 0);
    layout->addWidget(seed, 0, 1);

    layout->addWidget(new QLabel(tr("Frequency")), 1, 0);
    layout->addWidget(frequency, 1, 1);

    layout->addWidget(new QLabel(tr("Rotation Type 3D")), 2, 0);
    layout->addWidget(rotation_type_, 2, 1);

    general_settings->setLayout(layout);

    connect(
        seed, &QSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnSeedSpinBoxChange_
    );
    connect(
        frequency, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnFreqSpinBoxChange_
    );
    connect(
        rotation_type_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnRotationTypeChange_
    );

    return general_settings;
}

QGroupBox* NoisePropertiesPopupWidget::CreateFractalSettings_() {
    QGroupBox* fractal_settings = new QGroupBox(tr("Fractal"));

    QGridLayout* layout = new QGridLayout();

    // Fractal type
    fractal_type_ = new QComboBox();
    fractal_type_->addItem(
        tr("None"), QVariant(FastNoiseLite::FractalType_None)
    );
    fractal_type_->addItem(
        tr("Domain Warp Independent"),
        QVariant(FastNoiseLite::FractalType_DomainWarpIndependent)
    );
    fractal_type_->addItem(
        tr("Domain Warp Progressive"),
        QVariant(FastNoiseLite::FractalType_DomainWarpProgressive)
    );
    fractal_type_->addItem(tr("FBm"), QVariant(FastNoiseLite::FractalType_FBm));
    fractal_type_->addItem(
        tr("Ping Pong"), QVariant(FastNoiseLite::FractalType_PingPong)
    );
    fractal_type_->addItem(
        tr("Ridged"), QVariant(FastNoiseLite::FractalType_Ridged)
    );

    layout->addWidget(new QLabel(tr("Type")), 0, 0);
    layout->addWidget(fractal_type_, 0, 1);

    // Octaves
    fractal_octaves_ = new QSpinBox();
    fractal_octaves_->setRange(1, 100);

    layout->addWidget(new QLabel(tr("Octaves")), 1, 0);
    layout->addWidget(fractal_octaves_, 1, 1);

    // Lacunarity

    fractal_settings->setLayout(layout);

    // Connect signals
    connect(
        fractal_type_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnFractalTypeChange_
    );

    return fractal_settings;
}

void NoisePropertiesPopupWidget::SetEnabled_(bool enabled /*= true*/) {
    fractal_octaves_->setEnabled(enabled);
}
