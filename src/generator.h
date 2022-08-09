#pragma once

#include <vector>

#include "types.h"

namespace tg {

/**
 * @brief Basic terrain generator.
 *        Utilizes the marching cubes algorithm.
 */
class Generator {
   public:
    /**
     * @brief Construct a new Generator object
     * 
     * @param num_cells Number of cells in the grid.
     * @param cell_size Size of each cell in the grid.
     */
    Generator(int num_cells, float cell_size);

    /**
     * @brief Destroy the Generator object
     */
    ~Generator();

   protected:

    /**
     * @brief Executes the marching cube algorithm for the
     *        given "cube" or cell.
     * 
     * @param p Origin point of the cube to "march".
     */
    void MarchCube(vec3 p);

    /**
     * @brief Number of cells in the grid.
     */
    int num_cells_;

    /**
     * @brief Size of each cubic cell in the grid.
     */
    float cell_size_;

    /**
     * @brief Target value used in the marching cubes
     *        algorithm when checking each cell.
     */
    float target_val_;

    /**
     * @brief Origin point of the terrain.
     *        This generator considers the origin to be
     *        the bottom left corner of the grid.
     */
    vec3 origin_;

    std::vector<vec3> verts_;
};

}  // namespace tg