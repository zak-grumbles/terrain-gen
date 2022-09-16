#include "terrainviewwidget.h"

#include "generators/perlingenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TerrainViewWidget::TerrainViewWidget(QWidget* parent) : QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer)
{
    float aspect = (float)width() / (float)height();
    cameraPosition = glm::vec3(5.0f, 15.0f, 10.0f);

    modelMatrix = glm::mat4(1.0f);
    viewMatrix = glm::lookAt(cameraPosition, glm::vec3(5.0f, 0, -1), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
}

TerrainViewWidget::~TerrainViewWidget()
{
    makeCurrent();

    glDeleteProgram(shaderProg);
    vbo.destroy();
    vao.destroy();

    doneCurrent();
}

void TerrainViewWidget::initializeGL()
{
    initializeOpenGLFunctions();

    compileShaders();

    std::vector<glm::vec3> verts = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };
    terrainVerts = std::make_unique<std::vector<glm::vec3>>(verts);

    vao.create();
    if(vao.isCreated())
    {
        vao.bind();
    }

    vbo.create();
    vbo.bind();

    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(terrainVerts->data(), sizeof(glm::vec3) * terrainVerts->size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vbo.release();
    vao.release();
}

void TerrainViewWidget::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;
    projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
}

void TerrainViewWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProg);
    vao.bind();

    unsigned int modelLoc = glGetUniformLocation(shaderProg, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    unsigned int projLoc = glGetUniformLocation(shaderProg, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_LINE_LOOP, 0, terrainVerts->size());
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

    makeCurrent();
    vbo.destroy();

    vao.bind();
    vbo.create();
    vbo.bind();

    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(terrainVerts->data(), sizeof(glm::vec3) * terrainVerts->size());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vbo.release();
    vao.release();

    doneCurrent();

    update();
}

void TerrainViewWidget::onGenThreadFinished()
{
    // Nothing to do?
}

bool TerrainViewWidget::compileShaders()
{
    bool success = false;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"
        "void main()\n"
        "{\n"
            "mat4 mvp = proj * view * model;"
        "   gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderSource, nullptr);
    glCompileShader(vert);

    int status;
    char log[512];
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        glGetShaderInfoLog(vert, 512, nullptr, log);
        emit statusUpdate(QString("Vertex shader error: %1").arg(log));
    }

    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSource, nullptr);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        glGetShaderInfoLog(frag, 512, nullptr, log);
        emit statusUpdate(QString("Fragment shader error: %1").arg(log));
    }

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vert);
    glAttachShader(shaderProg, frag);
    glLinkProgram(shaderProg);

    glGetProgramiv(shaderProg, GL_LINK_STATUS, &status);
    if(!status)
    {
        glGetProgramInfoLog(shaderProg, 512, nullptr, log);
        emit statusUpdate(QString("Shader link error: %1").arg(log));
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return success;
}
