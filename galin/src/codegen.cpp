#include "scene/kame_house.hpp"

#include <fstream>
#include <iostream>
#include <set>

void throwing_main() {
    auto o = std::ofstream("galin/src/shader/kame_house_gen.glsl");
    if(not o.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    auto s = kame_house::scene();
    s->dump_sdf_only(o);
    s->dump_sdf_and_material(o);
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
