#pragma once

#include "opengl.hpp"

struct ObjectGroupData {
    std::vector<int> triangle_material_ids;
    gl::Buffer triangle_material_id_ssbo;

    // Unused ?
    std::vector<std::size_t> triangle_object_ids; 
};
