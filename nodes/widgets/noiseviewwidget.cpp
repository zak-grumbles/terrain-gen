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
    QSize size(256, 256);

    float min_val = std::numeric_limits<float>::max();
    float max_val = std::numeric_limits<float>::min();

    float* noise_values = new float[size.width() * size.height()];
    int index = 0;
    for(int y = 0; y < size.height(); y++)
    {
        for(int x = 0; x < size.width(); x++)
        {
            float fx = x;
            float fy = y;
            float value = noise_data_->GetNoise(fx, fy);

            max_val = std::max(max_val, value);
            min_val = std::min(min_val, value);

            noise_values[index++] = value;
        }
    }

    //uchar* image_data = new uchar[size.width() * size.height()];
    std::vector<BitmapPixel> image_data;
    image_data.reserve(size.width() * size.height());

    float scale = 255 / (max_val - min_val);
    for(int i = 0; i < size.width() * size.height(); i++)
    {
        float value = (noise_values[i] - min_val) * scale;

        float clamped_val = std::clamp(value, 0.0f, 255.0f);
        int rounded_val = std::round(clamped_val);

        //image_data[i] = rounded_val;
        BitmapPixel pxl;
        pxl.blue = rounded_val;
        pxl.green = rounded_val;
        pxl.red = rounded_val;

        image_data.push_back(pxl);
    }

    auto bmp = new Bitmap(size.width(), size.height());
    //bmp->GivePixelData(size.width() * size.height(), image_data);
    bmp->GivePixelData(image_data);

    std::ofstream test("TEST.bmp", std::ios::out | std::ios::binary);
    auto data = bmp->Data();
    if(test)
    {
        test << *bmp;
        test.flush();
        test.close();
    }

    QByteArray bmp_bytes;
    QBuffer buf(&bmp_bytes);
    buf.open(QIODevice::WriteOnly);

    QDataStream bmp_stream(&buf);
    bmp_stream << *bmp;

    buf.close();

    if(noise_bitmap_ != nullptr)
        delete noise_bitmap_;

    noise_bitmap_ = new QPixmap(size);
    noise_bitmap_->loadFromData(bmp_bytes);

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
