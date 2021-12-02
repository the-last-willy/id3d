#pragma once

enum class ColorFormula {
    drainage_area,
    laplacian,
    normals,
    shading,
    slope,
};

enum class DrainageAreaFormula {
    steepest,
    two_steepest,
    weighted,
};

enum class NormalFormula {
    gradient,
};

enum class SlopeFormula {
    gradient,
};

struct Settings {
    bool show_heights = true;

    bool is_mesh_outdated = true;

    bool is_color_outdated = true;
    ColorFormula color_formula = ColorFormula::shading;

    // Camera.
    float camera_height_scaling = 1.f;
    float camera_depth_scaling = 1.f;

    // Fields.

    bool is_drainage_area_outdated = true;
    DrainageAreaFormula drainage_area_formula = DrainageAreaFormula::steepest;

    bool is_gradient_outdated = true;

    bool is_height_outdated = true;

    bool is_laplacian_outdated = true;

    bool is_normal_outdated = true;
    NormalFormula normal_formula = NormalFormula::gradient;

    bool is_slope_outdated = true;
    SlopeFormula slope_formula = SlopeFormula::gradient;

    // Erosion.
    float debris_slope_erosion_factor = 1.f;
    float hill_slope_erosion_factor = 1.f;
    float stream_power_erosion_factor = 1.f;
};
