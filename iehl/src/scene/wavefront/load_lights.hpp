#pragma once

#include "scene/lighting/all.hpp"

#include <tiny_obj_loader.h>

#include <iostream>

inline
bool is_emissive(const tinyobj::material_t& m) {
    return false
    or m.emission[0] > 0.f
    or m.emission[1] > 0.f
    or m.emission[2] > 0.f; 
}

inline
void load_lights(
    LightGroup& lg,
    const tinyobj::ObjReader& wavefront)
{
    std::cout << "--Loading lights." << std::endl;

    auto& attrib = wavefront.GetAttrib();
    auto& materials = wavefront.GetMaterials();
    auto& shapes = wavefront.GetShapes();

    for(std::size_t si = 0; si < size(shapes); ++si) {
        auto& shape = shapes[si];
        auto& mesh = shape.mesh;
        for(std::size_t fi = 0; fi < size(mesh.material_ids);) {
            auto material_id = mesh.material_ids[fi];
            auto& material = materials[material_id];
            if(is_emissive(material)) {
                auto centroid_sum = agl::vec3(0.f);
                auto face_count = 0;
                auto total_area = 0.f;

                auto centroid0 = agl::vec3(0.f);
                {
                    for(int vi = 0; vi < 3; ++vi) {
                        auto vertex_id = mesh.indices[3 * fi + vi].vertex_index;
                        centroid0 = agl::vec3(
                            attrib.vertices[3 * vertex_id + 0],
                            attrib.vertices[3 * vertex_id + 1],
                            attrib.vertices[3 * vertex_id + 2]);
                    }
                }
                for(; fi < size(mesh.material_ids); ++fi) {
                    if(mesh.num_face_vertices[fi] != 3) {
                        throw std::runtime_error("Not supported.");
                    }
                    if(mesh.material_ids[fi] != material_id) {
                        break;
                    }
                    auto positions = std::array<agl::Vec3, 3>();
                    for(int vi = 0; vi < 3; ++vi) {
                        auto vertex_id = mesh.indices[3 * fi + vi].vertex_index;
                        positions[vi] = agl::vec3(
                            attrib.vertices[3 * vertex_id + 0],
                            attrib.vertices[3 * vertex_id + 1],
                            attrib.vertices[3 * vertex_id + 2]);
                        
                    }
                    auto centroid = (positions[0] + positions[1] + positions[2]) / 3.f;
                    if(distance(centroid, centroid0) > 2.f) {
                        break;
                    }
                    centroid_sum += centroid;
                    face_count += 1;
                    total_area += length(
                        cross(
                            positions[1] - positions[0],
                            positions[2] - positions[0])) 
                        / 2.f;
                }
                auto light = LightProperties();
                light.attenuation = agl::vec4(1.f, 0.f, 1.f, 0.f) / std::sqrt(total_area) / 32.f;
                light.rgb_color = agl::vec4(
                    material.emission[0],
                    material.emission[1],
                    material.emission[2],
                    1.f);
                light.position = agl::vec4(centroid_sum / float(face_count), 1.f);
                lg.light_properties.push_back(light);
            } else {
                for(++fi; fi < size(mesh.material_ids); ++fi) {
                    if(mesh.material_ids[fi] != material_id) {
                        break;
                    }
                }
            }
        }
    }
    std::cout << "unmerged light count=" << size(lg.light_properties) << std::endl;
    { // Merge lights.
        auto unmerged_lights = std::move(lg.light_properties);
        for(auto& ul : unmerged_lights) {
            bool is_merged = false;
            for(auto& l : lg.light_properties) {
                if(are_mergeable(l, ul)) {
                    l = merged(l, ul);
                    is_merged = true;
                }
            }
            if(not is_merged) {
                lg.light_properties.push_back(ul);
            }
        }
    }
    { // Upload to GPU.
        if(size(lg.light_properties) > 0) {
            gl::NamedBufferStorage(lg.light_properties_ssbo,
                lg.light_properties);
        }
    }
    std::cout << "merged light count=" << size(lg.light_properties) << std::endl;
}
