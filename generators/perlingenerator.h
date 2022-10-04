#ifndef PERLINGENERATOR_H
#define PERLINGENERATOR_H

#include <QObject>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <functional>
#include <memory>
#include <vector>

#include "FastNoiseLite/FastNoiseLite.h"

class PerlinGenerator : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief PerlinGenerator constructor
     * @param numCubes number of cubes in the grid
     * @param cubeSize size of each cube
     */
    PerlinGenerator(int num_cubes, float cube_size);
    ~PerlinGenerator();

    void SetNoise(FastNoiseLite noise) { this->noise_ = noise; }

signals:
    void ProgressMade(float percent_complete);
    void DoneGenerating(std::vector<glm::vec3>* verts);
    void Done();

public slots:
    void Generate();
    void CleanUp();

protected:
    void MarchingCubes_();
    void MarchCube_(glm::vec3 p);
    float GetOffset_(float a, float b);

    int num_cells_;
    float cell_size_;
    float target_value_;
    glm::vec3 origin_;
    std::vector<glm::vec3>* verts_;
    FastNoiseLite noise_;
};

#endif // PERLINGENERATOR_H
