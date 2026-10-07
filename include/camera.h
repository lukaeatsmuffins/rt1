#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "util.h"
#include "material.h"

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
                    clr += ray_color(r, max_depth, world);
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

    void set_max_depth(int depth) {
        if (depth < 1) {
            std::cerr << "Max depth must be greater than 0.\n";
            return;
        }
        max_depth = depth;
    }

    void set_vfov(double vfov_degrees) {
        if (vfov_degrees <= 0 || vfov_degrees >= 180) {
            std::cerr << "Vertical field of view must be in (0, 180) degrees.\n";
            return;
        }
        vfov = vfov_degrees;
    }

    void set_lookfrom(const point3& lookfrom_point) {
        lookfrom = lookfrom_point;
    }

    void set_lookat(const point3& lookat_point) {
        lookat = lookat_point;
    }

    void set_vup(const vec3& vup_vector) {
        vup = vup_vector;
    }

    void set_defocus_angle(double angle) {
        defocus_angle = angle;
    }

    void set_focus_distance(double distance) {
        focus_distance = distance;
    }

  private:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    int max_depth = 10;

    int image_height;
    double pixel_sample_scale;

    double vfov = 90.0;
    point3 lookfrom = point3(0,0,0);   // Point camera is looking from
    point3 lookat   = point3(0,0,-1);  // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    double defocus_angle = 0.0;
    double focus_distance = 10;

    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    vec3 u, v, w;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        int image_height_tmp = static_cast<int>(image_width / aspect_ratio);
        image_height = image_height_tmp < 1 ? 1 : image_height_tmp;

        pixel_sample_scale = 1.0 / static_cast<double>(samples_per_pixel);

        center = lookfrom;

        auto h = std::tan(degrees_to_radians(vfov) / 2.0);
        const double viewport_height = 2.0 * h * focus_distance;
        const double viewport_width = viewport_height * static_cast<double>(image_width) / static_cast<double>(image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w)); 
        v = cross(w, u);

        const vec3 viewport_u = viewport_width * u;
        const vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        const auto upper_left_corner = center - viewport_u / 2 - viewport_v / 2 - focus_distance * w;
        pixel00_loc = upper_left_corner + pixel_delta_u / 2 + pixel_delta_v / 2;
        
        auto defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {

        // Return black if we hit nothing.
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            color attenuation;
            ray scattered;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return color(0, 0, 0);
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
        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel00_loc
                                 + (pixel_delta_u * (i + offset.x()))
                                 + (pixel_delta_v * (j + offset.y()))
                                 - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    // Returns a random point in the camera defocus disk.
    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 sample_square() const {
        return vec3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0);
    }
};

#endif
