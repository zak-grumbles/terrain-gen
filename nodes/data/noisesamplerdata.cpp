#include "noisesamplerdata.h"

#include <QSize>

constexpr unsigned int kDefaultNoiseSeed = 1337;

NoiseSamplerData::NoiseSamplerData(FastNoiseLite::NoiseType noise_type) {
    type_ = HeightDataType::kFastNoise;

    settings_ = std::make_shared<NoiseData>();

    settings_->noise_type = noise_type;
    settings_->seed       = kDefaultNoiseSeed;

    noise_ = std::make_unique<FastNoiseLite>();
    ApplySettings_();
}

float NoiseSamplerData::GetValueAt(float x, float y) const {
    float result = 0.0f;
    if (noise_ != nullptr) {
        result = noise_->GetNoise(x, y);
    }
    return result;
}

void NoiseSamplerData::NoiseSettingsChanged() {
    ApplySettings_();
}

void NoiseSamplerData::ApplySettings_() {
    noise_->SetNoiseType(settings_->noise_type);
    noise_->SetRotationType3D(settings_->rotation_type);
    noise_->SetSeed(settings_->seed);
    noise_->SetFrequency(settings_->frequency);

    noise_->SetFractalType(settings_->fractal_type);
    noise_->SetFractalOctaves(settings_->octaves);
    noise_->SetFractalGain(settings_->gain);
    noise_->SetFractalWeightedStrength(settings_->weighted_strength);
    noise_->SetFractalPingPongStrength(settings_->pingpong_strength);
}
