#pragma once
#include <vector>

class FluidSimulator {
public:
    static const int N = 70;
    FluidSimulator(float dt, float diff, float visc);
    void step();

    std::vector<float> s, density, Vx, Vy, Vx0, Vy0;
    float dt, diff, visc;

    inline int IX(int i, int j) const { return i + (N + 2) * j; }

private:
    static const int GRID_SIZE = N + 2;
    static const int ARRAY_SIZE = GRID_SIZE * GRID_SIZE;

    void diffuse(int b, std::vector<float>& x, const std::vector<float>& x0, float factor);
    void advect(int b, std::vector<float>& d, const std::vector<float>& d0,
        const std::vector<float>& u, const std::vector<float>& v);
    void project(std::vector<float>& u, std::vector<float>& v,
        std::vector<float>& p, std::vector<float>& div);
    void linSolve(int b, std::vector<float>& x, const std::vector<float>& x0,
        float a, float c);
    void setBoundary(int b, std::vector<float>& x);

    void diffuseVelocity();
    void projectVelocity();
    void advectVelocity();
    void projectFinalVelocity();
    void diffuseDensity();
    void advectDensity();
};