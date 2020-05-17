#ifndef TG_GENERATOR_H
#define TG_GENERATOR_H

#include "FastNoise/FastNoise.h"
#include <glm/glm.hpp>

#include <vector>

struct TGGeneratorRange {
	float min_x;
	float max_x;
	float min_z;
	float max_z;
};

class TGGenerator {

private:
	int num_cells_;
	float cell_size_;
	float target_;

	glm::vec3 origin_;

	TGGeneratorRange range_;

	std::vector<glm::vec3> vertices_;

	FastNoise noise_generator_;

public:
	TGGenerator(int num_cells, float cell_size);
	~TGGenerator();

	const std::vector<glm::vec3>& vertices() const noexcept;

	void generate();

private:
	void march_cube(glm::vec3 cube_origin);

	float get_offset(float a, float b);
};

#endif