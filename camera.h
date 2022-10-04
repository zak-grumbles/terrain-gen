#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QKeyEvent>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(glm::vec3 cam_position,
           float fov,
           float aspect_ratio,
           float near,
           float far);

    const glm::mat4& GetViewMatrix() const { return view_matrix_; }
    const float* GetViewMatrixValuePtr() const { return glm::value_ptr(view_matrix_); }

    const glm::mat4& GetProjectionMatrix() const { return projection_matrix_; }
    const float* GetProjectionMatrixValuePtr() const { return glm::value_ptr(projection_matrix_); }

    const glm::vec3& GetPosition() const { return pos_; }

    void LookAt(glm::vec3 target);
    void SetAspectRatio(float aspect_ratio);

protected:
    void BuildProjectionMatrix_();

    glm::vec3 pos_;
    float fov_;
    float aspect_ratio_;
    float near_;
    float far_;

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
};

#endif // CAMERA_H
