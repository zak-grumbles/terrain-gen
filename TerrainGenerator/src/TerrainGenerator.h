#include <vector>
#include <gmtl/Point.h>
#include "Tri.h"
#include "noiseutils.h"

#define CELL_SIZE 1.0f
#define VOXEL_SIZE 0.1f
#define HEIGHT_MAP "h_map.bmp"

struct Voxel{
    static std::vector<Tri> getPolygonAt(
        gmtl::Point3f bottom_front_left, float voxel_size);

private:
    static float* densityFunction(std::vector<gmtl::Point3f> verts);

	static gmtl::Point3f linear_interp(gmtl::Point3f p1, gmtl::Point3f p2, float d1, float d2);
};

class TerrainGenerator{
public:
    TerrainGenerator(int, int, int);
	std::vector<Tri> getTriangles();
	utils::NoiseMap height_map;
	noise::module::Perlin plane_noise;

private:
    int grid_w;
    int grid_l;
	int grid_h;
    int voxel_count;
	gmtl::Point3f startPoint;
	void generateHeightMap();
};