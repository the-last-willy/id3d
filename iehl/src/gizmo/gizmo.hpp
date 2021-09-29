#pragma once

#include <agl/engine/all.hpp>

#include <memory>

inline
eng::Program line_segment_program() {
    const auto fragment_shader_source = std::string(
        "#version 450 core\n"
        "uniform vec3 color = vec3(1.);\n"
        "out vec3 fragment_color = color;\n"
        "void main {\n"
        "    fragment_color = color;\n"
        "}\n");
        
    const auto vertex_shader_source = std::string(
        "#version 450 core\n"
        "uniform vec3 positions[2];\n"
        "uniform mat4 transform = mat4(\n"
        "    1., 0., 0., 0.,\n"
        "    0., 1., 0., 0.,\n"
        "    0., 0., 1., 0.,\n"
        "    0., 0., 0., 1.);\n"
        "void main() {\n"
        "    if(gl_PrimitiveID == 0) {\n"
        "        gl_Position = transform * vec4(position0, 1.);\n"
        "    } else {\n"
        "        gl_Position = transform * vec4(position1, 1.);\n"
        "    }\n"
        "}\n");
    auto p = eng::Program();
    load(p, {
        {
            agl::fragment_shader_tag,
            fragment_shader_source
        }, {
            agl::vertex_shader_tag,
            vertex_shader_source
        }});
    return p;
}

inline
eng::Mesh line_segment_mesh() {
    auto me = eng::Mesh();
    auto& p = *(me.primitives.emplace_back(std::make_shared<eng::Primitive>()));
    p.draw_mode = agl::DrawMode::lines;
    p.draw_type = agl::DrawType::unsigned_byte;
    p.primitive_count = agl::Count<GLsizei>(2);
    return me;
}

struct Gizmo {
    Gizmo() {
        line_segment_pass.program = std::make_shared<eng::Program>(
            line_segment_program());
        subscribe(line_segment_pass, 
            std::make_shared<eng::Mesh>(line_segment_mesh()));
    }

    agl::engine::RenderPass line_segment_pass;
};

inline
void draw_line_segment(Gizmo& g, std::array<agl::Vec3, 2> positions) {
    g.line_segment_pass.uniforms["position0"]
    = std::make_shared<eng::Uniform<agl::Vec3>>(positions[0]);
    g.line_segment_pass.uniforms["position1"]
    = std::make_shared<eng::Uniform<agl::Vec3>>(positions[1]);
    render(g.line_segment_pass);
}

inline
void draw_line_segment(
    Gizmo& g,
    std::array<agl::Vec3, 2> positions,
    const agl::Mat4& transform)
{
    g.line_segment_pass.uniforms["position0"]
    = std::make_shared<eng::Uniform<agl::Vec3>>(positions[0]);
    g.line_segment_pass.uniforms["position1"]
    = std::make_shared<eng::Uniform<agl::Vec3>>(positions[1]);
    g.line_segment_pass.uniforms["transform"]
    = std::make_shared<eng::Uniform<agl::Mat4>>(transform);
    render(g.line_segment_pass);
}
