#ifndef NOISESAMPLERDATA_H
#define NOISESAMPLERDATA_H

#include <FastNoiseLite/FastNoiseLite.h>
#include <nodes/data/heightdata.h>

#include <QtNodes/NodeData>
#include "nodes/data/noisedata.h"

class NoiseSamplerData : public HeightData {
public:
    NoiseSamplerData() : NoiseSamplerData(FastNoiseLite::NoiseType_OpenSimplex2S) {}

    NoiseSamplerData(FastNoiseLite::NoiseType noise_type);

    virtual ~NoiseSamplerData() { noise_.reset(); }

    float GetValueAt(float x, float y) const override;

    std::shared_ptr<NoiseData> GetNoiseSettings() const { return settings_; }

public:
    void NoiseSettingsChanged();

protected:
    void ApplySettings_();

    std::shared_ptr<NoiseData> settings_ = nullptr;
    std::unique_ptr<FastNoiseLite> noise_ = nullptr;
};
#endif  // NOISESAMPLERDATA_H
