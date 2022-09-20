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

class TerrainViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    TerrainViewWidget(QWidget* parent);
    ~TerrainViewWidget();

public slots:
    void generate();
    void on_generation_progress(float percent);
    void on_terrain_generated(std::vector<glm::vec3>* verts);
    void on_gen_thread_finished();

    void set_render_wireframe(bool wireframe);
    void set_cube_size(double new_size);
    void set_grid_size(int new_size);

signals:
    void status_update(const QString& msg);
    void progress_update(int value);

protected:
    // QOpenGLWidget overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Helper methods
    bool compile_shaders();

    QThread generator_thread_;

    std::unique_ptr<std::vector<glm::vec3>> terrain_verts_;

    glm::vec3 camera_position_;
    glm::mat4 projection_matrix_;
    glm::mat4 model_matrix_;
    glm::mat4 view_matrix_;

    unsigned int shader_prog_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer vbo_;

    float cube_size_ = 1.0f;
    int grid_size_ = 64;
    bool render_wireframe_ = false;
};

#endif // TERRAINVIEWWIDGET_H
