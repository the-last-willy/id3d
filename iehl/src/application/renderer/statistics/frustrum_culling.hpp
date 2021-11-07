#pragma once

struct FrustrumCullingStatistics {
    int accepted_count = 0;
    int rejected_count = 0;
};

inline
int total_count(const FrustrumCullingStatistics& fcs) {
    return fcs.accepted_count + fcs.rejected_count;
}

inline
float accepted_percentage(const FrustrumCullingStatistics& fcs) {
    return float(100 * fcs.accepted_count) / float(total_count(fcs));
}

inline
float rejected_percentage(const FrustrumCullingStatistics& fcs) {
    return float(100 * fcs.rejected_count) / float(total_count(fcs));
}
