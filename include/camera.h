#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "util.h"

class camera {
  public:
    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            for (int i = 0; i < image_width; i++) {
                auto clr = color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; s++) {
                    ray r = get_ray(i, j);
                    clr += ray_color(r, world);
                }
                write_color(std::cout, clr * pixel_sample_scale);
            }
        }
    std::clog << "\rDone.                 \n";
    }

    void set_image_width(int width) {
        if (width < 1) {
            std::cerr << "Image width must be greater than 0.\n";
            return;
        }
        image_width = width;
    }

    void set_aspect_ratio(double ratio) {
        if (ratio <= 0) {
            std::cerr << "Aspect ratio must be greater than 0.\n";
            return;
        }
        aspect_ratio = ratio;
    }

    void set_samples_per_pixel(int samples) {
        if (samples < 1) {
            std::cerr << "Samples per pixel must be greater than 0.\n";
            return;
        }
        samples_per_pixel = samples;
    }


  private:
    int image_width = 400;
    double aspect_ratio = 16.0 / 9.0;
    int image_height;
    int samples_per_pixel = 10;
    double pixel_sample_scale;
    point3 center;
    point3 pixel00_loc;
    point3 pixel_delta_u;
    point3 pixel_delta_v;

    void initialize() {
        int image_height_tmp = static_cast<int>(image_width / aspect_ratio);
        image_height = image_height_tmp < 1 ? 1 : image_height_tmp;

        pixel_sample_scale = 1.0 / static_cast<double>(samples_per_pixel);

        const double focal_length = 1.0;
        const double viewport_height = 2.0;
        const double viewport_width = viewport_height * static_cast<double>(image_width) / static_cast<double>(image_height);

        center = point3(0, 0, 0);

        const vec3 viewport_u(viewport_width, 0, 0);
        const vec3 viewport_v(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        const auto upper_left_corner = center - viewport_u / 2 - viewport_v / 2 - vec3(0, 0, focal_length);
        pixel00_loc = upper_left_corner + pixel_delta_u / 2 + pixel_delta_v / 2;
    }

    color ray_color(const ray& r, const hittable& world) const {

        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }
    
        const vec3 unit_direction = r.direction() / r.direction().length();
        const double a = 0.5 * (unit_direction.y() + 1.0);

        // Blue background.
        const color white(1, 1, 1);
        const color blue(0.5, 0.7, 1);

        return (1-a) * white + a * blue;
    }

    ray get_ray(int i, int j) const {

        auto offset = sample_square();
        auto ray_direction = pixel00_loc
                                 + (pixel_delta_u * (i + offset.x()))
                                 + (pixel_delta_v * (j + offset.y()))
                                 - center;
        return ray(center, ray_direction);
    }

    vec3 sample_square() const {
        return vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0);
    }
};

#endif