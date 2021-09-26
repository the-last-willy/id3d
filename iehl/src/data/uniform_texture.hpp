#pragma once

#include <agl/engine/all.hpp>

namespace data {

inline
eng::Texture uniform_texture(agl::Vec3 value) {
    auto texture = eng::Texture();
    texture.texture = create(agl::TextureTarget::_2d);
    storage(
        texture.texture,
        GL_RGB8, agl::Width(1), agl::Height(1));
    image(
        texture.texture,
        agl::Level(0),
        0, 0, agl::Width(1), agl::Height(1),
        GL_RGB, GL_FLOAT,
        as_bytes(std::span(agl::data(value), size(value))));
    return texture;
}

}
