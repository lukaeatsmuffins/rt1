#include "../include/util.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/camera.h"


int main() {

    // Image.
    const int image_width = 400;
    const double aspect_ratio = 16.0 / 9.0;
    const int samples_per_pixel = 100;


    // World.
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera.
    camera cam;
    cam.set_image_width(image_width);
    cam.set_aspect_ratio(aspect_ratio);
    cam.set_samples_per_pixel(samples_per_pixel);
    cam.render(world);
}
