//
// Created by Rami on 1/30/2024.
//

#ifndef CUDA_RAY_TRACER_MIXTURE_PDF_H
#define CUDA_RAY_TRACER_MIXTURE_PDF_H

#include "PDF.h"

class Mixture_PDF : public PDF {
public:
    // Constructor
    // -----------------------------------------------------------------------
    Mixture_PDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    // Overridden Functions
    // -----------------------------------------------------------------------
    double PDF_value(const Vec3D &direction) const override {
        /* DEBUGGING
          std::cout << "Light PDF: " <<p[0]->PDF_value(direction) << ", Surface PDF: " << p[1]->PDF_value(direction) << std::endl;
          std::cout << direction << std::endl;
          if (std::isnan(direction.x())) {
            // Handle the case where pdf0 or pdf1 is NaN
            std::cerr << "PDF_value: NaN detected in pdf0 or pdf1." << std::endl;
            return 0.0; // Or handle it according to your requirements
          }
        */
        return 0.5 * p[0]->PDF_value(direction) + 0.5*p[1]->PDF_value(direction);
    }

    Vec3D generate_a_random_direction_based_on_PDF() const override {
        if (random_double() < 0.5) {
            //    if (std::isnan(p[0]->generate_a_random_direction_based_on_PDF().x())) {
            //    std::cout << p[0]->generate_a_random_direction_based_on_PDF();
            //    }
            //    std::cout << p[0]->generate_a_random_direction_based_on_PDF() << std::endl;
            return p[0]->generate_a_random_direction_based_on_PDF();
        }
        else {
            // std::cout << p[1]->generate_a_random_direction_based_on_PDF();
            return p[1]->generate_a_random_direction_based_on_PDF();
        }
    }

private:
    // Data members
    // -----------------------------------------------------------------------
    std::shared_ptr<PDF> p[2];              // the two PDFs to be mixed together
};

#endif //CUDA_RAY_TRACER_MIXTURE_PDF_H
