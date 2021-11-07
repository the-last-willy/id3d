#pragma once

#include <cstdint>
#include <cstdlib>


struct RayTracingSettings {
    bool enabled = false;
    float range = 1.f;
    uint32_t ray_per_frame = 0;
};

struct SceneStatistics {
    std::size_t object_count = 0;
};

struct TimeStatistics {
    float framerate = 0.f;

    float cpu_time = 0.f;
    float gpu_time = 0.f;
};
