#pragma once

#include <string>

template<typename Content>
struct Object {
    std::string name;

    Content content;
};

template<typename Content>
Object(std::string, Content) -> Object<Content>;
