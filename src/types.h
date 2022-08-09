#pragma once

#include <stdexcept>

namespace tg {

    struct vec3 {
        float x;
        float y;
        float z;
        
        vec3() : vec3(0.0f, 0.0f, 0.0f) {};
        vec3(float x, float y, float z) :
            x(x), y(y), z(z) {}
    };

}