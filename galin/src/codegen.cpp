#include "scene/kame_house.hpp"

#include <fstream>
#include <iostream>
#include <set>

// struct Writer {
//     std::set<std::string> visited;

//     template<typename T>
//     void operator()(const T&) {
//         std::cout << "// Skipped " << typeid(T).name() << std::endl << std::endl;
//     }

//     template<typename... C>
//     void operator()(const Colored<C...>& c) {
//         operator()(c.any);
//     }

//     template<typename... C>
//     void operator()(const Combination<C...>& c) {
//         // Hardcoded arity.
//         if constexpr(std::tuple_size_v<decltype(c.operands)> == 0) {
//             static_assert(false, "Not implemented.");
//         } else if constexpr(std::tuple_size_v<decltype(c.operands)> == 1) {
//             operator()(std::get<0>(c.operands));
//         } else if constexpr(std::tuple_size_v<decltype(c.operands)> == 2) {
//             operator()(std::get<0>(c.operands));
//             operator()(std::get<1>(c.operands));
//         }
//     }

//     template<typename O>
//     void operator()(const Object<O>& o) {
//         operator()(o.content);
//         std::cout << colored_distance_glsl_function_definition(o) << std::endl;
//         std::cout << distance_glsl_function_definition(o) << std::endl;
//     }
// };

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
