#ifndef NOISEPROPERTIESPOPUPWIDGET_H
#define NOISEPROPERTIESPOPUPWIDGET_H

#include <QWidget>

#include "nodes/data/noisedata.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include "qspinbox.h"

Q_DECLARE_METATYPE(FastNoiseLite::NoiseType)
Q_DECLARE_METATYPE(FastNoiseLite::RotationType3D)
Q_DECLARE_METATYPE(FastNoiseLite::FractalType)
Q_DECLARE_METATYPE(FastNoiseLite::CellularDistanceFunction)
Q_DECLARE_METATYPE(FastNoiseLite::CellularReturnType)

class NoisePropertiesPopupWidget : public QWidget {
    Q_OBJECT

public:
    NoisePropertiesPopupWidget(
        std::shared_ptr<NoiseData> noise, QWidget* parent = nullptr
    );
    ~NoisePropertiesPopupWidget();

signals:
    void NoiseSettingsChanged();

protected slots:
    void OnNoiseTypeChange_(int new_index);
    void OnSeedSpinBoxChange_(int new_seed);
    void OnFreqSpinBoxChange_(double new_freq);
    void OnRotationTypeChange_(int new_index);

    void OnFractalTypeChange_(int new_index);
    void OnOctavesChange_(int new_value);
    void OnLacunarityChange_(double new_value);
    void OnGainChange_(double new_value);
    void OnWeightedStrengthChange_(double new_value);
    void OnPingPongStrengthChange_(double new_value);

    void OnCellularDistanceFuncChange_(int new_index);
    void OnCellularReturnTypeChange_(int new_index);
    void OnCellularJitterChange_(double new_value);

private:
    std::shared_ptr<NoiseData> noise_settings_ = nullptr;

    QGroupBox* CreateGeneralSettings_();
    QComboBox* CreateNoiseTypeComboBox_();
    QComboBox* noise_type_    = nullptr;
    QComboBox* rotation_type_ = nullptr;

    QGroupBox* CreateFractalSettings_();
    QComboBox* CreateFractalTypeComboBox_();
    void SetFractalSettingsEnabled_(bool enabled);
    QComboBox* fractal_type_              = nullptr;
    QSpinBox* fractal_octaves_            = nullptr;
    QDoubleSpinBox* fractal_lacunarity_   = nullptr;
    QDoubleSpinBox* fractal_gain_         = nullptr;
    QDoubleSpinBox* fractal_weighted_str_ = nullptr;
    QDoubleSpinBox* fractal_pingpong_str_ = nullptr;

    QGroupBox* CreateCellularSettings_();
    QComboBox* CreateCellularDistanceFuncComboBox_();
    QComboBox* CreateCellularReturnTypeComboBox_();
    QComboBox* cellular_distance_func_ = nullptr;
    QComboBox* cellular_return_type_   = nullptr;
    QDoubleSpinBox* cellular_jitter_   = nullptr;
};

#endif  // NOISEPROPERTIESPOPUPWIDGET_H
