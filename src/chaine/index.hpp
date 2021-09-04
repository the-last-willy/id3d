#pragma once

namespace chaine {

template<typename T>
concept IndexTraits
= requires {
    IndexTraits::Value;

    IndexTraits::default_value;
};

template<typename Traits>
struct Index {
    using Value = Traits::Value;

    Value value = Traits::default_value;
    
    Index() = default;

    explicit
    Index(Value v)
        : value(v)
    {}

    operator const Value&() const noexcept {
        return value;
    }
};

template<typename I> constexpr
bool operator==(Index<I> i0, Index<I> i1) {
    return i0.value == i1.value;
}

template<typename I> constexpr
auto operator<=>(Index<I> i0, Index<I> i1) {
    return i0.value <=> i1.value;
}

}
