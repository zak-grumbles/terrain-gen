#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QKeyEvent>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Directions {
    kForward 	= 1 << 0,
    kBackward 	= 1 << 1,
    kLeft 		= 1 << 2,
    kRight 		= 1 << 3,
    kUp 		= 1 << 4,
    kDown 		= 1 << 5,
    kNone		= 1 << 6
};

Directions operator|(Directions lhs, Directions rhs);

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

    void Move(Directions dir);

protected:
    void BuildProjectionMatrix_();

    glm::vec3 pos_;
    glm::vec3 look_;
    float fov_;
    float aspect_ratio_;
    float near_;
    float far_;

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
};

#endif // CAMERA_H
