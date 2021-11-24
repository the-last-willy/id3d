#pragma once

#include "scene.hpp"

#include <tiny_obj_loader.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

inline
Scene wavefront_scene(std::filesystem::path file_path) {
    auto config = tinyobj::ObjReaderConfig();
    config.mtl_search_path = file_path.parent_path().string();
    auto reader = tinyobj::ObjReader();
    if(not reader.ParseFromFile(file_path.string(), config)) {
        if(not reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
        }
        throw std::runtime_error("");
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();
    std::ignore = materials;

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
    for(std::size_t s = 0; s < size(shapes); ++s) {
        auto& shape = shapes[s];
        auto& mesh = shape.mesh;
        scene.triangle_indices.reserve(
            size(scene.triangle_indices) + size(mesh.indices) / 3);
        for(std::size_t i = 0; i < size(mesh.indices); i += 3) {
            scene.triangle_indices.push_back(
                std::array<unsigned, 3>{
                    vertex_to_index[mesh.indices[i + 0]],
                    vertex_to_index[mesh.indices[i + 1]],
                    vertex_to_index[mesh.indices[i + 2]]});
        }
    }

    return scene;
}
