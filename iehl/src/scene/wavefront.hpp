#pragma once

#include "scene.hpp"

#include <stb_image_resize.h>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

inline
void load_lights(Scene& scene) {
    std::cout << "Loading lights." << std::endl;
    auto t = std::size_t();
    for(t = 0; t < size(scene.triangle_material_ids); ++t) {
        auto mid = scene.triangle_material_ids[t];
        auto& material = scene.materials[mid];
        if(is_emissive(material)) {
            auto bounds = agl::common::interval(
                scene.vertex_positions[scene.triangle_indices[t][0]]);
            for(; t < size(scene.triangle_material_ids); ++t) {
                if(scene.triangle_material_ids[t] != mid) {
                    break;
                }
            }
            auto& l = scene.lights.emplace_back();
            l.color = agl::vec4(
                material.emission_factor[0],
                material.emission_factor[1],
                material.emission_factor[2],
                1.f);
            l.position = agl::vec4(midpoint(bounds), 1.f);
        }
    }
    std::cout << "  light count = " << size(scene.lights) << std::endl;
}

inline
void load_albedo_textures(
    Scene& scene,
    const tinyobj::ObjReader& wavefront,
    const std::filesystem::path& folder_path)
{
    std::cout << "Loading albedo textures." << std::endl;

    auto& materials = wavefront.GetMaterials();

    constexpr auto height = agl::Height(1024);
    constexpr auto width = agl::Width(1024);
    auto depth = agl::Depth(GLsizei(size(materials)));

    {
        auto s = agl::create(agl::sampler_tag);
        mag_filter(s, GL_LINEAR);
        min_filter(s, GL_LINEAR);
        auto t = agl::create(agl::TextureTarget::_2d_array);
        storage(t, 1, GL_RGB8, width, height, depth);
        scene.albedo_array_texture = {s, t};
    }

    stbi_set_flip_vertically_on_load(true);
    for(std::size_t m = 0; m < size(materials); ++m) {
        auto& material = materials[m];
        if(not empty(material.diffuse_texname)) {
            auto texture_path = (folder_path / material.diffuse_texname).string();
            int x, y, n;
            auto data = stbi_load(
                texture_path.c_str(),
                &x, &y, &n, 3);
            if(data == NULL) { 
                throw std::runtime_error(
                    "Failed to open texture \"" + texture_path + "\"");
            }
            if(x != width.value or y != width.value) {
                auto resized = std::make_unique_for_overwrite<unsigned char[]>(3 * width.value * height.value);
                stbir_resize_uint8(
                    data, x, y, 0,
                    resized.get(), width.value, height.value, 0,
                    3);
                glTextureSubImage3D(
                    scene.albedo_array_texture.texture,
                    0,
                    0, 0, GLsizei(m),
                    width, height, 1,
                    GL_RGB, GL_UNSIGNED_BYTE,
                    resized.get());
            } else {
                glTextureSubImage3D(
                    scene.albedo_array_texture.texture,
                    0,
                    0, 0, GLsizei(m),
                    x, y, 1,
                    GL_RGB, GL_UNSIGNED_BYTE,
                    data);
            }
            stbi_image_free(data);
        } else {
            auto white = std::array<GLubyte, 3>{255, 255, 255};
            glClearTexSubImage(
                scene.albedo_array_texture.texture,
                0,
                0, 0, 0,
                width, height, depth,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data(white));
        }
    }
}

inline
void load_materials(Scene& s, const tinyobj::ObjReader& wavefront) {
    auto& materials = wavefront.GetMaterials();
    { // Material factors.
        s.materials.resize(size(materials));
        for(std::size_t i = 0; i < size(materials); ++i) {
            auto& m = materials[i];
            auto& sm = s.materials[i];
            sm.color_factor = {m.diffuse[0], m.diffuse[1], m.diffuse[2], 1.f};
            sm.emission_factor = {m.emission[0], m.emission[1], m.emission[2], 1.f};
        }
    }
}

inline
Scene wavefront_scene(std::filesystem::path file_path) {
    auto folder_path = file_path.parent_path();

    auto config = tinyobj::ObjReaderConfig();
    config.mtl_search_path = folder_path.string();
    auto reader = tinyobj::ObjReader();
    if(not reader.ParseFromFile(file_path.string(), config)) {
        if(not reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
        }
        throw std::runtime_error("");
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    auto scene = Scene();

    auto vertex_less = [] (auto a0, auto a1) {
        if(a0.normal_index != a1.normal_index) {
            return a0.normal_index < a1.normal_index;
        } else if(a0.vertex_index != a1.vertex_index) {
            return a0.vertex_index < a1.vertex_index;
        } else {
            return a0.texcoord_index < a1.texcoord_index;
        }
    };
    auto vertex_to_index
    = std::map<tinyobj::index_t, unsigned, decltype(vertex_less)>(vertex_less);
    
    // Copy vertices.
    for(std::size_t s = 0; s < size(shapes); ++s) {
        auto& shape = shapes[s];
        auto& mesh = shape.mesh;
        for(std::size_t v = 0; v < size(mesh.indices); ++v) {
            auto vertex = mesh.indices[v];
            auto it = vertex_to_index.find(vertex);
            if(it == end(vertex_to_index)) {
                vertex_to_index[vertex] = unsigned(size(vertex_to_index));
                auto normal_i = vertex.normal_index;
                if(normal_i != -1) {
                    scene.vertex_normals.push_back(
                        agl::vec3(
                            attrib.normals[3 * normal_i + 0],
                            attrib.normals[3 * normal_i + 1],
                            attrib.normals[3 * normal_i + 2]));
                }
                auto position_i = vertex.vertex_index;
                scene.vertex_positions.push_back(
                    agl::vec3(
                        attrib.vertices[3 * position_i + 0],
                        attrib.vertices[3 * position_i + 1],
                        attrib.vertices[3 * position_i + 2]));
                auto texcoord_i = vertex.texcoord_index;
                if(texcoord_i != -1) {
                    scene.vertex_texcoords.push_back(
                        agl::vec2(
                            attrib.texcoords[2 * texcoord_i + 0],
                            attrib.texcoords[2 * texcoord_i + 1]));
                }
            }
        }
    }
    // Copy objects.
    std::cout << "Loading objects.\n";
    for(std::size_t s = 0; s < size(shapes); ++s) {
        auto& shape = shapes[s];
        // std::cout << "Loading \"" << shape.name << "\".\n";
        auto& mesh = shape.mesh;
        auto tcount = size(mesh.indices) / 3;
        agl::standard::reserve_more(scene.triangle_material_ids, tcount);
        agl::standard::reserve_more(scene.triangle_indices, tcount);
        for(std::size_t i = 0; i < size(mesh.indices); i += 3) {
            scene.triangle_indices.push_back(
                std::array<unsigned, 3>{
                    vertex_to_index[mesh.indices[i + 0]],
                    vertex_to_index[mesh.indices[i + 1]],
                    vertex_to_index[mesh.indices[i + 2]]});
            scene.triangle_material_ids.push_back(
                mesh.material_ids[i / 3]);
        }
    }
    load_albedo_textures(scene, reader, folder_path);
    load_materials(scene, reader);

    load_lights(scene);

    return scene;
}
