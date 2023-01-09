#include "noiseviewwidget.h"

#include "bitmap.h"
#include "qbuffer.h"

#include <fstream>
#include <limits>
#include <math.h>

#include <QByteArray>
#include <QFile>
#include <QTextStream>

NoiseViewWidget::NoiseViewWidget(std::shared_ptr<NoiseData> data, QWidget *parent)
    : QWidget{parent}
{
    noise_data_ = data;

    QVBoxLayout* layout = new QVBoxLayout(this);

    noise_picker_ = new QComboBox();

    QStringList opts;
    opts.append("Perlin");
    opts.append("Value");
    opts.append("Open Simplex2");
    opts.append("Open Simplex2S");

    noise_picker_->addItems(opts);
    noise_picker_->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(noise_picker_, &QComboBox::currentIndexChanged,
            this, &NoiseViewWidget::OnNoiseTypeChanged_);

    layout->addWidget(noise_picker_);

    GenerateBitmap_();

    noise_preview_ = new QLabel();
    noise_preview_->setPixmap(*noise_bitmap_);
    noise_preview_->setFixedSize(128, 128);

    layout->addWidget(noise_preview_);
}

void NoiseViewWidget::UpdateNoise()
{
    GenerateBitmap_();
    noise_preview_->setPixmap(*noise_bitmap_);
}

void NoiseViewWidget::GenerateBitmap_()
{
    if(noise_bitmap_ != nullptr)
    {
        delete noise_bitmap_;
    }
    noise_bitmap_ = noise_data_->AsBitmap(0, 0, 256, 256);
}

void NoiseViewWidget::OnNoiseTypeChanged_(int new_type)
{
    NoiseSourceType type = static_cast<NoiseSourceType>(new_type);
    emit NoiseTypeChanged(ToFastNoiseEnum_(type));
}

FastNoiseLite::NoiseType NoiseViewWidget::ToFastNoiseEnum_(NoiseSourceType type) const
{
    FastNoiseLite::NoiseType result = FastNoiseLite::NoiseType_Perlin;
    switch(type)
    {
    case NoiseSourceType::kValue:
        result = FastNoiseLite::NoiseType_Value;
        break;
    case NoiseSourceType::kOpenSimplex2:
        result = FastNoiseLite::NoiseType_OpenSimplex2;
        break;
    case NoiseSourceType::kOpenSimplex2S:
        result = FastNoiseLite::NoiseType_OpenSimplex2S;
        break;
    case NoiseSourceType::kPerlin:
    default:
        break;
    }
    return result;
}
