#ifndef HEIGHTMAPDATA_H
#define HEIGHTMAPDATA_H

#include <nodes/data/heightdata.h>

class HeightMapData : public HeightData
{
public:
    HeightMapData();
    virtual ~HeightMapData() {}

    float GetValueAt(float x, float y) const override { return 0.0f; }
};

#endif // HEIGHTMAPDATA_H
