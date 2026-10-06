#include <iostream>

#include "../include/color.h"
#include "../include/ray.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    auto oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        // Return the nearest root that lies in the acceptable range.
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r) {

    const color white(1, 1, 1);
    const color blue(0.5, 0.7, 1);
    const color red(1, 0, 0);
 
    const vec3 unit_direction = r.direction() / r.direction().length();
    const double a = 0.5 * (unit_direction.y() + 1.0);

    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);

    if (t > 0) {
        vec3 n = unit_vector((r.at(t) - point3(0, 0, -1)));
        return 0.5 * color(n.x() + 1, n.y() + 1, n.z() + 1);
    }

    // Blue background.
    return (1-a) * white + a * blue;
}

int main() {

    // Image.
    const int image_width = 400;
    const double aspect_ratio = 16.0 / 9.0;

    int image_height_tmp = static_cast<int>(image_width / aspect_ratio);
    const int image_height = image_height_tmp < 1 ? 1 : image_height_tmp;

    // Camera.
    const double viewport_height = 2.0;
    const double viewport_width = viewport_height * static_cast<double>(image_width) / static_cast<double>(image_height);
    const double focal_length = 1.0;

    const point3 origin(0, 0, 0);

    const vec3 viewport_u(viewport_width, 0, 0);
    const vec3 viewport_v(0, -viewport_height, 0);

    const auto pixel_delta_u = viewport_u / image_width;
    const auto pixel_delta_v = viewport_v / image_height;

    const auto upper_left_corner = origin - viewport_u / 2 - viewport_v / 2 - vec3(0, 0, focal_length);
    const auto point00_loc = upper_left_corner + pixel_delta_u / 2 + pixel_delta_v / 2;

    // Render.
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            auto ray_direction = point00_loc + (pixel_delta_u * i) + (pixel_delta_v * j) - origin;
            ray r(origin, ray_direction);
            auto clr = ray_color(r);
            write_color(std::cout, clr);
        }
    }
    std::clog << "\rDone.                 \n";
}
