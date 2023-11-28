#ifndef NOISETYPESELECTORWIDGET_H
#define NOISETYPESELECTORWIDGET_H

#include <QObject>
#include <QWidget>

#include "qboxlayout.h"
#include "qpushbutton.h"

class NoiseTypeSelectorWidget : public QWidget {
    Q_OBJECT

public:
    explicit NoiseTypeSelectorWidget(QWidget* parent = nullptr);

    void UpdateButtonText(std::string new_text);

signals:
    void OpenPropertiesWindow();

protected slots:
    void OnPropertiesBtnClicked_(bool checked = false);

protected:
    QHBoxLayout* layout_         = nullptr;
    QPushButton* properties_btn_ = nullptr;
};

#endif  // NOISETYPESELECTORWIDGET_H
