#pragma once

#include <cstdlib>

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

struct Statistics {
    FrustumCullingStatistics frustum_culling;
};
