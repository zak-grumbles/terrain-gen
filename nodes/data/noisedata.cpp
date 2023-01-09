#include "noisedata.h"
#include "bitmap.h"
#include "qbuffer.h"
#include "qpixmap.h"

#include <QSize>

constexpr unsigned int kDefaultNoiseSeed = 1337;

NoiseData::NoiseData(FastNoiseLite::NoiseType noise_type)
{
    noise_ = std::make_unique<FastNoiseLite>();
    noise_->SetNoiseType(noise_type);
    noise_->SetSeed(kDefaultNoiseSeed);
}

QtNodes::NodeDataType NoiseData::type() const
{
    return QtNodes::NodeDataType {
        "noise", "Noise"
    };
}

void NoiseData::SetNoiseType(FastNoiseLite::NoiseType new_type)
{
    noise_->SetNoiseType(new_type);
}

void NoiseData::SetNoiseSeed(int new_seed)
{
    noise_->SetSeed(new_seed);
}

float NoiseData::GetNoise(float x, float y)
{
    float result = 0.0f;
    if(noise_ != nullptr)
    {
        result = noise_->GetNoise(x, y);
    }
    return result;
}

QPixmap* NoiseData::AsBitmap(float x_offset, float y_offset,
                float width, float height)
{
    return AsBitmap(QSize(x_offset, y_offset),
                    QSize(width, height));
}

QPixmap* NoiseData::AsBitmap(QSize offset, QSize size)
{
    float min_val = std::numeric_limits<float>::max();
    float max_val = std::numeric_limits<float>::min();

    float* noise_values = new float[size.width() * size.height()];
    int index = 0;
    for(int y = offset.height(); y < offset.height() + size.height(); y++)
    {
        for(int x = offset.width(); x < offset.width() + size.width(); x++)
        {
            float fx = x;
            float fy = y;
            float value = noise_->GetNoise(fx, fy);

            max_val = std::max(max_val, value);
            min_val = std::min(min_val, value);

            noise_values[index++] = value;
        }
    }

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
    bmp->GivePixelData(image_data);

    QByteArray bmp_bytes;
    QBuffer buf(&bmp_bytes);
    buf.open(QIODevice::WriteOnly);

    QDataStream bmp_stream(&buf);
    bmp_stream << *bmp;

    buf.close();

    QPixmap* image = new QPixmap(size);
    image->loadFromData(bmp_bytes);

    delete[] noise_values;

    return image;
}
