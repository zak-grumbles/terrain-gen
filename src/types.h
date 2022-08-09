#pragma once

#include <stdexcept>

namespace tg {

    struct vec3 {
        float v[3];

        vec3() : vec3(0.0f, 0.0f, 0.0f) {};
        vec3(float x, float y, float z) {
            v[0] = x;
            v[1] = y;
            v[2] = z;
        }

        float operator[](int i) const {
            if(i < 0 || i > 2) {
                throw std::out_of_range("Index out of range");
            }

            return v[i];
        }
    }

}