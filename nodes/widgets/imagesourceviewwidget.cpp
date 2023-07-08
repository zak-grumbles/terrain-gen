#include "imagesourceviewwidget.h"

#include <QFileDialog>
#include <QVboxLayout>

ImageSourceViewWidget::ImageSourceViewWidget(QWidget* parent /*= nullptr*/)
    : QWidget{parent}
{
    select_img_btn_ = new QPushButton("Select Image", this);
    connect(select_img_btn_, &QPushButton::clicked,
            this, &ImageSourceViewWidget::SelectImage);

    img_view_ = new QLabel();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(select_img_btn_);
    layout->addWidget(img_view_);
    img_view_->hide();
}

void ImageSourceViewWidget::SelectImage()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Import heightmap"), "", tr("Image (*.bmp,*.png);;All Files(*)"));

    if(!filename.isEmpty())
    {
        std::shared_ptr<QPixmap> img = std::make_shared<QPixmap>();
        img->load(filename);

        img_view_->setPixmap(*img);
        img_view_->setFixedSize(128, 128);

        img_view_->show();
        select_img_btn_->hide();

        emit ImageSourceSelected(img);
    }
}
