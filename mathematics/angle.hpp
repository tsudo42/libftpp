#ifndef ANGLE_HPP
#define ANGLE_HPP

#include <ostream>

class Angle
{
public:
    Angle(double value = 0, bool is_degree = true);

    double degrees() const;
    double radians() const;

    void normalize();

    Angle operator+(const Angle &other) const;
    Angle operator-(const Angle &other) const;
    Angle &operator+=(const Angle &other);
    Angle &operator-=(const Angle &other);
    bool operator==(const Angle &other) const;
    bool operator!=(const Angle &other) const;

    static double to_radians(double degrees);
    static double to_degrees(double radians);

    bool is_clockwise_to(const Angle &other) const;
    bool is_counterclockwise_to(const Angle &other) const;

    double distance_to(const Angle &other) const;

    Angle complement() const;

    Angle operator+(double delta) const;
    Angle operator-(double delta) const;

    bool is_within(const Angle &start, const Angle &end) const;

    bool is_positive() const;
    bool is_negative() const;

    double get(bool in_degrees) const;

    void convert_to_degrees();
    void convert_to_radians();

    void wrap_around(double lower_bound, double upper_bound);

    static const double PI;
    static const double DEGREE_TO_RADIAN;
    static const double RADIAN_TO_DEGREE;

    friend std::ostream &operator<<(std::ostream &os, const Angle &angle);

private:
    double value_;
    bool is_degree_;
    void normalize_internal();
};

#endif /* ANGLE_HPP */
