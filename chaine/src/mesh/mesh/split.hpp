#pragma once

#include "mesh/index/all.hpp"
#include "mesh/triangle/all.hpp"
#include "mesh/vertex/all.hpp"

namespace face_vertex {

inline
VertexProxy split(TriangleProxy triangle) {
	auto p = create_vertex(mesh(triangle));
	topology(p)->triangle = triangle;

	auto vt2 = vertex(triangle, 2);
	topology(vt2)->triangle = adjacent_triangle(triangle, 0);

	substitute_vertex(triangle, vt2, p);
	
	auto t0 = create_triangle(mesh(triangle));
	topology(t0)->vertices[0] = p;
	topology(t0)->vertices[1] = vertex(triangle, 1);
	topology(t0)->vertices[2] = vt2;
	topology(t0)->triangles[2] = triangle;

	auto t1 = create_triangle(mesh(triangle));
	topology(t1)->vertices[0] = p;
	topology(t1)->vertices[1] = vt2;
	topology(t1)->vertices[2] = vertex(triangle, 0);
	topology(t1)->triangles[1] = triangle;

	topology(t0)->triangles[1] = t1;
	topology(t1)->triangles[2] = t0;

	auto old_adjacent0 = adjacent_triangle(triangle, 0);
	if(has_adjacent_triangle(topology(triangle), 0)) {
		substitute_adjacent_triangle(old_adjacent0, triangle, t0);
		topology(t0)->triangles[0] = old_adjacent0;
	}
	topology(triangle)->triangles[0] = index(t0);

	auto old_adjacent1 = adjacent_triangle(triangle, 1);
	if(has_adjacent_triangle(topology(triangle), 1)) {
		substitute_adjacent_triangle(old_adjacent1, triangle, t1);
		topology(t1)->triangles[0] = old_adjacent1;
	}
	topology(triangle)->triangles[1] = index(t1);

	return p;
}

}