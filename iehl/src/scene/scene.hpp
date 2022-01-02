#pragma once

#include "lighting/all.hpp"
#include "material/all.hpp"
#include "object/all.hpp"
#include "vertex_attribute/all.hpp"

#include <agl/engine/all.hpp>

#include <array>
#include <vector>

struct Scene {
    LightGroup light_group;
    MaterialGroup material_group;
    ObjectGroup object_group;
    ObjectGroupData object_group_data;
    VertexAttributeGroup vertex_attribute_group;

    // Cached.

    // Should go inside geometry.
    agl::common::Interval<agl::Vec3> bounds;
};
