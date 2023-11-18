#ifndef NOISEPROPERTIESPOPUPWIDGET_H
#define NOISEPROPERTIESPOPUPWIDGET_H

#include "nodes/data/noisedata.h"
#include "qboxlayout.h"
#include "qspinbox.h"
#include <QWidget>

class NoisePropertiesPopupWidget : public QWidget
{
    Q_OBJECT

public:
    NoisePropertiesPopupWidget(std::shared_ptr<NoiseData> noise, QWidget* parent = nullptr);
    ~NoisePropertiesPopupWidget();

signals:
    void SeedChanged(int new_seed);
    void FrequencyChanged(float new_freq);

protected slots:
    void OnSeedSpinBoxChange(int new_seed);
    void OnFreqSpinBoxChange(double new_freq);

private:
    QVBoxLayout* layout_;
    QSpinBox* seed_;
    QDoubleSpinBox* frequency_;
};

#endif // NOISEPROPERTIESPOPUPWIDGET_H
