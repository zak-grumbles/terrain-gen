#include <vector>
#include <gmtl/Point.h>
#include "Tri.h"
#include "noiseutils.h"

#define CELL_SIZE 1.0f
#define VOXEL_SIZE 0.1f
#define HEIGHT_MAP "h_map.bmp"

class TerrainGenerator{
public:
    TerrainGenerator(int, int, int, float v_size = VOXEL_SIZE);
	std::vector<Tri> getTriangles();
    
    void setWidth(int);
    int getWidth();
    
    void setHeight(int);
    int getHeight();
    
    void setLength(int);
    int getLength();
    
    void setVoxelSize(float);
    float getVoxelSize();

    bool shouldUpdate();
    
	utils::Image getHeightImage();

private:
    int grid_w;
    int grid_l;
	int grid_h;

    float voxel_size;

    bool should_update;

    utils::NoiseMap height_map;
    noise::module::Perlin plane_noise;
	utils::Image height_image;

	gmtl::Point3f startPoint;

	void generateHeightMap();

	float* densityFunction(std::vector<gmtl::Point3f> verts);

	std::vector<Tri> getPolygonAt(gmtl::Point3f bottom_front_left);
};