#include "noisedata.h"

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
