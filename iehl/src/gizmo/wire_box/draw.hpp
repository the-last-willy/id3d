#pragma once

#include "topology.hpp"

inline
void draw(const WireBoxTopology& t) {
    glDrawElements(t.draw_mode, t.draw_count, t.draw_type, 0);
}
