#include "terrainviewwidget.h"

#include <math.h>

#include "generators/perlingenerator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TerrainViewWidget::TerrainViewWidget(QWidget* parent) : QOpenGLWidget(parent),
    vbo_(QOpenGLBuffer::VertexBuffer)
{
    float aspect = (float)width() / (float)height();
    auto camera_position = glm::vec3(5.0f, 30.0f, 10.0f);

    camera_ = std::make_unique<Camera>(camera_position,
        45.0f, aspect, 0.1f, 100.0f);
    camera_->LookAt(glm::vec3(5.0f, 0, -5));


    model_matrix_ = glm::mat4(1.0f);
}

TerrainViewWidget::~TerrainViewWidget()
{
    makeCurrent();

    glDeleteProgram(shader_prog_);
    vbo_.destroy();
    vao_.destroy();

    doneCurrent();

    camera_.reset();
}

void TerrainViewWidget::keyPressEvent(QKeyEvent* event)
{
    int direction = 0;
    if(event->key() == Qt::Key_W)
    {
        direction |= CameraDirections::kForward;
    }
    camera_->Move(direction);
    update();
}

void TerrainViewWidget::initializeGL()
{
    initializeOpenGLFunctions();

    CompileShaders_();

    std::vector<glm::vec3> verts = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };
    terrain_verts_ = std::make_unique<std::vector<glm::vec3>>(verts);

    vao_.create();
    if(vao_.isCreated())
    {
        vao_.bind();
    }

    vbo_.create();
    vbo_.bind();

    vbo_.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_.allocate(terrain_verts_->data(), (int)(sizeof(glm::vec3) * terrain_verts_->size()));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vbo_.release();
    vao_.release();
}

void TerrainViewWidget::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;
    camera_->SetAspectRatio(aspect);
}

void TerrainViewWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_prog_);
    vao_.bind();

    unsigned int modelLoc = glGetUniformLocation(shader_prog_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix_));

    unsigned int viewLoc = glGetUniformLocation(shader_prog_, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera_->GetViewMatrixValuePtr());

    unsigned int projLoc = glGetUniformLocation(shader_prog_, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera_->GetProjectionMatrixValuePtr());

    if(render_wireframe_ == true)
    {
        glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)terrain_verts_->size());
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)terrain_verts_->size());
    }
}

void TerrainViewWidget::Generate()
{
    emit StatusUpdate("Starting generation...");

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    PerlinGenerator* generator = new PerlinGenerator(grid_size_, cube_size_);
    generator->SetNoise(noise);
    generator->moveToThread(&generator_thread_);

    // thread signals/slots
    connect(&generator_thread_, &QThread::started, generator, &PerlinGenerator::Generate);
    connect(&generator_thread_, &QThread::finished, generator, &PerlinGenerator::deleteLater);
    connect(&generator_thread_, &QThread::finished, this, &TerrainViewWidget::OnGenThreadFinished);

    // generator signals/slots
    connect(generator, &PerlinGenerator::ProgressMade, this, &TerrainViewWidget::OnGenerationProgress);
    connect(generator, &PerlinGenerator::DoneGenerating, this, &TerrainViewWidget::OnTerrainGenerated);
    connect(generator, &PerlinGenerator::Done, &generator_thread_, &QThread::quit);

    generator_thread_.start();
}

void TerrainViewWidget::OnGenerationProgress(float percent)
{
    emit ProgressUpdate((int)floor(percent * 100));
}

void TerrainViewWidget::OnTerrainGenerated(std::vector<glm::vec3>* verts)
{
    emit StatusUpdate("Done generating!");
    terrain_verts_ = std::unique_ptr<std::vector<glm::vec3>>(verts);

    makeCurrent();
    vbo_.destroy();

    vao_.bind();
    vbo_.create();
    vbo_.bind();

    vbo_.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo_.allocate(terrain_verts_->data(), (int)(sizeof(glm::vec3) * terrain_verts_->size()));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vbo_.release();
    vao_.release();

    doneCurrent();

    update();
}

void TerrainViewWidget::OnGenThreadFinished()
{
    // Nothing to do?
}

void TerrainViewWidget::SetRenderWireframe(bool wireframe)
{
    render_wireframe_ = wireframe;
    update();
}

void TerrainViewWidget::SetCubeSize(double newSize)
{
    if(newSize > 0.0f)
    {
        cube_size_ = (float)newSize;
    }
    else
    {
        emit StatusUpdate("Cube size must be non-zero!");
    }
}

void TerrainViewWidget::SetGridSize(int newSize)
{
    if(newSize > 0)
    {
        grid_size_ = newSize;
    }
    else
    {
        emit StatusUpdate("Grid size must be non-zero!");
    }
}

bool TerrainViewWidget::CompileShaders_()
{
    bool success = false;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 pos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 proj;\n"
        "void main()\n"
        "{\n"
        "	mat4 mvp = proj * view * model;"
        "	pos = mvp * vec4(aPos.x, aPos.y * 2.0, aPos.z, 1.0);\n"
        "	gl_Position = pos;"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec4 pos;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   if(pos.y <= 0.5f)\n"
        "   {\n"
        "		FragColor = vec4(0.871f, 0.878f, 0.706f, 1.0f);\n"
        "	}\n"
        "   else if(pos.y > 0.5f && pos.y <= 5.0f)\n"
        "   {\n"
        "		FragColor = vec4(0.114f, 0.420f, 0.153f, 1.0f);\n"
        "	}\n"
        "   else if(pos.y > 5.0f && pos.y <= 7.0f)\n"
        "   {\n"
        "		FragColor = vec4(0.710f, 0.710f, 0.710f, 1.0f);\n"
        "	}\n"
        "   else if(pos.y > 7.0f)\n"
        "   {\n"
        "		FragColor = vec4(0.969f, 1.0f, 0.980f, 1.0f);\n"
        "	}\n"
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
        emit StatusUpdate(QString("Vertex shader error: %1").arg(log));
    }

    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSource, nullptr);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        glGetShaderInfoLog(frag, 512, nullptr, log);
        emit StatusUpdate(QString("Fragment shader error: %1").arg(log));
    }

    shader_prog_ = glCreateProgram();
    glAttachShader(shader_prog_, vert);
    glAttachShader(shader_prog_, frag);
    glLinkProgram(shader_prog_);

    glGetProgramiv(shader_prog_, GL_LINK_STATUS, &status);
    if(!status)
    {
        glGetProgramInfoLog(shader_prog_, 512, nullptr, log);
        emit StatusUpdate(QString("Shader link error: %1").arg(log));
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return success;
}
