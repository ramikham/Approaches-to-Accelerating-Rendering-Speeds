//
// Created by Rami on 1/29/2024.
//

#ifndef CUDA_RAY_TRACER_ROTATE_X_H
#define CUDA_RAY_TRACER_ROTATE_X_H

#include "../../Primitives/Primitive.h"

class Rotate_X : public Primitive {
public:
    Rotate_X(std::shared_ptr<Primitive> primitive_ptr, double rotation_angle)
            : primitive_ptr(primitive_ptr)
    {
        double radians = degrees_to_radians(rotation_angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        has_bbox = primitive_ptr->has_bounding_box(0, 1, bbox);

        point3D min(infinity, infinity, infinity);
        point3D max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.get_max().x() + (1 - i) * bbox.get_min().x();
                    auto y = j * bbox.get_max().y() + (1 - j) * bbox.get_min().y();
                    auto z = k * bbox.get_max().z() + (1 - k) * bbox.get_min().z();

                    auto newy = cos_theta * y - sin_theta * z;
                    auto newz = sin_theta * y + cos_theta * z;

                    Vec3D tester(x, newy, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = Axis_Aligned_Bounding_Box(min, max);
    }

    bool intersection(const Ray& r, double t_0, double t_1, Intersection_Information& intersection_info) const override
    {
        point3D origin = r.get_ray_origin();
        Vec3D direction = r.get_ray_direction();

        origin[1] = cos_theta * r.get_ray_origin()[1] - sin_theta * r.get_ray_origin()[2];
        origin[2] = sin_theta * r.get_ray_origin()[1] + cos_theta * r.get_ray_origin()[2];

        direction[1] = cos_theta * r.get_ray_direction()[1] - sin_theta * r.get_ray_direction()[2];
        direction[2] = sin_theta * r.get_ray_direction()[1] + cos_theta * r.get_ray_direction()[2];

        Ray rotated_ray(origin, direction, r.get_time());

        if (!primitive_ptr->intersection(rotated_ray, t_0, t_1, intersection_info))
            return false;

        point3D p = intersection_info.p;
        Vec3D normal = intersection_info.normal;

        p[1] = cos_theta * intersection_info.p[1] - sin_theta * intersection_info.p[2];
        p[2] = sin_theta * intersection_info.p[1] + cos_theta * intersection_info.p[2];

        normal[1] = cos_theta * intersection_info.normal[1] - sin_theta * intersection_info.normal[2];
        normal[2] = sin_theta * intersection_info.normal[1] + cos_theta * intersection_info.normal[2];

        intersection_info.p = p;
        intersection_info.normal = normal;

        return true;
    }

    bool has_bounding_box(double time_0, double time_1, Axis_Aligned_Bounding_Box& surrounding_AABB) const override
    {
        surrounding_AABB = bbox;
        return true;
    }

private:
    std::shared_ptr<Primitive> primitive_ptr;
    double sin_theta;
    double cos_theta;
    bool has_bbox;
    Axis_Aligned_Bounding_Box bbox;
};


#endif //CUDA_RAY_TRACER_ROTATE_X_H