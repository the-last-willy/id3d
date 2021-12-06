#pragma once

enum class ColorFormula {
    drainage_area,
    height,
    laplacian,
    normals,
    shading,
    slope,
    vegetation_probability,
    wetness,
};

enum class DrainageAreaFormula {
    steepest,
    weighted,
};

enum class NormalFormula {
    gradient,
};

enum class SlopeFormula {
    gradient,
    weighted,
};

struct Settings {
    bool show_heights = true;

    bool is_mesh_outdated = true;

    bool is_color_outdated = true;
    ColorFormula color_formula = ColorFormula::vegetation_probability;

    // View.
    float view_height_scaling = 1.f;
    float view_scaling = 1.f;

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

    bool is_vegetation_probability_outdated = true;

    bool is_wetness_outdated = true;

    // Erosion.
    float debris_slope_erosion_extent = 1.f;
    float hill_slope_erosion_extent = 1.f;
    float stream_power_erosion_extent = 1.f;
};
