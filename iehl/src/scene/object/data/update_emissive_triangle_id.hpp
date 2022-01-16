#pragma once

#include "../scene.hpp"

inline
void update_emissive_triangle_ids(Scene& s) {
    s.objects.data.emissive_triangle_ids.clear();
    for(GLuint ti = 0; ti < size(s.objects.topology.triangle_indices); ++ti) {
        auto material_id = s.objects.data.triangle_material_ids[ti];
        if(material_id != -1) {
            if(is_emissive(s.materials.material_properties[material_id])) {
                s.objects.data.emissive_triangle_ids.push_back(ti);
            }
        }
    }
}
