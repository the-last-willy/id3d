#pragma once

#include <array>

struct FFDSettings {
    std::array<int, 3> resolution = {2, 2, 2};
};

struct Settings {
    bool show_settings = false;

    int tesselation_resolution = 2;

    float transform_intensity = 0.f;
    agl::Vec3 transform_position;
    float transform_radius = 1.f;

    bool show_bounding_box = false;
    bool show_control_points = false;
    bool show_gizmo = true;
    bool show_triangulation = true;

    bool ffd_enabled = false;
    bool ffd_show_control_grid = true;
    FFDSettings ffd;
};
