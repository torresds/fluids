#include "fluid_renderer.h"

void FluidRenderer::initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FluidRenderer::render(const FluidSimulator& fluid) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);

    const float cellSize = 1.0f / FluidSimulator::N;
    for (int i = 0; i <= FluidSimulator::N; ++i) {
        for (int j = 0; j <= FluidSimulator::N; ++j) {
            const float d = fluid.density[fluid.IX(i, j)];
            glColor3f(d, d, d);

            const float x = (i - 0.5f) / FluidSimulator::N;
            const float y = (j - 0.5f) / FluidSimulator::N;

            glVertex2f(x, y);
            glVertex2f(x + cellSize, y);
            glVertex2f(x + cellSize, y + cellSize);
            glVertex2f(x, y + cellSize);
        }
    }
    glEnd();
    glutSwapBuffers();
}