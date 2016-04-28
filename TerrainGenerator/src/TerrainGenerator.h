#include <vector>
#include <gmtl/Point.h>
#include "Tri.h"
#include "noiseutils.h"

#define DEFAULT_VOX_SIZE = 1.0f;
#define HEIGHT_MAP "h_map.bmp"

struct Voxel{
	Point3f p[8];	//corners of voxels
	float d[8];		//density values of corners
};

class TerrainGenerator{
public:
	TerrainGenerator(int, int, int, float);
	
	void update();

	std::vector<Tri> getTriangles();

	bool shouldUpdate();

private:

	std::vector<Tri> triangles;

	bool should_update;

	int x_count, y_count, z_count;

	float vox_size;
	float x_bound, y_bound, z_bound;

	Point3f start;

	void initSlab(float*, int);

	void makePolysFromSlabs(float*, float*, int);

	float densityFunction(Point3f p);

	void addTriangles(Voxel);
};