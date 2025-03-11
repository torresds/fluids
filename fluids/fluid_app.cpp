#include "fluid_app.h"
#include <cstdlib>
#include <ctime>

FluidApp* FluidApp::instance = nullptr;

FluidApp::FluidApp(int argc, char** argv)
    : fluid(0.1f, 0.0000f, 0.001f),
    winWidth(512),
    winHeight(512),
    mouseX(0),
    mouseY(0),
    mouseDown(false),
    lastWinX(0),
    lastWinY(0) {
    initializeWindow(argc, argv);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void FluidApp::initializeWindow(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("fluids");
    lastWinX = glutGet(GLUT_WINDOW_X);
    lastWinY = glutGet(GLUT_WINDOW_Y);
}

void FluidApp::run() {
    instance = this;
    FluidRenderer::initialize();
    registerCallbacks();
    glutMainLoop();
}

void FluidApp::registerCallbacks() {
    glutDisplayFunc(displayCallback);
    glutIdleFunc(idleCallback);
    glutReshapeFunc(reshapeCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
}

void FluidApp::update() {
    handleWindowMovement();
    handleMouseInput();
    fluid.step();
    glutPostRedisplay();
}

void FluidApp::handleWindowMovement() {
    const int currentX = glutGet(GLUT_WINDOW_X);
    const int currentY = glutGet(GLUT_WINDOW_Y);
    const int dx = currentX - lastWinX;
    const int dy = currentY - lastWinY;

    if (dx != 0 || dy != 0) {
        constexpr float shakeFactor = 0.005f;
        for (int i = 1; i <= FluidSimulator::N; ++i) {
            for (int j = 1; j <= FluidSimulator::N; ++j) {
                fluid.Vx[fluid.IX(i, j)] += shakeFactor * dx;
                fluid.Vy[fluid.IX(i, j)] += shakeFactor * dy;
            }
        }
        lastWinX = currentX;
        lastWinY = currentY;
    }
}

void FluidApp::handleMouseInput() {
    if (mouseDown && winWidth > 1 && winHeight > 1) {
        const float fx = mouseX / static_cast<float>(winWidth - 1);
        const float fy = (winHeight - mouseY - 1) / static_cast<float>(winHeight - 1);

        const int i = static_cast<int>(fx * (FluidSimulator::N - 1)) + 1;
        const int j = static_cast<int>(fy * (FluidSimulator::N - 1)) + 1;

        if (i >= 1 && i <= FluidSimulator::N &&
            j >= 1 && j <= FluidSimulator::N) {

            const int idx = fluid.IX(i, j);
            fluid.density[idx] += 150.0f;
            fluid.Vx[idx] += 40.0f * (std::rand() % 100 / 50.0f - 1.0f);
            fluid.Vy[idx] += 40.0f * (std::rand() % 100 / 50.0f - 1.0f);
        }
    }
}

void FluidApp::displayCallback() {
    FluidRenderer::render(instance->fluid);
}

void FluidApp::idleCallback() {
    instance->update();
}

void FluidApp::reshapeCallback(int width, int height) {
    instance->winWidth = width;
    instance->winHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void FluidApp::mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        instance->mouseDown = (state == GLUT_DOWN);
        instance->mouseX = x;
        instance->mouseY = y;
    }
}

void FluidApp::motionCallback(int x, int y) {
    instance->mouseX = x;
    instance->mouseY = y;
}