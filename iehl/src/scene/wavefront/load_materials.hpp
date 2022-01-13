#pragma once

#include "load_albedo_textures.hpp"
#include "load_ao_roughness_metallic_textures.hpp"
#include "load_material_properties.hpp"

inline
void load_materials(
    Scene& scene,
    const tinyobj::ObjReader& wavefront,
    const std::filesystem::path& folder_path)
{
    load_albedo_textures(scene, wavefront, folder_path);
    load_ao_roughness_metallic_textures(scene, wavefront, folder_path);
    load_material_properties(scene, wavefront, folder_path);
}
