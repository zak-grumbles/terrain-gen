#include "noiseviewwidget.h"

#include <math.h>
#include <limits>

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

    layout->addWidget(noise_preview_);
}

void NoiseViewWidget::UpdateNoise()
{
    GenerateBitmap_();
    noise_preview_->setPixmap(*noise_bitmap_);
}

void NoiseViewWidget::GenerateBitmap_()
{
    QSize size(125, 125);

    float min_val = std::numeric_limits<float>::max();
    float max_val = std::numeric_limits<float>::min();

    float* noise_values = new float[size.width() * size.height()];
    int index = 0;
    for(int y = size.height() / -2; y < size.height() / 2; y++)
    {
        for(int x = size.width() / -2; x < size.width() / 2; x++)
        {
            float fx = x;
            float fy = y;
            float value = noise_data_->GetNoise(fx, fy);

            max_val = std::max(max_val, value);
            min_val = std::min(min_val, value);

            noise_values[index++] = value;
        }
    }

    uchar* image_data = new uchar[size.width() * size.height()];
    float scale = 255 / (max_val - min_val);
    for(int i = 0; i < size.width() * size.height(); i++)
    {
        float value = (noise_values[i] - min_val) * scale;

        float clamped_val = std::clamp(value, 0.0f, 255.0f);
        int rounded_val = std::round(clamped_val);

        image_data[i] = rounded_val;
    }

    /*
    if(noise_bitmap_ != nullptr)
    {
        delete noise_bitmap_;
    }
    noise_bitmap_ = new QPixmap(size);

    noise_bitmap_->loadFromData(image_data, size.width() * size.height(), "BMP");
    */

    // 54 here is the size of a bitmap file header plus the DIB header
    int file_size = size.width() * size.height() + 54;

    QByteArray data;
    data.reserve(file_size);

    // bitmap file header
    data.push_back(QByteArray::fromStdString("BM")); 	// data format
    data.push_back(QByteArray::number(file_size));		// file size
    data.push_back(QByteArray::number((quint16)0));		// gap
    data.push_back(QByteArray::number((quint16)0));		// gap
    data.push_back(QByteArray::number(54));				// Offset of pixel array

    // dib header
    data.push_back(QByteArray::number(40));			 	// size of DIB header
    data.push_back(QByteArray::number(size.width()));	// width of image
    data.push_back(QByteArray::number(size.height()));	// height of image
    data.push_back(QByteArray::number((quint16)1));		// number of color planes
    data.push_back(QByteArray::number((quint16)32));	// number of bits per pixel
    data.push_back(QByteArray::number(0));				// compression method (0 == BI_RGB)
    data.push_back(QByteArray::number(0));				// size of pixel data. Can use 0 since BI_RGB
    data.push_back(QByteArray::number(size.width()));
    data.push_back(QByteArray::number(size.height()));
    data.push_back(QByteArray::number(0));				// number of colors in pallete (0 defaults to 2^n)
    data.push_back(QByteArray::number(0));				// usually ignored

    data.push_back(image_data);
    QFile file("TEST.bmp");
    if(file.open(QIODevice::ReadWrite))
    {
        file.write(data);
    }

    if(noise_bitmap_ != nullptr)
        delete noise_bitmap_;

    noise_bitmap_ = new QPixmap(size);

    delete[] image_data;
    delete[] noise_values;
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
