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

struct Statistics {
    FrustumCullingStatistics frustum_culling;
    OcclusionCullingStatistics occlusion_culling;
};
