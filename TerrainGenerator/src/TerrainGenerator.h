/*
	Zachary Grumbles

	Class declaration for TerrainGenerator
*/

#include <noise/noise.h>
#include <vector>
#include <gmtl/Point.h>
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
	gmtl::Point3f start;

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
	std::vector<gmtl::Point3f> verts;

	//sample noise map at point
	float sample(gmtl::Point3f);

	//perform algorithm on given cube
	void march_cube(gmtl::Point3f);

	//initialize noise map
	void init_noise_map(gmtl::Point3f);

	//get offset between two values
	float get_offset(float, float);
};