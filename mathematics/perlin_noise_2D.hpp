#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include "mathematics/random_2D_coordinate_generator.hpp"

class PerlinNoise2D
{
public:
    PerlinNoise2D(unsigned long long seed = 0);
    virtual ~PerlinNoise2D() = default;

    float sample(const float &x, const float &y) const;
    float operator()(const float &x, const float &y) const;

protected:
    virtual float smooth_step(const float &w) const;

private:
    float interpolate(const float &a, const float &b, const float &x) const;
    float dotGridGradient(const int &ix, const int &iy, const float &x, const float &y) const;

    unsigned long long seed_;
    Random2DCoordinateGenerator randgen_;
};

#endif /* PERLIN_NOISE_2D_HPP */
