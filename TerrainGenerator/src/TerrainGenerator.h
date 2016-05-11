/*
	Zachary Grumbles

	Class declaration for TerrainGenerator
*/

#include <noise/noise.h>
#include <vector>
#include "Algebra.h"
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
	Point start;

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
	std::vector<Point> verts;

	//sample noise map at point
	float sample(Point);

	//perform algorithm on given cube
	void march_cube(Point);

	//initialize noise map
	void init_noise_map(Point);

	//get offset between two values
	float get_offset(float, float);
};