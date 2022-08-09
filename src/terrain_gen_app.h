#pragma once

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/OgreBuildSettings.h>

namespace tg {

constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1280;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 960;

class TerrainGenApp : public OgreBites::ApplicationContext {
   public:
    TerrainGenApp();
    ~TerrainGenApp();

    void setup() override;

   protected:
    unsigned int window_width_;
    unsigned int window_height_;
};

}  // namespace tg
