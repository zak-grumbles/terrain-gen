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
    PerlinGenerator(int numCubes, float cubeSize);
    ~PerlinGenerator();

    void setNoise(FastNoiseLite noise) { this->noise = noise; }

signals:
    void progressMade(float percentComplete);
    void doneGenerating(std::vector<glm::vec3>* verts);
    void done();

public slots:
    void generate();
    void cleanUp();

protected:
    void marchingCubes();
    void marchCube(glm::vec3 p);
    float getOffset(float a, float b);

    int numCells;
    float cellSize;
    float targetValue;
    glm::vec3 origin;
    std::vector<glm::vec3>* verts;
    FastNoiseLite noise;
};

#endif // PERLINGENERATOR_H
