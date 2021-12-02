#pragma once

#include <agl/common/all.hpp>

#include <ranges>

template<typename T>
struct Field {
    agl::common::Interval<T> range;
    agl::common::Grid<T> values;
};

template<typename T>
auto field(std::size_t rx, std::size_t ry, T t = T()) {
    auto f = Field<T>();
    f.range = agl::common::interval(t);
    f.values = agl::common::grid<T>(rx, ry);
    for(auto& v : f.values) {
        v = t;
    }
    return f;
}

template<typename... F>
void update_range(Field<F...>& f) {
    f.range = agl::common::interval(at(f.values, 0));
    for(auto&& v : f.values | std::views::drop(1)) {
        extend(f.range, v);
    }
}

template<typename... F>
auto value_accessor(Field<F...>& f) {
    return [&](std::size_t i, std::size_t j) -> auto& {
        return at(f.values, i, j);
    };
}
