#include "../include/color.h"
#include "../include/interval.h"

double linear_to_gamma(double linear_component) {
    return std::sqrt(linear_component);
}

void write_color(std::ostream &out, const color &pixel_color) {

    auto x = pixel_color.x();
    auto y = pixel_color.y();
    auto z = pixel_color.z();

    x = linear_to_gamma(x);
    y = linear_to_gamma(y);
    z = linear_to_gamma(z);

    static const interval intensity(0.0, 0.999);
    x = intensity.clamp(x);
    y = intensity.clamp(y);
    z = intensity.clamp(z);

    out << static_cast<int>(256 * x) << ' '
        << static_cast<int>(256 * y) << ' '
        << static_cast<int>(256 * z) << '\n';
}
