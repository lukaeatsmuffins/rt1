#include <iostream>
#include "../include/util.h"
#include "../include/hittable.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"


color ray_color(const ray& r, const hittable& world) {

    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
 
    const vec3 unit_direction = r.direction() / r.direction().length();
    const double a = 0.5 * (unit_direction.y() + 1.0);

    // Blue background.
    const color white(1, 1, 1);
    const color blue(0.5, 0.7, 1);

    return (1-a) * white + a * blue;
}

int main() {

    // Image.
    const int image_width = 400;
    const double aspect_ratio = 16.0 / 9.0;

    int image_height_tmp = static_cast<int>(image_width / aspect_ratio);
    const int image_height = image_height_tmp < 1 ? 1 : image_height_tmp;

    // World.
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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
            auto clr = ray_color(r, world);
            write_color(std::cout, clr);
        }
    }
    std::clog << "\rDone.                 \n";
}
