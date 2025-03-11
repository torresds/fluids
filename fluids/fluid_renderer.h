#pragma once
#include <GL/glut.h>
#include "fluid_simulator.h"

class FluidRenderer {
public:
    static void initialize();
    static void render(const FluidSimulator& fluid);
};