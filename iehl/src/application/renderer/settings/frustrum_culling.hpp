#pragma once

struct FrustrumCullingSettings {
    bool enabled = true;

    // Debbugging.

    bool draw_accepted_bounding_boxes = true;
    bool draw_rejected_bounding_boxes = true;
    bool draw_rejected_objects = false;
};
