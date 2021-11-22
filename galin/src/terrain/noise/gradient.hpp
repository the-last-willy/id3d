#pragma once

#include <agl/engine/all.hpp>

inline
agl::Vec2 gradient_noise_hash(agl::Vec2 v) {
    auto k = agl::vec2(0.3183099f, 0.3678794f);
    v = v * k + agl::vec2(k[1], k[0]);
    return -agl::vec2(1.f) + 2.f * agl::fract(16.f * k * agl::fract(v[0] * v[1] *(v[0] + v[1])));
}

inline
float gradient_noise(agl::Vec2 p) {
    using namespace agl;

    auto i = floor(p);
    auto f = fract(p);
	
	// auto u = f * f * (vec2(3.f) - 2.f * f);
	auto u = f * f * f * (vec2(10.f) + f * (-vec2(15.f) + f * 6.f));

    return mix( 
        mix(
            dot(
                gradient_noise_hash(i + vec2(0.f, 0.f)), 
                f - vec2(0.f, 0.f)), 
            dot(
                gradient_noise_hash(i + vec2(1.f, 0.f)), 
                f - vec2(1.f, 0.f)), 
            u[0]),
        mix(
            dot(
                gradient_noise_hash(i + vec2(0.f, 1.f)), 
                f - vec2(0.f, 1.f)), 
            dot(
                gradient_noise_hash(i + vec2(1.f, 1.f)), 
                f - vec2(1.f, 1.f)),
            u[0]), 
        u[1]);
}
