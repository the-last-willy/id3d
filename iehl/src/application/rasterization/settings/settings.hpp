#pragma once

enum class FrustumCullingMode {
    disabled,
    cpu,
    gpu,
};

struct FrustumCullingSettings {
    FrustumCullingMode mode = FrustumCullingMode::disabled;

    // Debugging.

    bool is_anchored = false;
};

struct RasterizationSettings {
    bool enabled = true;

    FrustumCullingSettings frustum_culling;
};

struct Settings {
    bool rasterization_enabled = true;  

    RasterizationSettings rasterization;

    bool bvh_debugging_enabled = false;
    int bvh_debugging_level = 0;

    bool object_grid_debugging_enabled = false;
};
