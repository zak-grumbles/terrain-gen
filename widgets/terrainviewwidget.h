#ifndef TERRAINVIEWWIDGET_H
#define TERRAINVIEWWIDGET_H

#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QThread>
#include <QWidget>

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include "camera.h"
#include "shader.h"

class TerrainViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    TerrainViewWidget(QWidget* parent);
    ~TerrainViewWidget();

public slots:
    void Generate();
    void OnGenerationProgress(float percent);
    void OnTerrainGenerated(std::vector<glm::vec3>* verts);
    void OnGenThreadFinished();

    void SetRenderWireframe(bool wireframe);
    void SetCubeSize(double new_size);
    void SetGridSize(int new_size);

signals:
    void StatusUpdate(const QString& msg);
    void ProgressUpdate(int value);

protected:
    // QOpenGLWidget overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Keyboard event overrides
    void keyPressEvent(QKeyEvent* event) override;

    // Mouse event overrides
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    // Helper methods
    bool CompileShaders_();

    QThread generator_thread_;

    std::unique_ptr<std::vector<glm::vec3>> terrain_verts_;

    std::unique_ptr<Camera> camera_;

    glm::mat4 model_matrix_;

    std::unique_ptr<ShaderProgram> shader_prog_ = nullptr;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer vbo_;

    float cube_size_ = 1.0f;
    int grid_size_ = 64;
    bool render_wireframe_ = false;

    // Mouse variables
    bool is_dragging_ = false;
    glm::vec3 drag_start_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif // TERRAINVIEWWIDGET_H
