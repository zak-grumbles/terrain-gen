#include "terrainviewwidget.h"

#include <math.h>

#include "shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TerrainViewWidget::TerrainViewWidget(QWidget* parent) : QOpenGLWidget(parent),
    vbo_(QOpenGLBuffer::VertexBuffer)
{
    float aspect = (float)width() / (float)height();
    auto camera_position = glm::vec3(0.0f, 0.0f, 10.0f);

    camera_ = std::make_unique<Camera>(camera_position,
        45.0f, aspect, 0.1f, 1000.0f);
    camera_->LookAt(glm::vec3(0.0f, 0, 0));


    model_matrix_ = glm::mat4(1.0f);
}

TerrainViewWidget::~TerrainViewWidget()
{
    makeCurrent();

    vbo_.destroy();
    vao_.destroy();

    doneCurrent();

    camera_.reset();
}

void TerrainViewWidget::SetRenderWireframe(bool wireframe)
{
    render_wireframe_ = wireframe;
    update();
}

void TerrainViewWidget::OnHeightmapUpdated(std::shared_ptr<QPixmap> heightmap)
{
    qDebug() << "CONNECTED";
}

void TerrainViewWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_prog_->Id());
    vao_.bind();

    unsigned int modelLoc = glGetUniformLocation(shader_prog_->Id(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix_));

    unsigned int viewLoc = glGetUniformLocation(shader_prog_->Id(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camera_->GetViewMatrixValuePtr());

    unsigned int projLoc = glGetUniformLocation(shader_prog_->Id(), "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, camera_->GetProjectionMatrixValuePtr());

    if(render_wireframe_ == true)
    {
        for(int i = 0; i < terrain_verts_->size(); i += 3)
        {
            glDrawArrays(GL_LINE_LOOP, i, 3);
        }
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)terrain_verts_->size());
    }
}

void TerrainViewWidget::keyPressEvent(QKeyEvent* event)
{
    Directions dir = Directions::kNone;

    if(event->key() == Qt::Key_S)
    {
        dir =  dir | Directions::kBackward;
    }
    if(event->key() == Qt::Key_W)
    {
        dir = dir | Directions::kForward;
    }

    if(event->key() == Qt::Key_D)
    {
        dir = dir | Directions::kRight;
    }
    if(event->key() == Qt::Key_A)
    {
        dir = dir | Directions::kLeft;
    }

    if(event->key() == Qt::Key_Q)
    {
        dir = dir | Directions::kDown;
    }
    if(event->key() == Qt::Key_E)
    {
        dir = dir | Directions::kUp;
    }

    camera_->Move(dir);

    update();
}

void TerrainViewWidget::mousePressEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::MouseButton::RightButton) == Qt::MouseButton::RightButton)
    {
        is_dragging_ = true;
        drag_start_ = glm::vec2(event->pos().x(), event->pos().y());
    }
}

void TerrainViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(is_dragging_ == true)
    {
        glm::vec2 drag_end = glm::vec2(event->pos().x(), event->pos().y());

        glm::vec2 offset = glm::vec2(
                    drag_end.x - drag_start_.x,
                    drag_start_.y - drag_end.y);

        camera_->Rotate(offset);
        update();

        drag_start_ = drag_end;
    }
}

void TerrainViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if((event->buttons() & Qt::MouseButton::RightButton) != Qt::MouseButton::RightButton)
    {
        is_dragging_ = false;
    }
}

bool TerrainViewWidget::CompileShaders_()
{
    bool success = false;

    std::shared_ptr<Shader> vert = std::make_shared<Shader>(
                "shaders/default_vert.glsl",
                ShaderType::kVertex);
    success = vert->Compile();

    std::shared_ptr<Shader> frag = std::make_shared<Shader>(
                "shaders/default_frag.glsl",
                ShaderType::kFragment);
    success &= frag->Compile();

    shader_prog_ = std::make_unique<ShaderProgram>(vert, frag);
    success &= shader_prog_->Link();

    return success;
}
