# Meshes and Computational Geometry

This project is made by:
- Willy Jacquet
- Yoan Parra

# Mesh interface

A **mesh** is composed of two things: a **geometry** and a **topology**.

The *geometry* has the same structure accross *mesh representations*.
(Not quite currently the case, but that's the goal).

Mesh components (vertices, edges, faces) can be accessed through proxies.

A [proxy](https://en.wikipedia.org/wiki/Proxy_pattern) provides a complete interface to a component
and all possible operations - both on geometry and topology - are expressed through them.
Depending on the *topology*, proxies will provide different set of operations.

Currently the *geometry* has a fixed set of attributes:
- for vertices: color, normal, position

The *attributes* can be accessed and **modified** through proxies:
```cpp
auto v = /*VertexProxy*/;
color(v) = /*...*/;
normal(v) = /*...*/;
position(v) = /*...*/;
```

# Supported mesh representations

## Face-vertex mesh



Operations on `Mesh`:
```cpp
using namespace face_vertex_mesh;

auto m = Mesh();

for(auto&& t : triangles(m)) {
    // Iterates through triangles.
}

for(auto&& v : vertices(m)) {
    // Iterates through vertices.
}
```

Operations on `TriangleProxy`:
```cpp
auto t = /*TriangleProxy*/;

// Access a vertex of a triangle by relative index.
vertex(t, /*[0, 3)*/);

for(auto&& t2 : triangles(t)) {
    // Iterates through adjacent triangles of a triangle.
}

for(auto&& v : vertices(t)) {
    // Iterates through vertices of a triangle.
}
```

Operations on `VertexProxy`:
```cpp
auto v = /*VertexProxy*/;

// Access one the triangle this vertex is part of.
any_adjacent_triangle(v);

for(auto&& t : adjacent_triangles(v)) {
    // Iterates through adjacent triangles of a vertex.
}
```

## Half-edge mesh

Not supported *yet*.

## Triangle mesh

The *triangle mesh* is mainly intended for rendering has it can be directly uploaded on GPU.

## Conversion

- triangle mesh -> face-vertex mesh

# Algorithms

## Laplacian

**Vertex normals** and **vertex mean curvature** can approximated through this algorithm.

**TODO: Insert images**

# Rendering

# UI

# Usage
