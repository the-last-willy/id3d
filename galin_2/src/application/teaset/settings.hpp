#pragma once

#include "modelling/attenuation.hpp"

#include <array>

enum class TransformAttenuation {
    none,
    linear,
    cubic
};

inline
auto attenuation_function(TransformAttenuation ta) {
    if(ta == TransformAttenuation::none) {
        return &no_attenuation;
    } else if(ta == TransformAttenuation::linear) {
        return &linear_attenuation;
    } else if(ta == TransformAttenuation::cubic) {
        return &cubic_attenuation;
    } else {
        throw std::logic_error("Invalid attenuation function.");
    }
}

enum class TransformType {
    none,
    scaling,
    twist,
};

// inline
// auto transformation_function(TransformAttenuation ta) {
//     if(ta == TransformAttenuation::none) {
//         return &no_attenuation;
//     } else if(ta == TransformAttenuation::linear) {
//         return &linear_attenuation;
//     } else if(ta == TransformAttenuation::cubic) {
//         return &cubic_attenuation;
//     } else {
//         // Throw ?
//     }
// }

struct TransformSettings {

};

struct FFDSettings {
    std::array<int, 3> resolution = {2, 2, 2};
};

struct Settings {
    bool show_settings = false;

    int tesselation_resolution = 2;

    bool transform_enabled = false;
    float transform_intensity = 0.f;
    agl::Vec3 transform_position;
    float transform_radius = 1.f;
    int transform_attenuation = int(TransformAttenuation::cubic);
    int transform_type = int(TransformType::scaling);

    bool show_bounding_box = false;
    bool show_control_points = false;
    bool show_gizmo = false;
    bool show_triangulation = true;

    bool ffd_enabled = false;
    bool ffd_show_control_grid = true;
    FFDSettings ffd;
};
