#pragma once

#include <array>

template<typename Any>
struct Colored {
    Any any;

    std::array<float, 3> color;
};

template<typename Any>
Colored(Any, std::array<float, 3>) -> Colored<Any>;
