#pragma once

#include "z_prepasser.hpp"

inline
void bind(const Z_Prepasser& zp) {
    glUseProgram(zp.program);
}
