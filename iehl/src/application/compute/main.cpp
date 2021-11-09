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

    void init() override {
        auto v = std::vector<int>(10);
        std::iota(begin(v), end(v), 0);
        for(auto e : v) {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        auto p = eng::Program();
        load(p, compiler, {{
            agl::compute_shader_tag,
            "iehl/src/application/compute/filter.comp"
        }});
        bind(p);

        auto input_b = create(agl::buffer_tag);
        storage(input_b, std::span(v));

        auto output_b = create(agl::buffer_tag);
        storage<int>(output_b, size(v));

        auto counter_b = create(agl::buffer_tag);
        storage<int>(counter_b, 1);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, input_b);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, output_b);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, counter_b);
        // glUniform1i(0, 12);

        glDispatchCompute((size(v) + 7) / 8, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glGetNamedBufferSubData(output_b, 0, agl::standard::byte_size(v), data(v));

        auto count = 0;
        glGetNamedBufferSubData(counter_b, 0, 4, &count);
        
        std::cout << "Count: " << count << std::endl;
        for(auto e : v) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
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
