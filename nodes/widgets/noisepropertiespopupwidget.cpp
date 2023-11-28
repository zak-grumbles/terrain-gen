#include "noisepropertiespopupwidget.h"

#include "qboxlayout.h"
#include "qlabel.h"
#include "qspinbox.h"

NoisePropertiesPopupWidget::NoisePropertiesPopupWidget(
    std::shared_ptr<NoiseData> noise, QWidget* parent /*= nullptr*/
)
    : QWidget(parent) {
    noise_settings_ = noise;

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(CreateGeneralSettings_());
    layout->addWidget(CreateFractalSettings_());

    setLayout(layout);
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget() {}

void NoisePropertiesPopupWidget::OnNoiseTypeChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::NoiseType new_type =
            noise_type_->itemData(new_index).value<FastNoiseLite::NoiseType>();
        noise_settings_->noise_type = new_type;
        emit NoiseSettingsChanged();
    }
}

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
        FastNoiseLite::FractalType new_type =
            fractal_type_->itemData(new_index)
                .value<FastNoiseLite::FractalType>();

        // We are switching to None from some non-None type
        if (new_type == FastNoiseLite::FractalType_None &&
            noise_settings_->fractal_type != FastNoiseLite::FractalType_None) {
            SetEnabled_(false);
        }
        // We are switching to a non-None type from None
        else if (noise_settings_->fractal_type == FastNoiseLite::FractalType_None) {
            SetEnabled_(true);
        }
        noise_settings_->fractal_type = new_type;

        emit NoiseSettingsChanged();
    }
}

