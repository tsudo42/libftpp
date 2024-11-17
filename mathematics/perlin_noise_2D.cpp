#ifndef PERLIN_NOISE_2D_CPP
#define PERLIN_NOISE_2D_CPP

#include "mathematics/perlin_noise_2D.hpp"
#include "mathematics/ivector2.hpp"
#include <cmath>

PerlinNoise2D::PerlinNoise2D(unsigned long long seed) : seed_(seed)
{
    randgen_ = Random2DCoordinateGenerator(seed_);
}

float PerlinNoise2D::interpolate(const float &a, const float &b, const float &w) const
{
    if (w < 0.0f)
    {
        return a;
    }
    else if (w > 1.0f)
    {
        return b;
    }
    return (b - a) * w + a;
}

float PerlinNoise2D::smooth_step(const float &w) const
{
    return w * w * w * (w * (w * 6 - 15) + 10);
}

float PerlinNoise2D::dotGridGradient(const int &ix, const int &iy, const float &x, const float &y) const
{
    float rand_norm = (randgen_(ix, iy) & 0xFFFF) / static_cast<float>(0xFFFF);
    float angle = rand_norm * 2.0 * M_PI;
    IVector2<float> rand_grad(std::cos(angle), std::sin(angle));
    return rand_grad.dot(IVector2<float>(x - ix, y - iy));
}

float PerlinNoise2D::sample(const float &x, const float &y) const
{
    int x0 = std::floor(x);
    int x1 = x0 + 1;
    int y0 = std::floor(y);
    int y1 = y0 + 1;

    float sx = x - x0;
    float sy = y - y0;

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    float n2 = dotGridGradient(x0, y1, x, y);
    float n3 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n2, n3, sx);

    return interpolate(ix0, ix1, sy);
}

float PerlinNoise2D::operator()(const float &x, const float &y) const
{
    return sample(x, y);
}

#endif /* PERLIN_NOISE_2D_CPP */
