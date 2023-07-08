#ifndef IMAGESOURCEVIEWWIDGET_H
#define IMAGESOURCEVIEWWIDGET_H

#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class ImageSourceViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSourceViewWidget(QWidget* parent = nullptr);

signals:
    void ImageSourceSelected(std::shared_ptr<QPixmap> img);

protected:
    void SelectImage();

    QLabel* img_view_ = nullptr;
    QPushButton* select_img_btn_ = nullptr;
};

#endif // IMAGESOURCEVIEWWIDGET_H
