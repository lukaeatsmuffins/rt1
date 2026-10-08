#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere(const point3& center, double radius, shared_ptr<material> mat_ptr)
        : center(center, point3(0,0,0)), radius(std::fmax(0,radius)), mat_ptr(mat_ptr) {}

    sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat_ptr)
        : center(center1, center2-center1), radius(std::fmax(0,radius)), mat_ptr(mat_ptr) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Time is between 0 and 1.
        point3 pos = center.at(r.time()); 
        vec3 oc = pos - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        auto outward_normal = (rec.p - pos) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }

  private:
    ray center;
    double radius;
    shared_ptr<material> mat_ptr;
};

#endif