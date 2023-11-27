#ifndef TERRAINVIEWWIDGET_H
#define TERRAINVIEWWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QThread>
#include <QWidget>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "camera.h"
#include "shader.h"

class TerrainViewWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    TerrainViewWidget(QWidget* parent);
    ~TerrainViewWidget();

public slots:
    void SetRenderWireframe(bool wireframe);
    void OnHeightmapUpdated(std::shared_ptr<QPixmap> heightmap);

signals:
    void StatusUpdate(const QString& msg);

protected:
    // QOpenGLWidget overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Keyboard event overrides
    void keyPressEvent(QKeyEvent* event) override;

    // Mouse event overrides
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    // Helper methods
    bool CompileShaders_();
    void UpdateBuffers_();

    std::unique_ptr<std::vector<glm::vec3>> terrain_verts_;
    std::unique_ptr<std::vector<unsigned int>> terrain_indices_;

    std::unique_ptr<Camera> camera_;

    glm::mat4 model_matrix_;

    std::unique_ptr<ShaderProgram> shader_prog_ = nullptr;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer vbo_;
    QOpenGLBuffer ebo_;

    bool render_wireframe_ = false;

    // Mouse variables
    bool is_dragging_     = false;
    glm::vec2 drag_start_ = glm::vec2(0.0, 0.0);
};

#endif  // TERRAINVIEWWIDGET_H
