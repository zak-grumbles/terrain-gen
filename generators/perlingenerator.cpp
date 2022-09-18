#include "perlingenerator.h"

#include "tables.h"

PerlinGenerator::PerlinGenerator(int numCubes, float cubeSize)
    : numCells(numCubes),
      cellSize(cubeSize),
      targetValue(0.0f),
      origin(0.0f, 0.0f, 0.0f)
{
    this->verts = new std::vector<glm::vec3>();
}

PerlinGenerator::~PerlinGenerator()
{
    cleanUp();
}

void PerlinGenerator::cleanUp()
{
    if(verts != nullptr)
    {
        verts = nullptr;
    }
}

void PerlinGenerator::generate()
{
    marchingCubes();
    emit doneGenerating(std::move(verts));
    emit done();
}

void PerlinGenerator::marchingCubes()
{
    for(int x = 0; x < numCells; x++)
    {
        if(x % 2 == 0)
            emit progressMade((float)x / (float)numCells);
        for(int y = 0; y < numCells; y++)
        {
            for(int z = 0; z < numCells; z++)
            {
                glm::vec3 p(origin.x + x * cellSize,
                            origin.y + y * cellSize,
                            origin.z + z * cellSize);

                marchCube(p);
            }
        }
    }
    emit progressMade(1.0f);
}

void PerlinGenerator::marchCube(glm::vec3 p) {
    std::vector<float> cubeValues(8);

    // get values at corners of the cube / cell
    for (int vert = 0; vert < 8; vert++) {
        // temp == current corner
        // it takes the value of the starting point p, plus the
        // current offset as given by the offsets in tables.h
        glm::vec3 temp(p.x + offsets[vert][0] * cellSize,
                p.y + offsets[vert][1] * cellSize,
                p.z + offsets[vert][2] * cellSize);

        // TODO: sample at point
        cubeValues[vert] = noise.GetNoise(temp.x, temp.y, temp.z);
    }

    // edge table index is determined by values at corners
    int index = 0;
    for (int test = 0; test < 8; test++) {
        if (cubeValues[test] > targetValue) {
            index |= 1 << test;
        }
    }

    int edgeFlags = edgeTable[index];

    std::vector<glm::vec3> edgeVertices(12);
    // if edge flags is 0, there are no faces in this cube.
    if (edgeFlags != 0) {
        // this loop determines if a given edge of the cube should
        // contain a vertex and, if so, the position of that vertex.
        float offset = 0;
        for (int edge = 0; edge < 12; edge++) {
            if (edgeFlags & (1 << edge)) {
                offset = getOffset(
                            cubeValues[edgeConnection[edge][0]],
                            cubeValues[edgeConnection[edge][1]]
                        );

                edgeVertices[edge].x = p.x + (offsets[edgeConnection[edge][0]][0]
                        + offset * edgeDirection[edge][0]) * cellSize;
                edgeVertices[edge].y = p.y + (offsets[edgeConnection[edge][0]][1]
                        + offset * edgeDirection[edge][1]) * cellSize;
                edgeVertices[edge].z = p.z + (offsets[edgeConnection[edge][0]][2]
                        + offset * edgeDirection[edge][2]) * cellSize;
            }
        }
    }

    // based on triangle table in tables.h, we can determine where
    // to place our generated vertices in the vertex vector.
    // Iterating up to 5 because there will only ever be a maximum
    // of 5 triangles in a given cube.
    for (int tri = 0; tri < 5; tri++) {

        // If we reach a negative number, we've already found all
        // triangles for this cube
        if (triTable[index][3 * tri] < 0) {
            break;
        }
        else {

            // add 3 verts of triangle
            for (int corner = 0; corner < 3; corner++) {
                int vert = triTable[index][3 * tri + corner];
                verts->push_back(edgeVertices[vert]);
            }
        }
    }
}

float PerlinGenerator::getOffset(float a, float b)
{
    float offset = 0.5;
    float d = b - a;
    if (d != 0.0)
    {
        offset = (targetValue - a) / d;
    }
    return offset;
}
