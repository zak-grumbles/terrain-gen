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

    setLayout(layout);
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

void NoisePropertiesPopupWidget::OnRotationTypeChange(int new_index)
{
    if(new_index != -1) {
        FastNoiseLite::RotationType3D new_type = rotation_type_
            ->itemData(new_index).value<FastNoiseLite::RotationType3D>();
        emit RotationType3DChanged(new_type);
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
    general_settings->setLayout(layout);

    layout->addWidget(new QLabel(tr("Rotation Type 3D")), 2, 0);
    layout->addWidget(rotation_type_, 2, 1);

    connect(
        seed, &QSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnSeedSpinBoxChange
    );
    connect(
        frequency, &QDoubleSpinBox::valueChanged,
        this, &NoisePropertiesPopupWidget::OnFreqSpinBoxChange
    );
    connect(
        rotation_type_, &QComboBox::currentIndexChanged,
        this, &NoisePropertiesPopupWidget::OnRotationTypeChange
    );

    return general_settings;
}
