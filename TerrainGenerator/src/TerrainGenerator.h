#include <vector>
#include <gmtl/Point.h>

#define CELL_SIZE 1.0f
#define VOXEL_SIZE 0.5f
struct Voxel{
    static std::vector<gmtl::Point3f> getPolygonAt(
        gmtl::Point3f bottom_front_left, float voxel_size);

private:
    static float* densityFunction(std::vector<gmtl::Point3f> verts);

	static gmtl::Point3f linear_interp(gmtl::Point3f p1, gmtl::Point3f p2, float d1, float d2);
};

struct Cell{
    Cell(gmtl::Point3f bottom_front_left, int vCount);

    std::vector<gmtl::Point3f> vertices;

    int voxel_count;
    float voxel_size;
};

class TerrainGenerator{
public:
    TerrainGenerator(int, int, int);
	std::vector<gmtl::Point3f> getVerts();

private:
    int grid_w;
    int grid_l;
	int grid_h;
    int voxel_count;
	gmtl::Point3f startPoint;
    
};