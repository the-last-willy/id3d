// Disabled warnings.

#pragma warning(disable : 4005 4996)

// Local headers.

#include <agl/standard/all.hpp>
#include <id3d/common/all.hpp>

// External libraries.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <range/v3/view/indirect.hpp>
#include <range/v3/view/map.hpp>

// Standard library.

#include <iostream>
#include <numeric>
#include <random>

//

struct App : Program {
    eng::ShaderCompiler compiler;
    GLuint vao;
    GLuint input;
    GLuint output;
    GLuint program;
    GLuint texture;

    void init() override {
        auto tab = std::vector<int>(10);
        for(auto val : tab) {
            std::cout << val << std::endl;
        }

        glGenBuffers(1, &input);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, input);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * tab.size(), tab.data(), GL_STREAM_READ);

        glGenBuffers(1, &output);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, output);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * tab.size(), tab.data(), GL_STREAM_COPY);

        auto p = eng::Program();
        load(p, compiler, {{agl::compute_shader_tag,"iehl/src/application/compute/add.comp"}});
        bind(p);
        
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output);
        glDispatchCompute(tab.size() + 7, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glGetNamedBufferSubData(output, 0, sizeof(int) * tab.size(), tab.data());

        for(auto val : tab) {
            std::cout << val << std::endl;
        }
    }

    void update(float) override {
        
    }

    void render() override {

    }
};

void throwing_main() {
    auto p = App();
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
