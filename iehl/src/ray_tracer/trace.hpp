#pragma once

#include "../random/all.hpp"
#include "point_cloud/all.hpp"
#include "scene/all.hpp"
#include "ray_tracer.hpp"
#include "ray.hpp"

struct Triangle {
    std::array<agl::Vec3, 3> vertices;
};

struct RayTriangleIntersection {
    // Ray parameter.
    float ray;
    // Barycentric coordinates.
    std::array<float, 3> triangle;
};

inline
std::optional<RayTriangleIntersection> opt_intersection(
    const Ray& r,
    const Triangle& tr)
{
    auto e1 = tr.vertices[1] - tr.vertices[0];
    auto e2 = tr.vertices[2] - tr.vertices[0];

    auto pvec = cross(r.direction, e2);
    auto det = dot(e1, pvec);

    auto inv_det = 1 / det;
    auto tvec = r.origin - tr.vertices[0];
    
    auto u = dot(tvec, pvec) * inv_det;
    if(u < 0 || u > 1) return std::nullopt;
    
    auto qvec= cross(tvec, e1);
    auto v = dot(r.direction, qvec) * inv_det;
    if(v < 0 || u + v > 1) return std::nullopt;
    
    auto t = dot(e2, qvec) * inv_det;
    if(t < 0) return std::nullopt;
    
    return RayTriangleIntersection(t, {1.f - u - v, u, v});
}

struct IntersectedTriangle {
    std::array<float, 3> barycentric_coordinates;
    std::size_t index;
};

inline
std::optional<IntersectedTriangle> closest_intersection(
    const Scene& s,
    const Ray& r)
{
    auto& vas = s.objects.vertex_attributes;
    auto& t = s.objects.topology;

    auto found = false;
    auto found_index = std::size_t(0);
    auto closest = RayTriangleIntersection();
    auto tr_index = std::size_t(0);
    for(; tr_index < size(t.triangle_indices); ++tr_index) {
        auto& is = t.triangle_indices[tr_index];
        auto tr = Triangle({
            vas.positions[is[0]],
            vas.positions[is[1]],
            vas.positions[is[2]]});
        auto oi = opt_intersection(r, tr);
        if(oi) {
            closest = *oi;
            found = true;
            found_index = tr_index;
            break;
        }
    }
    for(++tr_index; tr_index < size(t.triangle_indices); ++tr_index) {
        auto& is = t.triangle_indices[tr_index];
        auto tr = Triangle({
            vas.positions[is[0]],
            vas.positions[is[1]],
            vas.positions[is[2]]});
        auto oi = opt_intersection(r, tr);
        if(oi) {
            if(oi->ray < closest.ray) {
                closest = *oi;
                found_index = tr_index;
            }
        }
    }
    if(found) {
        return IntersectedTriangle(closest.triangle, found_index);
    } else {
        return std::nullopt;
    }
}

inline
void trace(
    RayTracer& rt,
    const Scene& s,
    const Ray& r)
{
    auto& data = s.objects.data;
    auto& topo = s.objects.topology;
    auto& vas = s.objects.vertex_attributes;

    if(rt.pc.size < rt.pc.capacity) {
        auto ci = closest_intersection(s, r);
        if(ci) {
            auto triangle_id = ci->index;
            auto material_id = s.objects.data.triangle_material_ids[triangle_id];
            auto& material = s.materials.material_properties[material_id];
            auto [a, b, c] = s.objects.topology.triangle_indices[triangle_id];
            auto [u, v, w] = ci->barycentric_coordinates;

            auto position 
            = u * vas.positions[a] + v * vas.positions[b] + w * vas.positions[c];

            auto rand_emissive_triangle_id = std::uniform_int_distribution<GLuint>(
                0, GLuint(size(data.emissive_triangle_ids)) - 1);
            auto emissive_triangle_id = data.emissive_triangle_ids[rand_emissive_triangle_id(rt.random)];

            auto rand_01 = std::uniform_real_distribution<float>(0.f, 1.f);

            auto [ea, eb, ec] =  topo.triangle_indices[emissive_triangle_id];
            auto [eu, ev, ew] = random_triangle_barycentric_coordinates(
                rand_01(rt.random),
                rand_01(rt.random));
            
            auto enormal
            = eu * vas.normals[ea]
            + ev * vas.normals[eb]
            + ew * vas.normals[ec];
            auto eposition
            = eu * vas.positions[ea]
            + ev * vas.positions[eb]
            + ew * vas.positions[ec];

            auto light_dir = normalize(eposition - position);
            auto light_ray = Ray{
                .origin = position + 0.001f * light_dir,
                .direction = light_dir};

            auto lci = closest_intersection(s, light_ray);
            if(lci) {
                auto [la, lb, lc] = s.objects.topology.triangle_indices[lci->index];
                auto [lu, lv, lw] = lci->barycentric_coordinates;
                auto lposition
                = lu * vas.positions[la]
                + lv * vas.positions[lb]
                + lw * vas.positions[lc];

                auto dist = distance(lposition, eposition);
                if(is_emissive(material)) {
                    auto color = agl::vec3(
                        material.emission_factor[0],
                        material.emission_factor[1],
                        material.emission_factor[2]);
                    push_back(rt.pc, Point{
                        .color = color,
                        .normal = agl::normalize(
                            u * vas.normals[a]
                            + v * vas.normals[b]
                            + w * vas.normals[c]),
                        .position = position});
                } else if(dist < 0.001f) {
                    auto normal = agl::normalize(
                        u * vas.normals[a]
                        + v * vas.normals[b]
                        + w * vas.normals[c]);

                    auto distance_to_light = distance(position, eposition);
                    float attenuation = 8.f / (1.f + distance_to_light * distance_to_light);
                    float lambertian = std::abs(dot(normal, light_dir));

                    auto color = lambertian * attenuation * agl::vec3(
                        material.color_factor[0],
                        material.color_factor[1],
                        material.color_factor[2]);

                    push_back(rt.pc, Point{
                        .color = color,
                        .normal = normal,
                        .position = position});
                } else {
                    push_back(rt.pc, Point{
                        .color = agl::vec3(0.f),
                        .normal = agl::normalize(
                            u * vas.normals[a]
                            + v * vas.normals[b]
                            + w * vas.normals[c]),
                        .position = position});
                }
            }
        }
    }
}
