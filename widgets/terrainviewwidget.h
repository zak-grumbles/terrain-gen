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
    void onGenerationProgress(float percent);
    void onTerrainGenerated(std::vector<glm::vec3>* verts);
    void onGenThreadFinished();

    void setRenderWireframe(bool wireframe);
    void setCubeSize(double newSize);
    void setGridSize(int newSize);

signals:
    void statusUpdate(const QString& msg);
    void progressUpdate(int value);

protected:
    // QOpenGLWidget overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Helper methods
    bool compileShaders();

    QThread generatorThread;

    std::unique_ptr<std::vector<glm::vec3>> terrainVerts;

    glm::vec3 cameraPosition;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;

    unsigned int shaderProg;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;

    float cubeSize = 1.0f;
    int gridSize = 64;
    bool renderWireframe = true;
};

#endif // TERRAINVIEWWIDGET_H
