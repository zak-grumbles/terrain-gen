#include "perlingenerator.h"

#include "tables.h"

PerlinGenerator::PerlinGenerator(int num_cubes, float cube_size)
    : num_cells_(num_cubes),
      cell_size_(cube_size),
      target_value_(0.0f),
      origin_(0.0f, 0.0f, 0.0f)
{
    this->verts_ = new std::vector<glm::vec3>();
}

PerlinGenerator::~PerlinGenerator()
{
    CleanUp();
}

void PerlinGenerator::CleanUp()
{
    if(verts_ != nullptr)
    {
        verts_ = nullptr;
    }
}

void PerlinGenerator::Generate()
{
    MarchingCubes_();
    emit DoneGenerating(std::move(verts_));
    emit Done();
}

void PerlinGenerator::MarchingCubes_()
{
    for(int x = 0; x < num_cells_; x++)
    {
        if(x % 2 == 0)
            emit ProgressMade((float)x / (float)num_cells_);
        for(int y = 0; y < num_cells_; y++)
        {
            for(int z = 0; z < num_cells_; z++)
            {
                glm::vec3 p(origin_.x + x * cell_size_,
                            origin_.y + y * cell_size_,
                            origin_.z + z * cell_size_);

                MarchCube_(p);
            }
        }
    }
    emit ProgressMade(1.0f);
}

void PerlinGenerator::MarchCube_(glm::vec3 p) {
    std::vector<float> cube_values(8);

    // get values at corners of the cube / cell
    for (int vert = 0; vert < 8; vert++) {
        // temp == current corner
        // it takes the value of the starting point p, plus the
        // current offset as given by the offsets in tables.h
        glm::vec3 temp(p.x + offsets[vert][0] * cell_size_,
                p.y + offsets[vert][1] * cell_size_,
                p.z + offsets[vert][2] * cell_size_);

        cube_values[vert] = noise_.GetNoise(temp.x, temp.y, temp.z);
    }

    // edge table index is determined by values at corners
    int index = 0;
    for (int test = 0; test < 8; test++) {
        if (cube_values[test] > target_value_) {
            index |= 1 << test;
        }
    }

    int edge_flags = edge_table[index];

    std::vector<glm::vec3> edge_vertices(12);
    // if edge flags is 0, there are no faces in this cube.
    if (edge_flags != 0) {
        // this loop determines if a given edge of the cube should
        // contain a vertex and, if so, the position of that vertex.
        float offset = 0;
        for (int edge = 0; edge < 12; edge++) {
            if (edge_flags & (1 << edge)) {
                offset = GetOffset_(
                            cube_values[edge_connection[edge][0]],
                            cube_values[edge_connection[edge][1]]
                        );

                edge_vertices[edge].x = p.x + (offsets[edge_connection[edge][0]][0]
                        + offset * edge_direction[edge][0]) * cell_size_;
                edge_vertices[edge].y = p.y + (offsets[edge_connection[edge][0]][1]
                        + offset * edge_direction[edge][1]) * cell_size_;
                edge_vertices[edge].z = p.z + (offsets[edge_connection[edge][0]][2]
                        + offset * edge_direction[edge][2]) * cell_size_;
            }
        }
    }

    // based on triangle table in tables.h, we can determine where
    // to place our generated vertices in the vertex vector.
    // Iterating up to 5 because there will only ever be a maximum
    // of 5 triangles in a given cube.
    for (int tri = 0; tri < 5; tri++) {

        // If we reach a negative number, we've already found all
        // triangles for this cube
        if (tri_table[index][3 * tri] < 0) {
            break;
        }
        else {

            // add 3 verts of triangle
            for (int corner = 0; corner < 3; corner++) {
                int vert = tri_table[index][3 * tri + corner];
                verts_->push_back(edge_vertices[vert]);
            }
        }
    }
}

float PerlinGenerator::GetOffset_(float a, float b)
{
    float offset = 0.5;
    float d = b - a;
    if (d != 0.0)
    {
        offset = (target_value_ - a) / d;
    }
    return offset;
}
