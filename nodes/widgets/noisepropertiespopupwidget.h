#ifndef NOISEPROPERTIESPOPUPWIDGET_H
#define NOISEPROPERTIESPOPUPWIDGET_H

#include <QWidget>

#include "nodes/data/noisedata.h"
#include "qcombobox.h"
#include "qgroupbox.h"

Q_DECLARE_METATYPE(FastNoiseLite::RotationType3D);
Q_DECLARE_METATYPE(FastNoiseLite::FractalType);

class NoisePropertiesPopupWidget : public QWidget {
    Q_OBJECT

public:
    NoisePropertiesPopupWidget(
        std::shared_ptr<NoiseData> noise, QWidget* parent = nullptr
    );
    ~NoisePropertiesPopupWidget();

signals:
    void SeedChanged(int new_seed);
    void FrequencyChanged(float new_freq);
    void RotationType3DChanged(FastNoiseLite::RotationType3D new_type);

    void FractalTypeChanged(FastNoiseLite::FractalType new_type);

protected slots:
    void OnSeedSpinBoxChange_(int new_seed);
    void OnFreqSpinBoxChange_(double new_freq);
    void OnRotationTypeChange_(int new_index);

    void OnFractalTypeChange_(int new_index);

private:
    QGroupBox* CreateGeneralSettings_(std::shared_ptr<NoiseData> noise);
    QComboBox* rotation_type_ = nullptr;

    QGroupBox* CreateFractalSettings_(std::shared_ptr<NoiseData> noise);
    QComboBox* fractal_type_ = nullptr;
};

#endif  // NOISEPROPERTIESPOPUPWIDGET_H
