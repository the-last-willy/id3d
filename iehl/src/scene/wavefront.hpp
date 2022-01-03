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

    // LIGHT PER OBJECT EXCEPT ITS NOT WORKING VERY WELL.

    // auto t = std::size_t();
    // for(t = 0; t < size(scene.triangle_material_ids); ++t) {
    //     auto mid = scene.triangle_material_ids[t];
    //     auto oid = scene.triangle_object_ids[t];
    //     auto& material = scene.materials[mid];
    //     if(is_emissive(material)) {
    //         auto bounds = agl::common::interval(
    //             scene.vertex_positions[scene.triangle_indices[t][0]]);
    //         for(; t < size(scene.triangle_material_ids); ++t) {
    //             if(scene.triangle_object_ids[t] != oid
    //                 or scene.triangle_material_ids[t] != mid)
    //             {
    //                 break;
    //             } else {
    //                 for(std::size_t i = 0; i < 3; ++i) {
    //                     extend(
    //                         bounds, 
    //                         scene.vertex_positions[scene.triangle_indices[t][i]]);
    //                 }
    //             }
    //         }
    //         auto& l = scene.lights.emplace_back();
    //         l.color = agl::vec4(
    //             material.emission_factor[0],
    //             material.emission_factor[1],
    //             material.emission_factor[2],
    //             1.f);
    //         if(mid != -1) {
    //             glGetTextureSubImage(
    //                 scene.albedo_array_texture.texture,
    //                 0,
    //                 0, 0, mid,
    //                 1, 1, 1,
    //                 GL_RGB,
    //                 GL_FLOAT,
    //                 32,
    //                 data(l.color));
    //         }
    //         l.position = agl::vec4(midpoint(bounds), 1.f);
    //     }
    // }

    // Light per triangle.
    auto t = std::size_t();
    for(t = 0; t < size(scene.objects.data.triangle_material_ids); ++t) {
        auto mid = scene.objects.data.triangle_material_ids[t];
        auto& material = scene.material_group.material_properties[mid];
        if(is_emissive(material)) {
            auto& l = scene.light_group.light_properties.emplace_back();
            l.attenuation = agl::vec4(1.f, 0.f, 50.f, 0.f) * 1.f;
            l.position = agl::vec4(triangle_centroid(scene.objects, t), 1.f);
            if(mid != -1) {
                glGetTextureSubImage(
                    scene.material_group.albedo_texture_array,
                    0,
                    0, 0, mid,
                    1, 1, 1,
                    GL_RGB,
                    GL_FLOAT,
                    32,
                    data(l.rgb_color));
            }
        }
    }
    std::cout << "  light count = " << size(scene.light_group.light_properties) << std::endl;
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
        glSamplerParameteri(scene.material_group.albedo_sampler,
            GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(scene.material_group.albedo_sampler,
            GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureStorage3D(scene.material_group.albedo_texture_array,
            1, GL_RGB8, width, height, depth);
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
                    scene.material_group.albedo_texture_array,
                    0,
                    0, 0, GLsizei(m),
                    width, height, 1,
                    GL_RGB, GL_UNSIGNED_BYTE,
                    resized.get());
            } else {
                glTextureSubImage3D(
                    scene.material_group.albedo_texture_array,
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
                scene.material_group.albedo_texture_array,
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
        s.material_group.material_properties.resize(size(materials));
        for(std::size_t i = 0; i < size(materials); ++i) {
            auto& m = materials[i];
            auto& sm = s.material_group.material_properties[i];
            sm.color_factor = {m.diffuse[0], m.diffuse[1], m.diffuse[2], 1.f};
            sm.emission_factor = {m.emission[0], m.emission[1], m.emission[2], 1.f};
        }
    }
    {
        gl::NamedBufferStorage(s.material_group.material_properties_ssbo,
            std::span(s.material_group.material_properties));
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
                    scene.objects.vertex_attributes.normals.push_back(
                        agl::vec3(
                            attrib.normals[3 * normal_i + 0],
                            attrib.normals[3 * normal_i + 1],
                            attrib.normals[3 * normal_i + 2]));
                }
                auto position_i = vertex.vertex_index;
                scene.objects.vertex_attributes.positions.push_back(
                    agl::vec3(
                        attrib.vertices[3 * position_i + 0],
                        attrib.vertices[3 * position_i + 1],
                        attrib.vertices[3 * position_i + 2]));
                auto texcoord_i = vertex.texcoord_index;
                if(texcoord_i != -1) {
                    scene.objects.vertex_attributes.texcoords.push_back(
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
        agl::standard::reserve_more(scene.objects.data.triangle_material_ids, tcount);
        agl::standard::reserve_more(scene.objects.topology.triangle_indices, tcount);
        for(std::size_t i = 0; i < size(mesh.indices); i += 3) {
            scene.objects.topology.triangle_indices.push_back(
                std::array<unsigned, 3>{
                    vertex_to_index[mesh.indices[i + 0]],
                    vertex_to_index[mesh.indices[i + 1]],
                    vertex_to_index[mesh.indices[i + 2]]});
            scene.objects.data.triangle_material_ids.push_back(
                mesh.material_ids[i / 3]);
            scene.objects.data.triangle_object_ids.push_back(s);
        }
    }

    load_albedo_textures(scene, reader, folder_path);
    load_materials(scene, reader);

    load_lights(scene);

    { // Light group.
        if(size(scene.light_group.light_properties) > 0) {
            gl::NamedBufferStorage(scene.light_group.light_properties_ssbo,
                std::span(scene.light_group.light_properties));
        }
    }
    { // Object group.
        gl::NamedBufferStorage(scene.objects.topology.element_buffer,
            std::span(scene.objects.topology.triangle_indices));

        scene.objects.topology.draw_commands.push_back(gl::DrawElementsIndirectCommand{
            .count = 3 * GLuint(size(scene.objects.topology.triangle_indices)),
            .instanceCount = 1,
            .firstIndex = 0,
            .baseVertex = 0,
            .baseInstance = 0});
        gl::NamedBufferStorage(scene.objects.topology.draw_command_buffer,
            std::span(scene.objects.topology.draw_commands));

        scene.objects.topology.draw_count = 1;
    }
    { // Object group data.
        gl::NamedBufferStorage(
            scene.objects.data.triangle_material_id_ssbo,
            std::span(scene.objects.data.triangle_material_ids));
    }
    { // Vertex attribute group.
        if(size(scene.objects.vertex_attributes.normals) > 0) {
            gl::NamedBufferStorage(scene.objects.vertex_attributes.normal_buffer,
                std::span(scene.objects.vertex_attributes.normals));
        }
        if(size(scene.objects.vertex_attributes.positions) > 0) {
            gl::NamedBufferStorage(scene.objects.vertex_attributes.position_buffer,
                std::span(scene.objects.vertex_attributes.positions));
        }
        if(size(scene.objects.vertex_attributes.texcoords) > 0) {
            gl::NamedBufferStorage(scene.objects.vertex_attributes.texcoords_buffer,
                std::span(scene.objects.vertex_attributes.texcoords));
        }
    }
    { // Compute scene bounds.
        auto& positions = scene.objects.vertex_attributes.positions;
        if(not positions.empty()) {
            scene.objects.data.bounds = agl::common::interval(agl::vec4(positions[0], 1.f));
            for(auto& p : positions) {
                extend(scene.objects.data.bounds, agl::vec4(p, 1.f));
            }
        }
    }

    return scene;
}
