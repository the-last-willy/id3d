#pragma once

#include <stb_image_resize.h>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include <iostream>

inline
void load_ao_roughness_metallic_textures(
    Scene& scene,
    const tinyobj::ObjReader& wavefront,
    const std::filesystem::path& folder_path)
{
    std::cout << "--Loading AO/roughness/metallic textures." << std::endl;

    auto& materials = wavefront.GetMaterials();

    constexpr auto height = agl::Height(1024);
    constexpr auto width = agl::Width(1024);
    auto depth = agl::Depth(GLsizei(size(materials)));

    auto sampler = gl::Sampler();
    {
        glSamplerParameteri(sampler,
            GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler,
            GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    auto texture_array = gl::Texture(GL_TEXTURE_2D_ARRAY);
    {
        glTextureStorage3D(texture_array,
            1, GL_RGB8, width, height, depth);
    }

    stbi_set_flip_vertically_on_load(true);
    for(std::size_t m = 0; m < size(materials); ++m) {
        auto& material = materials[m];
        if(not empty(material.specular_texname)) {
            auto texture_path = (folder_path / material.specular_texname).string();
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
                    texture_array,
                    0,
                    0, 0, GLsizei(m),
                    width, height, 1,
                    GL_RGB, GL_UNSIGNED_BYTE,
                    resized.get());
            } else {
                glTextureSubImage3D(
                    texture_array,
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
                texture_array,
                0,
                0, 0, 0,
                width, height, depth,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data(white));
        }
    }
    scene.materials.ao_roughness_metallic_sampler = std::move(sampler);
    scene.materials.ao_roughness_metallic_texture_array = std::move(texture_array);
}
