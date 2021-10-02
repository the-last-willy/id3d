#pragma once

#include "tree2/node/node.hpp"

#include <sstream>

struct AnyObject {
    std::string name_;

    AnyObject(std::string&& name)
        : name_(std::move(name))
    {} 

    virtual ~AnyObject() {};

    std::string name() const {
        return name_;
    }
};

template<typename Distance, typename Color>
struct Object : AnyObject, Node<Distance, Color> {
    Object(std::string name, Distance d, Color c)
        : AnyObject(std::move(name))
        , Node<Distance, Color>(std::move(d), std::move(c))
    {}
};

template<typename D, typename C>
Object(std::string, D, C) -> Object<D, C>;
