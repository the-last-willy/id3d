#pragma once

#include "grid.hpp"



struct GridLights {
    struct Span {
        GLuint offset;
        GLuint count;
    };

    std::vector<GLuint> indices;
    std::vector<Span> spans;

    agl::Buffer light_index_ssbo;
    agl::Buffer light_span_ssbo;
};

inline
auto lights(const Grid& g, std::span<const LightProperties> lights) {
    auto gls = GridLights();
    auto offset = GLuint(0);
    for(auto& c : g.cells) {
        if(not is_empty(c)) {
            auto count = GLuint(0);
            for(GLuint li = 0; li < size(lights); ++li) {
                auto& l = lights[li];
                auto d = length(l.position.xyz() - clamp(c.bounds, l.position.xyz()));
                if(d < 0.05f) {
                    count += 1;
                    gls.indices.emplace_back(li);
                }
            }
            gls.spans.emplace_back(GridLights::Span{offset, count});
            offset += count;
        }
    }
    {
        gls.light_span_ssbo = agl::create(agl::buffer_tag);
        storage(gls.light_span_ssbo, std::span(gls.spans));
    }
    {
        gls.light_index_ssbo = agl::create(agl::buffer_tag);
        storage(gls.light_index_ssbo, std::span(gls.indices));
    }
    return gls;
}
