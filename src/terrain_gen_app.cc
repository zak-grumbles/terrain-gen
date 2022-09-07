#include "FastNoiseLite/FastNoiseLite.h"
#include "terrain_gen_app.h"

#include <iostream>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <thread>

namespace tg {

    const std::string TerrainGenApp::kAppName = "TerrainGen";
    const std::string TerrainGenApp::kCamName = "main_cam";

    TerrainGenApp::TerrainGenApp()
        : OgreBites::ApplicationContext(kAppName),
          window_width_(kDefaultWindowWidth),
          window_height_(kDefaultWindowHeight) {}

    TerrainGenApp::~TerrainGenApp() {}

    void TerrainGenApp::setup() {
        OgreBites::ApplicationContext::setup();

        Ogre::Root* root = this->getRoot();
        Ogre::SceneManager* scene_mgr = root->createSceneManager();
        Ogre::RTShader::ShaderGenerator* shader_gen =
            Ogre::RTShader::ShaderGenerator::getSingletonPtr();

        Ogre::SceneNode* cam_node =
            scene_mgr->getRootSceneNode()->createChildSceneNode();
        cam_node->setPosition(0, 0, 15);
        cam_node->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

        Ogre::Camera* cam = scene_mgr->createCamera(kCamName);
        cam->setNearClipDistance(0.1);
        cam->setAutoAspectRatio(true);
        cam_node->attachObject(cam);

        getRenderWindow()->addViewport(cam);
    }

    void TerrainGenApp::Run() {
        PreRunSetup();
        bool shouldClose = false;
        while(!shouldClose) {
            getRoot()->renderOneFrame();
        }
    }

    void TerrainGenApp::PreRunSetup() {
        FastNoiseLite noise;
        noise.SetNoiseType((FastNoiseLite::NoiseType_Perlin));

        generator_ = std::make_unique<Generator>(64, 2.0f);
        generator_->SetNoise(noise);
        this->getRoot()->renderOneFrame();

        std::cout << "Starting generator thread" << std::endl;
        gen_thread_ = std::make_unique<std::thread>(
            std::bind(&TerrainGenApp::GenerateTerrain, this)
        );
        gen_thread_->detach();
    }

    void TerrainGenApp::GenerateTerrain() {
        std::cout << "Generating terrain" << std::endl;
        generator_->Generate(std::bind(&TerrainGenApp::OnGeneratorFinished, this));
    }

    void TerrainGenApp::OnGeneratorFinished() {
        std::cout << "Callback called" << std::endl;
        gen_thread_.release();
    }

}  // namespace tg
