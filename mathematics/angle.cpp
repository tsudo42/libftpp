#include "mathematics/angle.hpp"

#include <cmath>
#include <cfloat>

const double Angle::PI = 3.14159265358979323846;
const double Angle::DEGREE_TO_RADIAN = PI / 180.0;
const double Angle::RADIAN_TO_DEGREE = 180.0 / PI;

Angle::Angle(double value, bool is_degree)
    : value_(value), is_degree_(is_degree)
{
    normalize_internal();
}

double Angle::degrees() const
{
    return is_degree_ ? value_ : to_degrees(value_);
}

double Angle::radians() const
{
    return is_degree_ ? to_radians(value_) : value_;
}

void Angle::normalize()
{
    normalize_internal();
}

double Angle::to_radians(double degrees)
{
    return degrees * DEGREE_TO_RADIAN;
}

double Angle::to_degrees(double radians)
{
    return radians * RADIAN_TO_DEGREE;
}

Angle Angle::operator+(const Angle &other) const
{
    double result = degrees() + other.degrees();
    return Angle(result, true);
}

Angle Angle::operator-(const Angle &other) const
{
    double result = degrees() - other.degrees();
    return Angle(result, true);
}

Angle &Angle::operator+=(const Angle &other)
{
    value_ = degrees() + other.degrees();
    normalize_internal();
    return *this;
}

Angle &Angle::operator-=(const Angle &other)
{
    value_ = degrees() - other.degrees();
    normalize_internal();
    return *this;
}

bool Angle::operator==(const Angle &other) const
{
    return std::fabs(degrees() - other.degrees()) < DBL_EPSILON;
}

bool Angle::operator!=(const Angle &other) const
{
    return !(*this == other);
}

bool Angle::is_clockwise_to(const Angle &other) const
{
    return (other.degrees() - degrees()) > 0;
}

bool Angle::is_counterclockwise_to(const Angle &other) const
{
    return (degrees() - other.degrees()) > 0;
}

double Angle::distance_to(const Angle &other) const
{
    double diff = std::fabs(degrees() - other.degrees());
    return (diff > 180.0) ? 360.0 - diff : diff;
}

Angle Angle::complement() const
{
    return Angle(360.0 - degrees(), true);
}

Angle Angle::operator+(double delta) const
{
    return Angle(degrees() + delta, true);
}

Angle Angle::operator-(double delta) const
{
    return Angle(degrees() - delta, true);
}

bool Angle::is_within(const Angle &start, const Angle &end) const
{
    double angle_deg = degrees();
    double start_deg = start.degrees();
    double end_deg = end.degrees();

    if (start_deg < end_deg)
        return angle_deg >= start_deg && angle_deg <= end_deg;
    else
        return angle_deg >= start_deg || angle_deg <= end_deg;
}

bool Angle::is_positive() const
{
    return degrees() > 0;
}

bool Angle::is_negative() const
{
    return degrees() < 0;
}

double Angle::get(bool in_degrees) const
{
    return in_degrees ? degrees() : radians();
}

void Angle::convert_to_degrees()
{
    if (!is_degree_)
    {
        value_ = to_degrees(value_);
        is_degree_ = true;
    }
}

void Angle::convert_to_radians()
{
    if (is_degree_)
    {
        value_ = to_radians(value_);
        is_degree_ = false;
    }
}

void Angle::wrap_around(double lower_bound, double upper_bound)
{
    double range = upper_bound - lower_bound;
    while (value_ < lower_bound)
        value_ += range;
    while (value_ >= upper_bound)
        value_ -= range;
}

void Angle::normalize_internal()
{
    if (is_degree_)
    {
        wrap_around(0.0, 360.0);
    }
    else
    {
        wrap_around(0.0, 2.0 * PI);
    }
}

std::ostream &operator<<(std::ostream &os, const Angle &angle)
{
    if (angle.is_degree_)
        os << angle.degrees() << "°";
    else
        os << angle.radians() << " rad";
    return os;
}
