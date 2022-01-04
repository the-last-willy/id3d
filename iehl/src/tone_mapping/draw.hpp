#pragma once

#include "tone_mapper.hpp"

inline
void draw(const ToneMapper&) {
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
