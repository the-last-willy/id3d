#pragma once

#include <tuple>

template<typename Operator, typename... Operands>
struct Combination {
    Operator operator_;
    std::tuple<Operands...> operands;

    constexpr
    Combination(Operator o, Operands... os)
        : operator_(std::move(o))
        , operands(std::move(os)...)
    {}
};

template<typename Operator, typename... Operands>
Combination(Operator o, Operands... os) -> Combination<Operator, Operands...>;
