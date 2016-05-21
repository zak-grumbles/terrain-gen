/*
	Zachary Grumbles

	Class declaration for TerrainGenerator
*/

#include <noise/noise.h>
#include <vector>
#include <glm/glm.hpp>
#include "noiseutils.h"

using namespace noise;

class TerrainGenerator{
public:
	//constructor
	TerrainGenerator(int, float);

	//destructor
	~TerrainGenerator();

	//render terrain
	void draw();

	//set cell size
	void setCellSize(float);

	//execute algorithm
	void MarchingCubes();

	//initialize noise map and run algorithm
	void init();
	
	//set seed values
	void setSeed(float, float, float, float);


private:
	//number cells in grid
	int num_cells;

	//size of cells
	float cell_size;

	//targer value
	float target;

	//origin point of terrain
    glm::vec3 start;

	/*
		Seed values.
		These are actually the ranges that the noise function will be sampled
		across. 
		seeds[0] = minX, seeds[1] = maxX,
		seeds[2] = minZ, seeds[3] = maxZ
	*/
	float seeds[4];

	//noise map
	utils::NoiseMap map;

	//vertices that represent terrain
	std::vector<glm::vec3> verts;

	//sample noise map at point
	float sample(glm::vec3);

	//perform algorithm on given cube
	void march_cube(glm::vec3);

	//initialize noise map
	void init_noise_map(glm::vec3);

	//get offset between two values
	float get_offset(float, float);
};