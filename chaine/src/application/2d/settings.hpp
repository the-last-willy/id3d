#pragma once

enum class ViewMode {
    _2D, _3D
};

struct Settings {
    bool show_help = false;
    bool show_settings = true;

    ViewMode view_mode = ViewMode::_2D;

    bool delaunay_insertion = false;

    bool show_voronoi = false;
};
