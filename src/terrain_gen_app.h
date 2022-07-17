#pragma once

#include <OGRE/OgreBuildSettings.h>
#include <OGRE/Bites/OgreApplicationContext.h>

class TerrainGenApp : public OgreBites::ApplicationContext {
public:
    TerrainGenApp();
    ~TerrainGenApp();

    void setup() override;
};
