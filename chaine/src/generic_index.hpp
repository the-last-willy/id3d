#pragma once

template<typename T>
concept IndexTraits
= requires {
    T::Value;

    T::default_value;
};

template<IndexTraits Traits>
struct Index {
    using Value = typename Traits::Value;

    Value value = Traits::default_value;
    
    Index() = default;

    template<typename... Args> explicit
    Index(Args&&... args)
        : value(std::forward<Args>(args)...)
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
