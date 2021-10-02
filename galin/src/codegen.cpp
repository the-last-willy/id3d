#include "scene/kame_house.hpp"
#include "tree2/all.hpp"

#include <iostream>
#include <set>

struct Writer {
    std::set<std::string> visited;

    template<typename... N, typename... Ts>
    void operator()(const Node<N...>& n, const Ts&... ts) {
        if constexpr(sizeof...(N) > 0) {
            auto f = [this](const auto&... args) { this->operator()(args...); };
            std::apply(f, n.children);
        } else {
            (void) n;
        }
        if constexpr(sizeof...(Ts) > 0) {
            operator()(ts...);
        }
    }

    template<typename... O, typename... Ts>
    void operator()(const Object<O...>& o, const Ts&... ts) {
        operator()(static_cast<const Node<O...>&>(o));
        std::cout << glsl_distance_function_definition(o) << std::endl;
        if constexpr(sizeof...(Ts) > 0) {
            operator()(ts...);
        }
    }
};

void throwing_main() {
    auto scene = kame_house::kame_house();
    auto writer = Writer();
    writer(scene);
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
