#ifndef NOISETYPESELECTORWIDGET_H
#define NOISETYPESELECTORWIDGET_H

#include <QObject>
#include <QWidget>

#include "FastNoiseLite/FastNoiseLite.h"
#include "qcombobox.h"

enum NoiseSelectorType
{
    kPerlin = 0,
    kValue,
    kOpenSimplex2,
    kOpenSimplex2S
};

class NoiseTypeSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoiseTypeSelectorWidget(QWidget* parent = nullptr);

signals:
    void NoiseTypeChanged(FastNoiseLite::NoiseType new_type);

protected slots:
    void OnSelectorChanged_(int new_type);

protected:
    FastNoiseLite::NoiseType ToFastNoiseEnum_(NoiseSelectorType type) const;

    QComboBox* noise_picker_ = nullptr;
};

#endif // NOISETYPESELECTORWIDGET_H
