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
    void init() override {
        
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
