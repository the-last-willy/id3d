#pragma once

#include "point_cloud.hpp"
#include "point.hpp"

inline
void push_back(
    PointCloud& pc,
    const Point& p)
{
    glNamedBufferSubData(pc.color_vbo,
        pc.size * sizeof(Point::Color),
        sizeof(Point::Color), data(p.color));
    glNamedBufferSubData(pc.normal_vbo,
        pc.size * sizeof(Point::Normal),
        sizeof(Point::Normal), data(p.normal));
    glNamedBufferSubData(pc.position_vbo,
        pc.size * sizeof(Point::Position),
        sizeof(Point::Position), data(p.position));
    pc.size += 1;
}
