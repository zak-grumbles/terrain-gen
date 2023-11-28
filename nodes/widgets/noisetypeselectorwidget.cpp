#include "noisetypeselectorwidget.h"

#include "qboxlayout.h"

NoiseTypeSelectorWidget::NoiseTypeSelectorWidget(QWidget* parent /*= nullptr*/)
    : QWidget{parent} {
    properties_btn_ = new QPushButton();
    properties_btn_->setText("...");

    connect(
        properties_btn_, &QPushButton::clicked, this,
        &NoiseTypeSelectorWidget::OnPropertiesBtnClicked_
    );

    layout_ = new QHBoxLayout(this);
    layout_->addWidget(properties_btn_);

    this->setAttribute(Qt::WA_TranslucentBackground);
}

void NoiseTypeSelectorWidget::UpdateButtonText(std::string new_text) {
    properties_btn_->setText(QString::fromStdString(new_text));
}

void NoiseTypeSelectorWidget::OnPropertiesBtnClicked_(bool
                                                          checked /*= false*/) {
    emit OpenPropertiesWindow();
}
