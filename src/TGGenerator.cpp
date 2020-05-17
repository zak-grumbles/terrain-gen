#include "TGGenerator.h"

#include "TGTables.h"

TGGenerator::TGGenerator(int num_cells, float cell_size) {
	num_cells_ = num_cells;
	cell_size_ = cell_size;
	target_ = 0.0f;
	vertices_ = std::vector<glm::vec3>();
	noise_generator_.SetNoiseType(FastNoise::Perlin);
}

TGGenerator::~TGGenerator() {
	vertices_.clear();
}

const std::vector<glm::vec3>& TGGenerator::vertices() const noexcept {
	return vertices_;
}

void TGGenerator::generate() {
	vertices_.clear();

	origin_ = glm::vec3(0, 0, 0);

	for (int x = 0; x < num_cells_; x++) {

		for (int y = 0; y < num_cells_; y++) {

			for (int z = 0; z < num_cells_; z++) {

				glm::vec3 p;
				p.x = origin_.x + x * cell_size_;
				p.y = origin_.y + y * cell_size_;
				p.z = origin_.z + z * cell_size_;

				march_cube(p);
			}

		}

	}
}

void TGGenerator::march_cube(glm::vec3 cube_origin) {

	// get values at corners of cube
	float cube_value[8];
	for (int v = 0; v < 8; v++) {
		/*
			temp == current corner
			Takes value of starting point 'cube_origin', plus current offset
			as given by offsets table in TGTables.h
		 */
		glm::vec3 temp(
			cube_origin.x + offsets[v][0] * cell_size_,
			cube_origin.y + offsets[v][1] * cell_size_,
			cube_origin.z + offsets[v][2] * cell_size_
		);

		cube_value[v] = noise_generator_.GetNoise(cube_origin.x, cube_origin.z) -
			cube_origin.y;
	}

	// Determine edge table index based on corner values
	int index = 0;
	for (int test = 0; test < 8; test++) {
		if (cube_value[test] > target_) {
			index |= 1 << test;
		}
	}

	int edge_flags = edge_table[index];

	// If edge_flags are 0, no faces in this cube. i.e. outside of mesh
	if (edge_flags == 0) {
		return;
	}

	// determine if given edge of cube should contain a vertex
	// if so, determine position
	float offset = 0;
	glm::vec3 edge_vertex[12];
	for (int edge = 0; edge < 12; edge++) {
		if (edge_flags & (1 << edge)) {
			offset = get_offset(
				cube_value[edge_connection[edge][0]],
				cube_value[edge_connection[edge][1]]
			);

			edge_vertex[edge].x = cube_origin.x + (offsets[edge_connection[edge][0]][0]
				+ offset * edge_direction[edge][0]) * cell_size_;
			edge_vertex[edge].y = cube_origin.y + (offsets[edge_connection[edge][0]][1]
				+ offset * edge_direction[edge][1]) * cell_size_;
			edge_vertex[edge].z = cube_origin.z + (offsets[edge_connection[edge][0]][2]
				+ offset * edge_direction[edge][2]) * cell_size_;
		}
	}


	/*
		Based on triangle table, determine where to place generated
		vertices in verticies_ vector.
		
		Iterating up to 5 because there will only ever be a max of 5
		triangles in any given cube.
	 */
	for (int tri = 0; tri < 5; tri++) {

		// If we reach a negative, we have already found all triangles
		if (tri_table[index][3 * tri] < 0)
			break;

		for (int corner = 0; corner < 3; corner++) {
			int vert = tri_table[index][3 * tri + corner];

			vertices_.push_back(edge_vertex[vert]);
		}

	}
}

float TGGenerator::get_offset(float a, float b) {
	double d = (double)b - (double)a;

	if (d == 0.0)
		return 0.5;

	double diff = (double)target_ - (double)a;
	return diff / d;
}
