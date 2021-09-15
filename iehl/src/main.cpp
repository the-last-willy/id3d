// Definitions.

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION

// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include <agl/standard/all.hpp>
#include <local/all.hpp>
#include "data/all.hpp"
#include "agl/format/gltf2/all.hpp"
#include "program/all.hpp"

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>

//

using namespace iehl;

struct DirectionalLight {
    agl::Vec3 direction = {};
    agl::Mat4 transform = {};

    std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
};

struct SpotLight {
    agl::Vec3 direction = {};
    agl::Mat4 transform = {};

    std::shared_ptr<eng::Texture> shadow_map = std::make_shared<eng::Texture>();;
};

struct PointLight {
    agl::Vec3 position = {};
};

struct GltfProgram;

inline
void load_model(GltfProgram& program, const std::string& filepath);

struct GltfProgram : Program {
    std::vector<std::string> files = {
        "D:/data/sample/gltf2/box/Box/glTF/Box.gltf",
    };

    eng::ShaderCompiler shader_compiler = {};

    // glTF file.
    format::gltf2::Content database = {};

    // Default sampler.
    agl::Sampler default_sampler;

    // Ambient lighting.
    eng::Material ambient_light_mat = {};

    // Active camera.
    std::shared_ptr<eng::Camera> active_camera = {};

    // Player camera.
    tlw::PerspectiveProjection projection = {};
    tlw::View view = {};

    agl::engine::RenderPass ambient_pass;
    agl::engine::RenderPass blinn_phong_pass;

    std::shared_ptr<agl::engine::MeshInstance> object;
    agl::Mat4 object_model_transform;
    
    float time = 0.f;

    void init() override {
        { // Shader compiler.
            shader_compiler.log_folder = "logs/";
            shader_compiler.root = "iehl/src/shader";
        }

        database = format::gltf2::load(
            // "D:/data/gltf_sample_models/Sponza/glTF/Sponza.gltf"
            "D:/data/gltf_sample_models/Box/glTF/Box.gltf");

        { // First object in model.
            object = std::make_shared<agl::engine::MeshInstance>(
                begin(database.meshes)->second);
        }

        { // Render passes
            ambient_pass = data::forward_ambient_render_pass(shader_compiler);
            blinn_phong_pass = data::forward_blinn_phong_render_pass(shader_compiler);
        }
        {
            subscribe(ambient_pass, object);
            subscribe(blinn_phong_pass, object);
            // for(auto& m : database.meshes | ranges::views::values) {
            //     subscribe(ambient_pass, m);
            // }
            // for(auto& m : database.meshes | ranges::views::values) {
            //     subscribe(blinn_phong_pass, m);
            // }

        }

        { // Camera.
            // SCENE CAMERA DISABLED>
            if constexpr(true /* empty(database.cameras) */) {
                auto& c = *(active_camera = std::make_shared<eng::Camera>());
                if(auto pp = std::get_if<eng::PerspectiveProjection>(&c.projection)) {
                    pp->aspect_ratio = 16.f / 9.f;
                }
            } else {
                active_camera = database.cameras.begin()->second;
            }
        }
    }

    void update(float dt) override {
        { // Increment time.
            time += dt;
        }
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
        { // Camera controls.
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
        { // Update object.
            object_model_transform = agl::translation(std::cos(time), 0.f, 0.f);
        }
    }

    void render() override {
        clear(agl::default_framebuffer, agl::depth_tag, 1.f);

        auto inv_v = transform(view);
        auto v = inverse(inv_v);

        auto vp = transform(*active_camera) * v;

        auto normal_transform = transpose(inverse(v));

        auto light_position = (vp * agl::vec4(2.f, 2.f, 2.f, 1.f)).xyz();
        auto view_position = (vp * vec4(view.position, 1.f)).xyz();

        { // Object uniforms.
            object->uniforms["mvp_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(vp * object_model_transform);
        }

        if constexpr(false) { // Ambient pass.
            agl::engine::render(ambient_pass);
        }
        if constexpr(true) { // Blinn Phong pass.
            blinn_phong_pass.uniforms["light_position"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(light_position);
            blinn_phong_pass.uniforms["normal_transform"]
            = std::make_shared<eng::Uniform<agl::Mat4>>(normal_transform);
            blinn_phong_pass.uniforms["view_position"]
            = std::make_shared<eng::Uniform<agl::Vec3>>(view_position);
            agl::engine::render(blinn_phong_pass);
        }
    }
};

void throwing_main() {
    auto p = GltfProgram();
    run(p);
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
