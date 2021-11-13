#pragma once

template<typename Distance, typename Color>
struct Operator {
    Distance distance;
    Color color;
};

template<typename Distance, typename Color>
Operator(Distance, Color) -> Operator<Distance, Color>;
