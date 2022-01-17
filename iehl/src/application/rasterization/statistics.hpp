#pragma once

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

struct FrustumCullingStatistics {
    std::size_t accepted_count = 0;
    std::size_t rejected_count = 0;
};

inline
auto accepted_ratio(const FrustumCullingStatistics& fcs) {
    auto total = fcs.accepted_count + fcs.rejected_count;
    return float(fcs.accepted_count) / float(total);
}

inline
auto rejected_ratio(const FrustumCullingStatistics& fcs) {
    auto total = fcs.accepted_count + fcs.rejected_count;
    return float(fcs.rejected_count) / float(total);
}

////////////////////////////////////////////////////////////////////////////////

struct OcclusionCullingStatistics {
    std::size_t accepted_count = 0;
    std::size_t rejected_count = 0;
};

inline
auto accepted_ratio(const OcclusionCullingStatistics& ocs) {
    auto total = ocs.accepted_count + ocs.rejected_count;
    return float(ocs.accepted_count) / float(total);
}

inline
auto rejected_ratio(const OcclusionCullingStatistics& ocs) {
    auto total = ocs.accepted_count + ocs.rejected_count;
    return float(ocs.rejected_count) / float(total);
}

////////////////////////////////////////////////////////////////////////////////

struct RasterizationStatistics {
    unsigned object_draw_count = 0;

    float cpu_time = 0.f;
    float gpu_time = 0.f;
};

////////////////////////////////////////////////////////////////////////////////

struct RayTracerStatistics {
    std::size_t point_count = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Statistics {
    FrustumCullingStatistics frustum_culling;
    OcclusionCullingStatistics occlusion_culling;
    RasterizationStatistics rasterization;
    RayTracerStatistics ray_tracer;
};
