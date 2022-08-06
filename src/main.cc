#include <iostream>

#include "terrain_gen_app.h"

#include <OGRE/OgreRoot.h>

int main(int argc, char* argv[]) {
    TG::TerrainGenApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}