void NoisePropertiesPopupWidget::OnOctavesChange_(int new_value) {
    noise_settings_->octaves = new_value;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnLacunarityChange_(double new_value) {
    noise_settings_->lacunarity = new_value;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnGainChange_(double new_value) {
    noise_settings_->gain = new_value;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnWeightedStrengthChange_(double new_value) {
    noise_settings_->weighted_strength = new_value;
    emit NoiseSettingsChanged();
}

void NoisePropertiesPopupWidget::OnPingPongStrengthChange_(double new_value) {
    noise_settings_->pingpong_strength = new_value;
    emit NoiseSettingsChanged();
}

QGroupBox* NoisePropertiesPopupWidget::CreateGeneralSettings_() {
    QGroupBox* general_settings = new QGroupBox(tr("General"));
    QGridLayout* layout         = new QGridLayout();

    noise_type_ = CreateNoiseTypeComboBox_();
    int current_type_index =
        noise_type_->findData(QVariant(noise_settings_->noise_type));
    if (current_type_index != -1) {
        noise_type_->setCurrentIndex(current_type_index);
    }
    layout->addWidget(new QLabel(tr("Noise Type")), 0, 0);
    layout->addWidget(noise_type_, 0, 1);

    QSpinBox* seed = new QSpinBox();
    seed->setRange(-9999, 9999);
    seed->setValue(noise_settings_->seed);
    layout->addWidget(new QLabel(tr("Seed")), 1, 0);
    layout->addWidget(seed, 1, 1);

    QDoubleSpinBox* frequency = new QDoubleSpinBox();
    frequency->setRange(0.0, 10.0);
    frequency->setDecimals(3);
    frequency->setValue(noise_settings_->frequency);
    frequency->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    layout->addWidget(new QLabel(tr("Frequency")), 2, 0);
    layout->addWidget(frequency, 2, 1);

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
    layout->addWidget(new QLabel(tr("Rotation Type 3D")), 3, 0);
    layout->addWidget(rotation_type_, 3, 1);

    general_settings->setLayout(layout);

    connect(
        noise_type_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnNoiseTypeChange_
    );
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

QComboBox* NoisePropertiesPopupWidget::CreateNoiseTypeComboBox_() {
    QComboBox* result = new QComboBox();

    result->addItem(
        tr("Open Simplex 2"), QVariant(FastNoiseLite::NoiseType_OpenSimplex2)
    );
    result->addItem(
        tr("Open Simplex 2S"), QVariant(FastNoiseLite::NoiseType_OpenSimplex2S)
    );
    result->addItem(
        tr("Cellular"), QVariant(FastNoiseLite::NoiseType_Cellular)
    );
    result->addItem(tr("Perlin"), QVariant(FastNoiseLite::NoiseType_Perlin));
    result->addItem(
        tr("Value Cubic"), QVariant(FastNoiseLite::NoiseType_ValueCubic)
    );
    result->addItem(tr("Value"), QVariant(FastNoiseLite::NoiseType_Value));

    return result;
}

QGroupBox* NoisePropertiesPopupWidget::CreateFractalSettings_() {
    QGroupBox* fractal_settings = new QGroupBox(tr("Fractal"));

    QGridLayout* layout = new QGridLayout();

    // Fractal type
    fractal_type_ = new QComboBox();
    fractal_type_->addItem(
        tr("None"), QVariant(FastNoiseLite::FractalType_None)
    );
    fractal_type_->addItem(tr("FBm"), QVariant(FastNoiseLite::FractalType_FBm));
    fractal_type_->addItem(
        tr("Ping Pong"), QVariant(FastNoiseLite::FractalType_PingPong)
    );
    fractal_type_->addItem(
        tr("Ridged"), QVariant(FastNoiseLite::FractalType_Ridged)
    );
    fractal_type_->addItem(
        tr("Domain Warp Independent"),
        QVariant(FastNoiseLite::FractalType_DomainWarpIndependent)
    );
    fractal_type_->addItem(
        tr("Domain Warp Progressive"),
        QVariant(FastNoiseLite::FractalType_DomainWarpProgressive)
    );

    layout->addWidget(new QLabel(tr("Type")), 0, 0);
    layout->addWidget(fractal_type_, 0, 1);

    // Octaves
    fractal_octaves_ = new QSpinBox();
    fractal_octaves_->setRange(1, 100);
    fractal_octaves_->setValue(noise_settings_->octaves);

    layout->addWidget(new QLabel(tr("Octaves")), 1, 0);
    layout->addWidget(fractal_octaves_, 1, 1);

    // Lacunarity
    fractal_lacunarity_ = new QDoubleSpinBox();
    fractal_lacunarity_->setDecimals(3);
    fractal_lacunarity_->setRange(1.0, 10.0);
    fractal_lacunarity_->setValue(noise_settings_->lacunarity);
    fractal_lacunarity_->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

    layout->addWidget(new QLabel(tr("Lacunarity")), 2, 0);
    layout->addWidget(fractal_lacunarity_, 2, 1);

    // Gain
    fractal_gain_ = new QDoubleSpinBox();
    fractal_gain_->setDecimals(3);
    fractal_gain_->setRange(0.0, 10.0);
    fractal_gain_->setValue(noise_settings_->gain);
    fractal_gain_->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);

    layout->addWidget(new QLabel(tr("Gain")), 3, 0);
    layout->addWidget(fractal_gain_, 3, 1);

    // Weighted Strength
    fractal_weighted_str_ = new QDoubleSpinBox();
    fractal_weighted_str_->setDecimals(3);
    fractal_weighted_str_->setRange(0.0, 10.0);
    fractal_weighted_str_->setValue(noise_settings_->weighted_strength);
    fractal_weighted_str_->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType
    );

    layout->addWidget(new QLabel(tr("Weighted Strength")), 4, 0);
    layout->addWidget(fractal_weighted_str_, 4, 1);

    // Ping Pong Str
    fractal_pingpong_str_ = new QDoubleSpinBox();
    fractal_pingpong_str_->setDecimals(3);
    fractal_pingpong_str_->setRange(0.0, 10.0);
    fractal_pingpong_str_->setValue(noise_settings_->pingpong_strength);
    fractal_pingpong_str_->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType
    );

    layout->addWidget(new QLabel(tr("Ping Pong Strength")), 5, 0);
    layout->addWidget(fractal_pingpong_str_, 5, 1);

    fractal_settings->setLayout(layout);

    // Connect signals
    connect(
        fractal_type_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnFractalTypeChange_
    );
    connect(
        fractal_octaves_, &QSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnOctavesChange_
    );
    connect(
        fractal_lacunarity_, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnLacunarityChange_
    );
    connect(
        fractal_gain_, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnGainChange_
    );
    connect(
        fractal_weighted_str_, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnWeightedStrengthChange_
    );
    connect(
        fractal_pingpong_str_, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnPingPongStrengthChange_
    );

    return fractal_settings;
}

void NoisePropertiesPopupWidget::SetEnabled_(bool enabled /*= true*/) {
    fractal_octaves_->setEnabled(enabled);
}
