#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "FastNoiseLite/FastNoiseLite.h"
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

    /**
     * @brief Set the \p noise_ object
     * 
     * @param noise New noise.
     */
    void SetNoise(FastNoiseLite noise);

    void Generate(std::function<void()> completion_callback);

    /**
     * @brief Get the vertices that make up the terrain.
     * 
     * @return const std::vector<vec3>& Vector of vertices.
     */
    const std::vector<vec3>& GetTerrain() const { return verts_; }

   protected:
    /**
     * @brief Executes the marching cubes algorithm for 
     *        the grid of cubes to generate the terrain.
     */
    void MarchingCubes();

    /**
     * @brief Executes the marching cube algorithm for the
     *        given "cube" or cell.
     * 
     * @param p Origin point of the cube to "march".
     */
    void MarchCube(vec3 p);

    /**
     * @brief Get the offset between two values.
     * 
     * @param a First value
     * @param b Second value.
     * @return float Offset between \p a and \p b.
     */
    float GetOffset(float a, float b);

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

    /**
     * @brief Vertices representing the terrain.
     */
    std::vector<vec3> verts_;

    /**
     * @brief Noise function that will be used to 
     *        generate terrain.
     */
    FastNoiseLite noise_;
};

}  // namespace tg