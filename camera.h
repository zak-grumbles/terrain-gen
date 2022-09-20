#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 cam_position);

protected:
    glm::vec3 pos_;
    glm::mat4 view_matrix_;
};

#endif // CAMERA_H
