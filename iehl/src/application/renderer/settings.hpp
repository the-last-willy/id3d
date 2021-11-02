#pragma once

#include <cstdint>

struct RayTracingSettings {
    bool enabled = false;
    float range = 1.f;
    uint32_t ray_per_frame = 0;
};


