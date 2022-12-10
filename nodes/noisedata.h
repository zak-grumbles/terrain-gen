#ifndef NOISEDATA_H
#define NOISEDATA_H

#include <FastNoiseLite/FastNoiseLite.h>
#include <QtNodes/NodeData>

class NoiseData : public QtNodes::NodeData
{
public:
    NoiseData() : NoiseData(FastNoiseLite::NoiseType_Perlin) {}

    NoiseData(FastNoiseLite::NoiseType noise_type);

    virtual ~NoiseData() { noise_.reset(); }

    QtNodes::NodeDataType type() const override;

protected:
    std::unique_ptr<FastNoiseLite> noise_ = nullptr;
};
#endif // NOISEDATA_H
