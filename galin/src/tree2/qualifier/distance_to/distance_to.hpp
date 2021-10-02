#pragma once

#include "tree2/node/node.hpp"

template<typename Child>
struct DistanceTo : Node<Child> {};

template<typename Child>
DistanceTo(Child) -> DistanceTo<Child>;
