#ifndef NOISETYPESELECTORWIDGET_H
#define NOISETYPESELECTORWIDGET_H

#include <QObject>
#include <QWidget>

#include "FastNoiseLite/FastNoiseLite.h"
#include "qboxlayout.h"
#include "qcombobox.h"
#include "qpushbutton.h"

enum NoiseSelectorType { kPerlin = 0, kValue, kOpenSimplex2, kOpenSimplex2S };

class NoiseTypeSelectorWidget : public QWidget {
    Q_OBJECT

public:
    explicit NoiseTypeSelectorWidget(QWidget* parent = nullptr);

signals:
    void NoiseTypeChanged(FastNoiseLite::NoiseType new_type);
    void OpenPropertiesWindow();

protected slots:
    void OnSelectorChanged_(int new_type);
    void OnPropertiesBtnClicked_(bool checked = false);

protected:
    FastNoiseLite::NoiseType ToFastNoiseEnum_(NoiseSelectorType type) const;

    QHBoxLayout* layout_         = nullptr;
    QComboBox* noise_picker_     = nullptr;
    QPushButton* properties_btn_ = nullptr;
};

#endif  // NOISETYPESELECTORWIDGET_H
