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
    layout->addWidget(CreateCellularSettings_());

    setLayout(layout);
}

NoisePropertiesPopupWidget::~NoisePropertiesPopupWidget() {}

void NoisePropertiesPopupWidget::OnNoiseTypeChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::NoiseType new_type =
            noise_type_->itemData(new_index).value<FastNoiseLite::NoiseType>();
        noise_settings_->noise_type = new_type;

        SetCellularSettingsEnabled_(
            new_type == FastNoiseLite::NoiseType_Cellular
        );

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
            noise_settings_->fractal_type = new_type;
            SetFractalSettingsEnabled_(false);
        }
        // We are switching to a non-None type
        else if (new_type != FastNoiseLite::FractalType_None) {
            noise_settings_->fractal_type = new_type;
            SetFractalSettingsEnabled_(true);
        }

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

void NoisePropertiesPopupWidget::OnCellularDistanceFuncChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::CellularDistanceFunction new_func =
            cellular_distance_func_->itemData(new_index)
                .value<FastNoiseLite::CellularDistanceFunction>();

        noise_settings_->cellular_distance_func = new_func;
        emit NoiseSettingsChanged();
    }
}

void NoisePropertiesPopupWidget::OnCellularReturnTypeChange_(int new_index) {
    if (new_index != -1) {
        FastNoiseLite::CellularReturnType new_type =
            cellular_return_type_->itemData(new_index)
                .value<FastNoiseLite::CellularReturnType>();

        noise_settings_->cellular_return_type = new_type;
        emit NoiseSettingsChanged();
    }
}

void NoisePropertiesPopupWidget::OnCellularJitterChange_(double new_value) {
    noise_settings_->jitter = new_value;
    emit NoiseSettingsChanged();
}

QGroupBox* NoisePropertiesPopupWidget::CreateGeneralSettings_() {
    QGroupBox* general_settings = new QGroupBox(tr("General"));
    QGridLayout* layout         = new QGridLayout();

    noise_type_ = CreateNoiseTypeComboBox_();
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

    int current_type_index =
        result->findData(QVariant(noise_settings_->noise_type));
    if (current_type_index != -1) {
        result->setCurrentIndex(current_type_index);
    }

    return result;
}

