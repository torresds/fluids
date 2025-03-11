#pragma once
#include <GL/glut.h>
#include "fluid_simulator.h"
#include "fluid_renderer.h"

class FluidApp {
public:
    static FluidApp* instance;

    FluidApp(int argc, char** argv);
    void run();

private:
    FluidSimulator fluid;
    int winWidth;
    int winHeight;
    int mouseX;
    int mouseY;
    bool mouseDown;
    int lastWinX;
    int lastWinY;

    void initializeWindow(int argc, char** argv);
    void registerCallbacks();
    void update();
    void handleWindowMovement();
    void handleMouseInput();

    static void displayCallback();
    static void idleCallback();
    static void reshapeCallback(int width, int height);
    static void mouseCallback(int button, int state, int x, int y);
    static void motionCallback(int x, int y);
};