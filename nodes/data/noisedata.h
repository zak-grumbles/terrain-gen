#ifndef NOISEDATA_H
#define NOISEDATA_H

#include <FastNoiseLite/FastNoiseLite.h>

struct NoiseData {
    // General Settings

    FastNoiseLite::NoiseType noise_type =
        FastNoiseLite::NoiseType_OpenSimplex2S;
    FastNoiseLite::RotationType3D rotation_type =
        FastNoiseLite::RotationType3D_None;
    int seed        = 1337;
    float frequency = 0.010f;

    // Fractal Settings

    FastNoiseLite::FractalType fractal_type = FastNoiseLite::FractalType_None;
    int octaves                             = 1;
    float lacunarity                        = 2.0f;
    float gain                              = 0.5f;
    float weighted_strength                 = 0.0f;
    float pingpong_strength                 = 2.0f;

    // Cellular Settings

    FastNoiseLite::CellularDistanceFunction cellular_distance_func =
        FastNoiseLite::CellularDistanceFunction_EuclideanSq;
    FastNoiseLite::CellularReturnType cellular_return_type =
        FastNoiseLite::CellularReturnType_Distance;
    float jitter = 1.0f;

    // Domain Warp Settings

    FastNoiseLite::DomainWarpType domain_warp_type =
        FastNoiseLite::DomainWarpType_BasicGrid;
    FastNoiseLite::RotationType3D domain_rotation_type =
        FastNoiseLite::RotationType3D_None;
    float domain_warp_amplitude = 1.0f;
    int domain_warp_seed        = 1337;
    float domain_warp_frequency = 0.01f;

    // Domain Warp Fractal Settings

    FastNoiseLite::FractalType domain_warp_fractal_type =
        FastNoiseLite::FractalType_None;
    int domain_warp_octaves      = 1;
    float domain_warp_lacunarity = 2.0f;
    float domain_warp_gain       = 0.5f;
};

#endif  // NOISEDATA_H
