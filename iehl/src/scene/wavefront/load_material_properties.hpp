#pragma once

#include <stb_image_resize.h>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include <iostream>

inline
void load_material_properties(
    Scene& s,
    const tinyobj::ObjReader& wavefront)
{
    std::cout << "--Loading material properties." << std::endl;

    auto& materials = wavefront.GetMaterials();
    { // Material factors.
        s.materials.material_properties.resize(size(materials));
        for(std::size_t i = 0; i < size(materials); ++i) {
            auto& m = materials[i];
            auto& sm = s.materials.material_properties[i];
            sm.color_factor = {m.diffuse[0], m.diffuse[1], m.diffuse[2], 1.f};
            sm.emission_factor = {m.emission[0], m.emission[1], m.emission[2], 1.f};
        }
    }
    {
        gl::NamedBufferStorage(s.materials.material_properties_ssbo,
            std::span(s.materials.material_properties));
    }
}
