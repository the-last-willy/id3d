#pragma once

enum class FrustumCullingMode {
    disabled,
    cpu,
    gpu,
};

struct FrustumCullingSettings {
    bool is_enabled = true;

    // Debugging.

    bool is_anchored = false;
};

struct OcclusionCullingSettings {
    bool is_enabled = true;

    // Debugging.

    bool is_anchored = false;

    
};

struct RasterizationSettings {
    bool enabled = true;

    
};

struct Settings {
    FrustumCullingSettings frustum_culling;
    OcclusionCullingSettings occlusion_culling;
};
