#pragma once
#include "generator.h"

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/OgreBuildSettings.h>
#include <thread>

namespace tg {
    constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1280;
    constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 960;

    class TerrainGenApp : public OgreBites::ApplicationContext {
       public:
        TerrainGenApp();
        ~TerrainGenApp();

        void setup() override;

        void Run();

       protected:
        void PreRunSetup();
        void GenerateTerrain();
        void OnGeneratorFinished();

        unsigned int window_width_;
        unsigned int window_height_;

        std::unique_ptr<Generator> generator_;
        std::unique_ptr<std::thread> gen_thread_;
    };
}  // namespace tg
