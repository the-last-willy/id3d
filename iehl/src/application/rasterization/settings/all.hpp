#pragma once

#include "debugging.hpp"
#include "frustrum_culling.hpp"
#include "optimization.hpp"
#include "ui.hpp"

struct AllSettings {
    DebuggingSettings debugging;
    FrustrumCullingSettings frustrum_culling;
    OptimizationSettings optimization;
    UiSettings ui;
};
