#pragma once

#include "ruppert/settings.hpp"

enum class ViewMode {
    _2D, _3D
};

struct Settings {
    bool show_help = false;
    bool show_settings = true;

    ViewMode view_mode = ViewMode::_2D;

    RuppertSettings ruppert_settings;

    bool delaunay_insertion = false;
    bool lawson_insertion = false;
    bool ruppert_insertion = false;

    bool show_edges = true;
    bool show_triangle_circumcircles = false;
    bool show_triangle_connectivity = false;
    bool show_voronoi = false;
};