QGroupBox* NoisePropertiesPopupWidget::CreateFractalSettings_() {
    QGroupBox* fractal_settings = new QGroupBox(tr("Fractal"));

    QGridLayout* layout = new QGridLayout();

    // Fractal type
    fractal_type_ = CreateFractalTypeComboBox_();
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

    SetFractalSettingsEnabled_(
        noise_settings_->fractal_type != FastNoiseLite::FractalType_None
    );

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

QComboBox* NoisePropertiesPopupWidget::CreateFractalTypeComboBox_() {
    QComboBox* type = new QComboBox();

    type->addItem(tr("None"), QVariant(FastNoiseLite::FractalType_None));
    type->addItem(tr("FBm"), QVariant(FastNoiseLite::FractalType_FBm));
    type->addItem(
        tr("Ping Pong"), QVariant(FastNoiseLite::FractalType_PingPong)
    );
    type->addItem(tr("Ridged"), QVariant(FastNoiseLite::FractalType_Ridged));
    type->addItem(
        tr("Domain Warp Independent"),
        QVariant(FastNoiseLite::FractalType_DomainWarpIndependent)
    );
    type->addItem(
        tr("Domain Warp Progressive"),
        QVariant(FastNoiseLite::FractalType_DomainWarpProgressive)
    );
    int current_fractal_index =
        type->findData(QVariant(noise_settings_->fractal_type));
    if (current_fractal_index != -1) {
        type->setCurrentIndex(current_fractal_index);
    }

    return type;
}

void NoisePropertiesPopupWidget::SetFractalSettingsEnabled_(bool enabled) {
    fractal_octaves_->setEnabled(enabled);
    fractal_lacunarity_->setEnabled(enabled);
    fractal_gain_->setEnabled(enabled);
    fractal_weighted_str_->setEnabled(enabled);
    fractal_pingpong_str_->setEnabled(
        noise_settings_->fractal_type == FastNoiseLite::FractalType_PingPong
    );
}

QGroupBox* NoisePropertiesPopupWidget::CreateCellularSettings_() {
    QGroupBox* cellular_settings = new QGroupBox("Cellular");
    QGridLayout* layout          = new QGridLayout();

    cellular_distance_func_ = CreateCellularDistanceFuncComboBox_();
    layout->addWidget(new QLabel("Distance Function"), 0, 0);
    layout->addWidget(cellular_distance_func_, 0, 1);

    cellular_return_type_ = CreateCellularReturnTypeComboBox_();
    layout->addWidget(new QLabel("Return Type"), 1, 0);
    layout->addWidget(cellular_return_type_, 1, 1);

    cellular_jitter_ = new QDoubleSpinBox();
    cellular_jitter_->setRange(0.1, 10.0);
    cellular_jitter_->setValue(noise_settings_->jitter);
    layout->addWidget(new QLabel("Jitter"), 2, 0);
    layout->addWidget(cellular_jitter_, 2, 1);

    cellular_settings->setLayout(layout);

    connect(
        cellular_distance_func_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnCellularDistanceFuncChange_
    );
    connect(
        cellular_return_type_, &QComboBox::currentIndexChanged, this,
        &NoisePropertiesPopupWidget::OnCellularReturnTypeChange_
    );
    connect(
        cellular_jitter_, &QDoubleSpinBox::valueChanged, this,
        &NoisePropertiesPopupWidget::OnCellularReturnTypeChange_
    );

    return cellular_settings;
}

QComboBox* NoisePropertiesPopupWidget::CreateCellularDistanceFuncComboBox_() {
    QComboBox* distance_func = new QComboBox();

    distance_func->addItem(
        "Euclidean", QVariant(FastNoiseLite::CellularDistanceFunction_Euclidean)
    );
    distance_func->addItem(
        "Euclidean Square",
        QVariant(FastNoiseLite::CellularDistanceFunction_EuclideanSq)
    );
    distance_func->addItem(
        "Manhattan", QVariant(FastNoiseLite::CellularDistanceFunction_Manhattan)
    );
    distance_func->addItem(
        "Hybrid", QVariant(FastNoiseLite::CellularDistanceFunction_Hybrid)
    );

    int current_func_index =
        distance_func->findData(QVariant(noise_settings_->cellular_distance_func
        ));
    if (current_func_index != -1) {
        distance_func->setCurrentIndex(current_func_index);
    }

    return distance_func;
}

QComboBox* NoisePropertiesPopupWidget::CreateCellularReturnTypeComboBox_() {
    QComboBox* return_type = new QComboBox();

    return_type->addItem(
        "Cell Value", QVariant(FastNoiseLite::CellularReturnType_CellValue)
    );
    return_type->addItem(
        "Distance", QVariant(FastNoiseLite::CellularReturnType_Distance)
    );
    return_type->addItem(
        "Distance 2", QVariant(FastNoiseLite::CellularReturnType_Distance2)
    );
    return_type->addItem(
        "Distance 2 Add",
        QVariant(FastNoiseLite::CellularReturnType_Distance2Add)
    );
    return_type->addItem(
        "Distance 2 Sub",
        QVariant(FastNoiseLite::CellularReturnType_Distance2Sub)
    );
    return_type->addItem(
        "Distance 2 Mul",
        QVariant(FastNoiseLite::CellularReturnType_Distance2Mul)
    );
    return_type->addItem(
        "Distance 2 Div",
        QVariant(FastNoiseLite::CellularReturnType_Distance2Div)
    );

    int current_type_index =
        return_type->findData(QVariant(noise_settings_->cellular_return_type));
    if (current_type_index != -1) {
        return_type->setCurrentIndex(current_type_index);
    }

    return return_type;
}

void NoisePropertiesPopupWidget::SetCellularSettingsEnabled_(bool enabled) {
    cellular_distance_func_->setEnabled(enabled);
    cellular_return_type_->setEnabled(enabled);
    cellular_jitter_->setEnabled(enabled);
}
