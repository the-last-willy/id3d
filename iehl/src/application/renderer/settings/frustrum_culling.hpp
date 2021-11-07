#pragma once

struct FrustrumCullingSettings {
    bool enabled = true;

    // Debbugging.

    bool draw_rejected_bounding_boxes = false;
    bool draw_rejected_objects = false;
};
