#pragma once

#include "load_lights.hpp"
#include "load_materials.hpp"

#include <stb_image_resize.h>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

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
            // scene.objects.data.triangle_object_ids.push_back(s);
        }
    }

    load_lights(scene.lights, reader);
    load_materials(scene, reader, folder_path);

    { // Object group.
        gl::NamedBufferStorage(scene.objects.topology.element_buffer,
            std::span(scene.objects.topology.triangle_indices));

        scene.objects.topology.commands.commands.push_back(gl::DrawElementsIndirectCommand{
            .count = 3 * GLuint(size(scene.objects.topology.triangle_indices)),
            .instanceCount = 1,
            .firstIndex = 0,
            .baseVertex = 0,
            .baseInstance = 0});
        gl::NamedBufferStorage(scene.objects.topology.commands.commands_buffer,
            std::span(scene.objects.topology.commands.commands));
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
