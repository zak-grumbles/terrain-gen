#ifndef TERRAINVIEWWIDGET_H
#define TERRAINVIEWWIDGET_H

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QThread>
#include <QWidget>

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

class TerrainViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    TerrainViewWidget(QWidget* parent);

public slots:
    void generate();
    void onGenerationProgress(float percent);
    void onTerrainGenerated(std::vector<glm::vec3>* verts);
    void onGenThreadFinished();

signals:
    void statusUpdate(const QString& msg);

protected:
    // QOpenGLWidget overrides
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    QThread generatorThread;

    std::unique_ptr<std::vector<glm::vec3>> terrainVerts;

    glm::vec3 cameraPosition;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
};

#endif // TERRAINVIEWWIDGET_H
