// Definitions.

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include "all.hpp"
#include "error_callback.hpp"


#include <local/all.hpp>
#include <program/all.hpp>

// External libraries.

#include <agl/engine/all.hpp>
#include <agl/standard/all.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library.

#include <iostream>

//

namespace galin {}

using namespace galin;

struct Settings {
    int steps = 200;
    float threshold = 0.5;

    std::vector<agl::SubroutineIndex> subroutines;
};

struct App : Program {
    Settings settings;

    eng::ShaderCompiler shader_compiler = {};

    agl::engine::View view = {};

    agl::VertexArray empty_vertex_array = agl::create(agl::vertex_array_tag);
    eng::Program program = {};

    float time = 0.f;

    bool shader_loaded = false;

    void load_shader() {
        try {
            { // Render pass.
                shader_compiler.log_folder = "logs/";
                shader_compiler.root = "galin/src/shader/";
                load(program, shader_compiler, {
                    {
                        agl::vertex_shader_tag,
                        "shadertoy.vs"
                    },
                    {
                        agl::fragment_shader_tag,
                        "kame_house.glsl"
                    }
                });
            }
            shader_loaded = true;
        } catch(...) {
            shader_loaded = false;
        }
        if(shader_loaded) {
            uniform(program, "iResolution", agl::vec3(
                static_cast<float>(window.width()),
                static_cast<float>(window.height()),
                0.f));
            { // Subroutines.
                settings.subroutines.resize(
                    active_subroutine_uniform_locations(
                        program.program, agl::fragment_shader_tag));
            }
        }
    }

    void init() override {
        load_shader();
    }

    void update(float dt) override {
        time += dt;

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
                auto direction = (rotation(view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                view.position = view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_D)) {
                auto direction = (rotation(view) * agl::rotation_y(agl::constant::pi / 2.f))[2].xyz();
                view.position = view.position + direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_S)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position - direction / 10.f;
            }
            if(glfwGetKey(window.window, GLFW_KEY_W)) {
                auto direction = rotation(view)[2].xyz();
                view.position = view.position + direction / 10.f;
            }
        }
        { // Shader hot reloading.
            if(glfwGetKey(window.window, GLFW_KEY_R)) {
                load_shader();
            }
        }

        if(shader_loaded) {
            double xpos, ypos;
            glfwGetCursorPos(window.window, &xpos, &ypos);
            auto pressed = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_1);
            uniform(program, "iMouse", agl::vec4(
                static_cast<float>(xpos),
                static_cast<float>(ypos),
                static_cast<float>(pressed),
                0.f
            ));

            uniform(program, "view", inverse(transform(view)));
        }
    }

    void ui() {
        ImGui::Begin("Settings");
        ImGui::DragInt("Steps", &settings.steps, 5.f, 1, 1000, "%d");
        ImGui::DragFloat("Threshold", &settings.threshold, 0.001f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        ImGui::End();
    }

    void render() override {
        clear(agl::default_framebuffer, agl::color_tag, {0., 0., 0., 1.});
        if(shader_loaded) {
            bind(empty_vertex_array);
            bind(program);
            uniform(program, "iTime", time);

            uniform(program, "view_transform", transform(view));

            uniform(program, "Steps", settings.steps);
            uniform(program, "T", settings.threshold);

            draw_arrays(
                agl::DrawMode::triangles,
                agl::Offset<GLint>(0),
                agl::Count<GLsizei>(6));
        }
        ui();
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