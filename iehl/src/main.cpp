// Definitions.

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "data/all.hpp"
#include <local/all.hpp>
#include "program/all.hpp"

#include <agl/all.hpp>
#include <agl/format/gltf2/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>

//

namespace iehl {}

using namespace iehl;

struct App : Program {
    eng::ShaderCompiler shader_compiler = {};

    format::gltf2::Content database;

    tlw::View view = {};
    eng::PerspectiveProjection projection = {};

    eng::RenderPass forward_ambient_render_pass;
    
    void init() override {
        shader_compiler.log_folder = "../../../logs/";
        shader_compiler.root = "../../../iehl/src/shader/";

        database = format::gltf2::load("D:/data/sample/gltf2/box/Box/glTF/Box.gltf");

        { // Render pass.
            forward_ambient_render_pass = data::forward_ambient_render_pass(shader_compiler);
            for(auto& m : database.meshes | ranges::views::values | ranges::views::indirect) {
                add(forward_ambient_render_pass, m);
            }
        }

        projection.aspect_ratio = 16.f / 9.f;
    }

    void update(float) override {
        if(glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1)) {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            agl::Vec2 d = current_cursor_pos - previous_cursor_pos;
            view.yaw += d[0] / 500.f;
            view.pitch += d[1] / 500.f;

            previous_cursor_pos = current_cursor_pos;
        } else {
            glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            previous_cursor_pos = current_cursor_pos;
        }
        {
            if(glfwGetKey(window.window, GLFW_KEY_A)) {
                auto direction = (rotation(view) * agl::rotation_y(agl::pi / 2.f))[2].xyz();
                view.position = view.position - direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(view) * agl::rotation_y(agl::pi / 2.f))[2].xyz();
                view.position = view.position + direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position - direction / 100.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position + direction / 100.f;
            }
        }
    }

    void render() override {
        glClearDepthf(1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
 
        auto v = inverse(transform(view));
        auto vp = transform(projection) * v;

        forward_ambient_render_pass.uniforms["mvp_transform"]
        = std::make_shared<eng::Uniform<agl::Mat4>>(vp);

        { // Forward ambient lighting.
            eng::render(forward_ambient_render_pass);
        }
    }

    
};

void throwing_main() {
    auto a = App();
    run(a);
}

int main() {
    try {
        throwing_main();
        return 0;
    } catch(const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Unhandled exception." << std::endl;
    }
    return -1;
}
