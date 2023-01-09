#ifndef NOISEDATA_H
#define NOISEDATA_H

#include <FastNoiseLite/FastNoiseLite.h>
#include <QtNodes/NodeData>

class NoiseData : public QtNodes::NodeData
{
public:
    NoiseData() : NoiseData(FastNoiseLite::NoiseType_OpenSimplex2S) {}

    NoiseData(FastNoiseLite::NoiseType noise_type);

    virtual ~NoiseData() { noise_.reset(); }

    QtNodes::NodeDataType type() const override;

    void SetNoiseType(FastNoiseLite::NoiseType new_type);
    void SetNoiseSeed(int new_seed);

    float GetNoise(float x, float y);

    QPixmap* AsBitmap(float x_offset, float y_offset,
                    float width, float height);
    QPixmap* AsBitmap(QSize offset, QSize size);

protected:
    std::unique_ptr<FastNoiseLite> noise_ = nullptr;
};
#endif // NOISEDATA_H
