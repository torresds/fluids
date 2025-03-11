#include "fluid_simulator.h"
#include <cmath>

FluidSimulator::FluidSimulator(float dt, float diff, float visc)
    : dt(dt), diff(diff), visc(visc) {
    s.resize(ARRAY_SIZE);
    density.resize(ARRAY_SIZE);
    Vx.resize(ARRAY_SIZE);
    Vy.resize(ARRAY_SIZE);
    Vx0.resize(ARRAY_SIZE);
    Vy0.resize(ARRAY_SIZE);
}

void FluidSimulator::step() {
    diffuseVelocity();
    projectVelocity();
    advectVelocity();
    projectFinalVelocity();
    diffuseDensity();
    advectDensity();
}

void FluidSimulator::diffuse(int b, std::vector<float>& x, const std::vector<float>& x0, float factor) {
    float a = dt * factor * N * N;
    linSolve(b, x, x0, a, 1 + 4 * a);
}

void  FluidSimulator::advect(int b, std::vector<float>& d, const std::vector<float>& d0,
    const std::vector<float>& u, const std::vector<float>& v) {
    const float dt0 = dt * N;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            const int idx = IX(i, j);
            float x = i - dt0 * u[idx];
            float y = j - dt0 * v[idx];

            x = std::max(0.5f, std::min(N + 0.5f, x));
            y = std::max(0.5f, std::min(N + 0.5f, y));

            const int i0 = static_cast<int>(x);
            const int j0 = static_cast<int>(y);
            const int i1 = i0 + 1;
            const int j1 = j0 + 1;

            const float s1 = x - i0;
            const float s0 = 1 - s1;
            const float t1 = y - j0;
            const float t0 = 1 - t1;

            d[idx] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    setBoundary(b, d);
}

void  FluidSimulator::project(std::vector<float>& u, std::vector<float>& v,
    std::vector<float>& p, std::vector<float>& div) {
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            div[IX(i, j)] = -0.5f * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
                v[IX(i, j + 1)] - v[IX(i, j - 1)]) / N;
            p[IX(i, j)] = 0;
        }
    }
    setBoundary(0, div);
    setBoundary(0, p);
    linSolve(0, p, div, 1, 4);

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
        }
    }
    setBoundary(1, u);
    setBoundary(2, v);
}

void  FluidSimulator::linSolve(int b, std::vector<float>& x, const std::vector<float>& x0,
    float a, float c) {
    for (int k = 0; k < 20; ++k) {
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                    x[IX(i, j - 1)] + x[IX(i, j + 1)])) / c;
            }
        }
        setBoundary(b, x);
    }
}

void FluidSimulator::setBoundary(int b, std::vector<float>& x) {
    for (int i = 1; i <= N; ++i) {
        x[IX(0, i)] = (b == 1) ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N + 1, i)] = (b == 1) ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i, 0)] = (b == 2) ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N + 1)] = (b == 2) ? -x[IX(i, N)] : x[IX(i, N)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
    x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void  FluidSimulator::diffuseVelocity() {
    diffuse(1, Vx0, Vx, visc);
    diffuse(2, Vy0, Vy, visc);
}

void FluidSimulator::projectVelocity() {
    project(Vx0, Vy0, Vx, Vy);
}

void  FluidSimulator::advectVelocity() {
    advect(1, Vx, Vx0, Vx0, Vy0);
    advect(2, Vy, Vy0, Vx0, Vy0);
}

void  FluidSimulator::projectFinalVelocity() {
    project(Vx, Vy, Vx0, Vy0);
}

void  FluidSimulator::diffuseDensity() {
    diffuse(0, s, density, diff);
}

void  FluidSimulator::advectDensity() {
    advect(0, density, s, Vx, Vy);
}