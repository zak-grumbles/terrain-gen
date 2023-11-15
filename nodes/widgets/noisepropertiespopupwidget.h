#ifndef NOISEPROPERTIESPOPUPWIDGET_H
#define NOISEPROPERTIESPOPUPWIDGET_H

#include "qspinbox.h"
#include <QWidget>

class NoisePropertiesPopupWidget : public QWidget
{
    Q_OBJECT

public:
    NoisePropertiesPopupWidget(QWidget* parent = nullptr);
    ~NoisePropertiesPopupWidget();

private:
    QSpinBox* seed_;
    QSpinBox* frequency_;
};

#endif // NOISEPROPERTIESPOPUPWIDGET_H
