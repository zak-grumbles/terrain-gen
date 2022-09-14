#include "terrainviewwidget.h"

#include "perlingenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TerrainViewWidget::TerrainViewWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    float aspect = (float)width() / (float)height();
    cameraPosition = glm::vec3(0.25f, 50.0f, 0.0f);

    modelMatrix = glm::mat4();
    viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
}

void TerrainViewWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    static float one[] = {1, 1, 1, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

    makeCurrent();
}

void TerrainViewWidget::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;
    projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
}

void TerrainViewWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glm::mat4 modelView = modelMatrix * viewMatrix;
    glLoadMatrixf(glm::value_ptr(modelView));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // draw terrain
    glBegin(GL_TRIANGLES);
    if(terrainVerts != nullptr)
    {
        for(int i = 0; i < terrainVerts->size(); i++)
        {
            float y = terrainVerts->at(i)[1] * 10.0;

            if(y < 0.5f)
                glColor3f(0.871f, 0.878f, 0.706f);
            else if(y > 0.5f && y <= 5.0f)
                glColor3f(0.114f, 0.420f, 0.153f);
            else if(y > 5.0f && y <= 7.0f)
                glColor3f(0.710f, 0.710f, 0.710f);
            else if(y > 7.0f)
                glColor3f(0.969f, 1.0f, 0.980f);

            glVertex3f(terrainVerts->at(i)[0], y, terrainVerts->at(i)[2]);
        }
    }
    glEnd();
}

void TerrainViewWidget::generate()
{
    emit statusUpdate("Starting generation...");

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    PerlinGenerator* generator = new PerlinGenerator(64, 0.25f);
    generator->setNoise(noise);
    generator->moveToThread(&generatorThread);

    // thread signals/slots
    connect(&generatorThread, &QThread::started, generator, &PerlinGenerator::generate);
    connect(&generatorThread, &QThread::finished, generator, &PerlinGenerator::deleteLater);
    connect(&generatorThread, &QThread::finished, this, &TerrainViewWidget::onGenThreadFinished);

    // generator signals/slots
    connect(generator, &PerlinGenerator::progressMade, this, &TerrainViewWidget::onGenerationProgress);
    connect(generator, &PerlinGenerator::doneGenerating, this, &TerrainViewWidget::onTerrainGenerated);
    connect(generator, &PerlinGenerator::done, &generatorThread, &QThread::quit);

    generatorThread.start();
}

void TerrainViewWidget::onGenerationProgress(float percent)
{
    emit statusUpdate(QString("%1 percent generated").arg(percent * 100));
}

void TerrainViewWidget::onTerrainGenerated(std::vector<glm::vec3>* verts)
{
    emit statusUpdate("Done generating!");
    terrainVerts = std::unique_ptr<std::vector<glm::vec3>>(verts);
    update();
}

void TerrainViewWidget::onGenThreadFinished()
{
    // Nothing to do?
}
