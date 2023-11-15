#ifndef NOISEDATA_H
#define NOISEDATA_H

#include <FastNoiseLite/FastNoiseLite.h>
#include <QtNodes/NodeData>
#include <nodes/data/heightdata.h>

class NoiseData : public HeightData
{
public:
    NoiseData() : NoiseData(FastNoiseLite::NoiseType_OpenSimplex2S) {}

    NoiseData(FastNoiseLite::NoiseType noise_type);

    virtual ~NoiseData() { noise_.reset(); }

    void SetNoiseType(FastNoiseLite::NoiseType new_type);
    void SetNoiseSeed(int new_seed);

    float GetValueAt(float x, float y) const override;

protected:
    std::unique_ptr<FastNoiseLite> noise_ = nullptr;
};
#endif // NOISEDATA_H
