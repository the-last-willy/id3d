#pragma once

#include "lighting/light_group.hpp"
#include "material/material_group.hpp"
#include "object/object_group.hpp"

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Scene {
    LightGroup lights;
    MaterialGroup materials;
    ObjectGroup objects;
};
