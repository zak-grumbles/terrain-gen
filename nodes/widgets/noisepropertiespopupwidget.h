#ifndef NOISEPROPERTIESPOPUPWIDGET_H
#define NOISEPROPERTIESPOPUPWIDGET_H

#include "nodes/data/noisedata.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include <QWidget>

Q_DECLARE_METATYPE(FastNoiseLite::RotationType3D);

class NoisePropertiesPopupWidget : public QWidget
{
    Q_OBJECT

public:
    NoisePropertiesPopupWidget(std::shared_ptr<NoiseData> noise, QWidget* parent = nullptr);
    ~NoisePropertiesPopupWidget();

signals:
    void SeedChanged(int new_seed);
    void FrequencyChanged(float new_freq);
    void RotationType3DChanged(FastNoiseLite::RotationType3D new_type);

protected slots:
    void OnSeedSpinBoxChange(int new_seed);
    void OnFreqSpinBoxChange(double new_freq);
    void OnRotationTypeChange(int new_index);

private:
    QGroupBox* CreateGeneralSettings_(std::shared_ptr<NoiseData> noise);

    QComboBox* rotation_type_ = nullptr;
};

#endif // NOISEPROPERTIESPOPUPWIDGET_H
