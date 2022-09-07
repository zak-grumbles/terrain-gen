#include "generator.h"

#include "tables.h"

#include <iostream>

namespace tg {

	Generator::Generator(int num_cells, float cell_size)
		: num_cells_(num_cells),
		cell_size_(cell_size),
		target_val_(0.0f),
		origin_(0.0f, 0.0f, 0.0f) {}

	Generator::~Generator() {
		verts_.clear();
	}

	void Generator::SetNoise(FastNoiseLite noise) { noise_ = noise; }

	void Generator::Generate(std::function<void()> completion_callback) {
		MarchingCubes();
		std::cout << "\tDone marching" << std::endl;
		completion_callback();
	}

	void Generator::MarchingCubes() {
		verts_.clear();

		std::cout << "Marching grid of size " << num_cells_ << std::endl;
		for (int x = 0; x < num_cells_; x++) {
			if (x % 16 == 0) {
				std::cout << "\t" << x << std::endl;
			}
			for (int y = 0; y < num_cells_; y++) {
				for (int z = 0; z < num_cells_; z++) {
					vec3 p;
					p.x = origin_.x + x * cell_size_;
					p.y = origin_.y + y * cell_size_;
					p.z = origin_.z + z * cell_size_;

					MarchCube(p);
				}
			}
		}
	}

	void Generator::MarchCube(vec3 p) {
		std::vector<float> cube_values(8);

		// get values at corners of the cube / cell
		for (int vert = 0; vert < 8; vert++) {
			// temp == current corner
			// it takes the value of the starting point p, plus the
			// current offset as given by the offsets in tables.h
			vec3 temp = vec3(p.x + offsets[vert][0] * cell_size_,
				p.y + offsets[vert][1] * cell_size_,
				p.z + offsets[vert][2] * cell_size_);

			// TODO: sample at point
			cube_values[vert] = noise_.GetNoise(temp.x, temp.y, temp.z);
		}

		// edge table index is determined by values at corners
		int index = 0;
		for (int test = 0; test < 8; test++) {
			if (cube_values[test] > target_val_) {
				index |= 1 << test;
			}
		}

		int edge_flags = edge_table[index];

		std::vector<vec3> edge_vertices(12);
		// if edge flags is 0, there are no faces in this cube.
		if (edge_flags != 0) {
			// this loop determines if a given edge of the cube should
			// contain a vertex and, if so, the position of that vertex.
			float offset = 0;
			for (int edge = 0; edge < 12; edge++) {
				if (edge_flags & (1 << edge)) {
					offset = GetOffset(
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
			if (triTable[index][3 * tri] < 0) {
				break;
			}
			else {

				// add 3 verts of triangle
				for (int corner = 0; corner < 3; corner++) {
					int vert = triTable[index][3 * tri + corner];
					verts_.push_back(edge_vertices[vert]);
				}
			}
		}
	}

	float Generator::GetOffset(float a, float b) {
		double offset = 0.5;

		// are these points equal?
		// if so, return default offset of 0.5
		double d = b - a;
		if (d != 0.0) {
			offset = (target_val_ - a) / d;
		}

		return offset;
	}

}  // namespace tg