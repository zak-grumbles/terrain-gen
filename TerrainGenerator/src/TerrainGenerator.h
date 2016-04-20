#include <vector>
#include <gmtl/Point.h>

#define CELL_SIZE 1.0f

struct Voxel{
    static std::vector<gmtl::Point3f> getPolygonAt(
        gmtl::Point3f bottom_front_left);

private:
    static float densityFunction(gmtl::Point3f p);
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
    

private:
    int grid_w;
    int grid_l;
    int voxel_count;
    std::vector<std::vector<Cell>> cell_grid;
    
};