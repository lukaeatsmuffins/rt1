#include "../include/color.h"
#include "../include/interval.h"


void write_color(std::ostream &out, const color &pixel_color) {

    auto x = pixel_color.x();
    auto y = pixel_color.y();
    auto z = pixel_color.z();

    static const interval intensity(0.0, 0.999);
    x = intensity.clamp(x);
    y = intensity.clamp(y);
    z = intensity.clamp(z);

    out << static_cast<int>(256 * pixel_color.x()) << ' '
        << static_cast<int>(256 * pixel_color.y()) << ' '
        << static_cast<int>(256 * pixel_color.z()) << '\n';
}
