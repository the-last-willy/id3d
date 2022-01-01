#pragma once

struct MaterialProperties {
    std::array<float, 4> color_factor;
    std::array<float, 4> emission_factor;
};

inline
bool is_emissive(const MaterialProperties& mps) {
    return mps.emission_factor[0] > 0.f
    or mps.emission_factor[1] > 0.f
    or mps.emission_factor[2] > 0.f;
}
