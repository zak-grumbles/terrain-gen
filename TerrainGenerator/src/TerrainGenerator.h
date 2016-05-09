#include <noise/noise.h>
#include <vector>
#include "Algebra.h"
#include "noiseutils.h"

using namespace noise;

class TerrainGenerator{
public:
	TerrainGenerator(int, float);
	~TerrainGenerator();

	void draw();
	void setResolution(float);
	void MarchingCubes();
	void init();

	void setSeed(float, float, float, float);


private:
	int num_cells;
	float cell_size;
	float target;
	Point start;
	float seeds[4];

	utils::NoiseMap map;
	utils::Image image;

	std::vector<Point> verts;

	bool update;

	float sample(Point);
	void march_cube(Point);
	void init_height_map(Point);
	float get_offset(float, float);
};