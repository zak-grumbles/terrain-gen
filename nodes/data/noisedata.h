#ifndef NOISEDATA_H
#define NOISEDATA_H

#include <FastNoiseLite/FastNoiseLite.h>
#include <nodes/data/heightdata.h>

#include <QtNodes/NodeData>

class NoiseData : public HeightData {
public:
    NoiseData() : NoiseData(FastNoiseLite::NoiseType_OpenSimplex2S) {}

    NoiseData(FastNoiseLite::NoiseType noise_type);

    virtual ~NoiseData() { noise_.reset(); }

    void SetNoiseType(FastNoiseLite::NoiseType new_type);

    void SetNoiseSeed(int new_seed);
    int GetNoiseSeed() const { return noise_->GetSeed(); }

    void SetFrequency(float new_freq);
    float GetFrequency() const { return noise_->GetFrequency(); }

    void SetRotationType3D(FastNoiseLite::RotationType3D new_type);
    FastNoiseLite::RotationType3D GetRotationType3D() const {
        return noise_->GetRotationType3D();
    }

    float GetValueAt(float x, float y) const override;

protected:
    std::unique_ptr<FastNoiseLite> noise_ = nullptr;
};
#endif  // NOISEDATA_H
