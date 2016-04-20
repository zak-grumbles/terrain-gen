#include "TerrainGenerator.h"

std::vector<gmtl::Point3f> Voxel::getPolygonAt(gmtl::Point3f bottom_front_left)
{

}

Cell::Cell(gmtl::Point3f bottom_front_left, int vCount){
    voxel_count = vCount;
    voxel_size = CELL_SIZE / (float)voxel_count;

    int x = bottom_front_left[0];
    int y = bottom_front_left[1];
    int z = bottom_front_left[2];

    for (int i = 0; i < voxel_count; i++){
        x += i * voxel_size;

        for (int j = 0; j < voxel_count; j++){
            y += j * voxel_size;

            for (int k = 0; k < voxel_count; k++){
                z += k * voxel_size;

                std::vector<gmtl::Point3f> v = Voxel::getPolygonAt(
                    gmtl::Point3f(x, y, z));
                vertices.insert(vertices.end(), v.begin(), v.end());
            }

        }

    }
}

TerrainGenerator::TerrainGenerator(int w, int l, int voxelCount){
    grid_w = w;
    grid_l = l;
    voxel_count = voxelCount;
    if (grid_w > 0 && grid_l > 0){
        cell_grid = std::vector<std::vector<Cell>>();

        float start_x = ((float)grid_w * CELL_SIZE) / 2.0;
        start_x *= -1;
        float start_z = ((float)grid_l * CELL_SIZE) / 2.0;
        start_z *= -1;

        float y = -0.5f;


        for (int i = 0; i < grid_w; i++){
            cell_grid.push_back(std::vector<Cell>());

            start_x += i * CELL_SIZE;

            for (int j = 0; j < grid_l; j++){
                start_z += j * CELL_SIZE;

                cell_grid[i].push_back(Cell(gmtl::Point3f(start_x, y,
                    start_z), voxel_count));
            }

        }
    }
}