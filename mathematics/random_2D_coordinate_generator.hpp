#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

class Random2DCoordinateGenerator
{
public:
    Random2DCoordinateGenerator(long long seed_ = 0);

    long long operator()(const long long &x, const long long &y) const;

    long long seed() const;
    void set_seed(const long long &seed);

private:
    long long seed_;
};

#endif /* RANDOM_2D_COORDINATE_GENERATOR_HPP */
