#include "noisedata.h"

#include <QSize>

constexpr unsigned int kDefaultNoiseSeed = 1337;

NoiseData::NoiseData(FastNoiseLite::NoiseType noise_type)
{
    noise_ = std::make_unique<FastNoiseLite>();
    noise_->SetNoiseType(noise_type);
    noise_->SetSeed(kDefaultNoiseSeed);

    type_ = HeightDataType::kFastNoise;
}

void NoiseData::SetNoiseType(FastNoiseLite::NoiseType new_type)
{
    noise_->SetNoiseType(new_type);
}

void NoiseData::SetNoiseSeed(int new_seed)
{
    noise_->SetSeed(new_seed);
}

void NoiseData::SetFrequency(float new_freq)
{
    noise_->SetFrequency(new_freq);
}

void NoiseData::SetRotationType3D(FastNoiseLite::RotationType3D new_type)
{
    noise_->SetRotationType3D(new_type);
}

float NoiseData::GetValueAt(float x, float y) const
{
    float result = 0.0f;
    if(noise_ != nullptr)
    {
        result = noise_->GetNoise(x, y);
    }
    return result;
}
