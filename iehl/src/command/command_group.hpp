#pragma once

#include "opengl.hpp"

#include <vector>

struct CommandGroup {
    // 'count_buffer' and 'commands_buffer'
    // should be in the same buffer
    // so it can be use with 'MultiDrawElementsIndrectCount'.

    // CPU / GPU updates ?
    // Persitent mapping ?

    std::vector<gl::DrawElementsIndirectCommand> commands;

    gl::Buffer count_buffer;
    gl::Buffer commands_buffer;

    CommandGroup() {
        gl::NamedBufferStorage(count_buffer,
            sizeof(GLuint), GL_DYNAMIC_STORAGE_BIT);
    }

    CommandGroup(GLsizei capacity)
        : CommandGroup()
    {
        commands.reserve(capacity);
    }

    const gl::DrawElementsIndirectCommand&
    operator[](std::size_t i) const {
        return commands[i];
    }

    gl::DrawElementsIndirectCommand&
    operator[](std::size_t i) {
        return commands[i];
    }
};

inline
GLsizei size(const CommandGroup& cg) {
    return GLsizei(size(cg.commands));
}
