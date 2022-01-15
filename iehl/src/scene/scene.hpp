#pragma once

#include "scene/lighting/all.hpp"
#include "scene/material/all.hpp"
#include "scene/object/all.hpp"
#include "scene/vertex_attribute/all.hpp"

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Scene {
    LightGroup lights;
    MaterialGroup materials;
    ObjectGroup objects;
};
