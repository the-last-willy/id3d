#pragma once

#include "scene/vertex_attribute/all.hpp"
#include "object_group_data.hpp"
#include "object_group_topology.hpp"

struct ObjectGroup {
    ObjectGroupData data;
    ObjectGroupTopology topology;
    VertexAttributeGroup vertex_attributes;
};
