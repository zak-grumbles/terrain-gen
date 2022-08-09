#include "generator.h"

#include "tables.h"

namespace tg {

    Generator::Generator(int num_cells, float cell_size) :
        num_cells_(num_cells),
        cell_size_(cell_size),
        target_val_(0.0f) {}

    Generator::~Generator(){}

    void Generator::MarchCube(vec3 p) {
        std::vector<float> cube_values(8);
        std::vector<vec3> edge_vertices(12);

        // get values at corners of the cube / cell
        for (int vert = 0; vert < 8; vert++) {
            // temp == current corner
            // it takes the value of the starting point p, plus the
            // current offset as given by the offsets in tables.h
            vec3 temp = vec3(
                p[0] + offsets[vert][0] * cell_size_,
                p[1] + offsets[vert][1] * cell_size_,
                p[2] + offsets[vert][2] * cell_size_
            );

            // TODO: sample at point
            // cube_values[vert] = sample(temp);
        }
    }
}