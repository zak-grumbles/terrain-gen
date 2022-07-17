#include "terrain_gen_app.h"

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>

TerrainGenApp::TerrainGenApp() : 
    OgreBites::ApplicationContext("TerrainGen") {
}

TerrainGenApp::~TerrainGenApp(){}

void TerrainGenApp::setup() {
    OgreBites::ApplicationContext::setup();

    Ogre::Root* root = this->getRoot();
    Ogre::SceneManager* scene_mgr = root->createSceneManager();
    Ogre::RTShader::ShaderGenerator* shader_gen = 
        Ogre::RTShader::ShaderGenerator::getSingletonPtr();

    Ogre::SceneNode* cam_node = scene_mgr->getRootSceneNode()
        ->createChildSceneNode();
    cam_node->setPosition(0, 0, 15);
    cam_node->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    Ogre::Camera* cam = scene_mgr->createCamera("main_cam");
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    cam_node->attachObject(cam);

    getRenderWindow()->addViewport(cam);
}