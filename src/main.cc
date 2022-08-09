#include <OGRE/OgreRoot.h>

#include <iostream>

#include "terrain_gen_app.h"

int main(int argc, char* argv[]) {
    tg::TerrainGenApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}