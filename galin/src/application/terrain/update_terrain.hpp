#pragma once

#include "application.hpp"

void update_terrain(App& a) {
    if(a.settings.is_height_outdated) {
        a.settings.is_height_outdated = false;

        update_range(a.terrain.height);

        a.settings.is_drainage_area_outdated = true;
        a.settings.is_gradient_outdated = true;
        a.settings.is_laplacian_outdated = true;
        a.settings.is_mesh_outdated = true;
    }

    // Requires height.
    // Required by color.
    if(a.settings.is_drainage_area_outdated) {
        a.settings.is_drainage_area_outdated = false;

        if(a.settings.drainage_area_formula == DrainageAreaFormula::steepest) {
            update_drainage_area_using_steepest(a.terrain);
        } else if(a.settings.drainage_area_formula == DrainageAreaFormula::two_steepest) {
             
        } else if(a.settings.drainage_area_formula == DrainageAreaFormula::weighted) {
            update_drainage_area_using_weighted(a.terrain);
        }
        update_range(a.terrain.drainage_area);

        a.settings.is_color_outdated = true;
    }

    // Requires height.
    // Required by color.
    if(a.settings.is_laplacian_outdated) {
        a.settings.is_laplacian_outdated = false;

        update_laplacian(a.terrain);
        update_range(a.terrain.laplacian);
        
        a.settings.is_color_outdated = true;
    }

    // Requires height.
    // Required by normal, slope.
    if(a.settings.is_gradient_outdated) {
        a.settings.is_gradient_outdated = false;

        update_gradient(a.terrain);

        a.settings.is_normal_outdated = true;
        a.settings.is_slope_outdated = true;
    }

    // Requires gradient.
    // Required by color.
    if(a.settings.is_slope_outdated) {
        a.settings.is_slope_outdated = false;

        update_slope_using_mean(a.terrain);
        update_range(a.terrain.slope);

        a.settings.is_color_outdated = true;
    }

    // Requires gradient.
    // Required by color.
    if(a.settings.is_normal_outdated) {
        a.settings.is_normal_outdated = false;

        update_normals_using_gradient(a.terrain);

        a.settings.is_color_outdated = true;
    }

    // Requires drainage, laplacian, normal, shading, slope.
    if(a.settings.is_color_outdated) {
        a.settings.is_color_outdated = false;

        if(a.settings.color_formula == ColorFormula::drainage_area) {
            update_colors_using_drainage_area(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::laplacian) {
            update_colors_using_laplacian(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::normals) {
            update_colors_using_normals(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::shading) {
            update_colors_for_shading(a.terrain);
        } else if(a.settings.color_formula == ColorFormula::slope) {
            update_colors_using_slope(a.terrain);
        }

        a.settings.is_mesh_outdated = true;
    }

    if(a.settings.is_mesh_outdated) {
        a.settings.is_mesh_outdated = false;

        update_cpu(a.terrain);
        update_gpu(a.terrain);
    }
}
