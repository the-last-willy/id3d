#include "scene/kame_house.hpp"
#include "tree2/all.hpp"

#include <iostream>
#include <set>

// struct Writer {
//     std::set<std::string> visited;

//     void operator()(const Node& n) {
//         if(not visited.contains(n.name())) {
//             visited.insert(n.name());
//             for(auto&& child : n) {
//                 operator()(*child);
//             }
//             std::cout << n.glsl() << std::endl;
//         }
//     }
// };

void throwing_main() {
    auto p1 = Point{{1.f, 2.f, 3.f}, 1.f};
    auto p2 = Point{{2.f, 4.f, 6.f}, 2.f};
    auto u = Union(p1, p2);
    auto o = Object("obj", std::move(u));
    std::cout << glsl_distance_fn(o) << std::endl;
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
