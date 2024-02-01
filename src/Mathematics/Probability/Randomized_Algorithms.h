//
// Created by Rami on 9/27/2023.
//

#ifndef CUDA_RAY_TRACER_RANDOMIZED_ALGORITHMS_H
#define CUDA_RAY_TRACER_RANDOMIZED_ALGORITHMS_H

#include "../../Utilities.h"
#include <random>

inline double random_double() {
    // Returns a random double between [0.0,1.0).

    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double lower_bound, double upper_bound) {
    // Returns a random double between [lower_range,upper_range).

    return lower_bound + (upper_bound-lower_bound)*random_double();
}


static Vec3D random_vector_in_range(double lower_bound, double upper_bound) {
    // Returns a random vector with each of its components in the range: [lower_bound,upper_bound).

    return {
            random_double(lower_bound, upper_bound),
            random_double(lower_bound, upper_bound),
            random_double(lower_bound, upper_bound)
            };
}

inline int random_int_in_range(double min, double max) {
    // Returns a random int in [min, max]

    return static_cast<int>(random_double(min, max+1));
}

static Vec3D random_vector_in_range() {
    // Returns a random vector with each if its components in the range: [lower_bound,upper_bound).

    return {
            random_double(),
            random_double(),
            random_double()
            };
}

/// Reference: [4]
inline Vec3D random_unit_vector() {
    // Returns a random vector on the unit sphere of directions.

    do {
        auto random_vector = random_vector_in_range(-1, 1);
        if (random_vector.length_squared() < 1)
            return unit_vector(random_vector);
    } while (true);
}

inline Vec3D random_on_hemisphere(const Vec3D& normal) {
    // Returns a random vector on the unit hemisphere of directions.

    Vec3D on_unit_sphere = random_unit_vector();

    // In the same hemisphere as the normal?
    if (dot_product(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline Vec3D random_cosine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();

    auto phi = 2*M_PI*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);
    auto z = sqrt(1-r2);
   // std::cout << x << " " << y << " " << z << std::endl;
    return Vec3D(x, y, z);
}

const double TWO_PI = 2 * M_PI;

inline Vec3D cosine_weighted_direction() {
    double a = random_double();
    double b = random_double();

    double phi = TWO_PI * a;
    double sqrt_b = sqrt(b);
    double cos_phi = cos(phi);

    return {cos_phi * sqrt_b, sin(phi) * sqrt_b, sqrt(1.0 - b)};
}
#endif //CUDA_RAY_TRACER_RANDOMIZED_ALGORITHMS_H