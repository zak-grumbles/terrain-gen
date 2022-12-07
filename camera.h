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
Directions operator&(Directions lhs, Directions rhs);

class Camera
{
public:
    Camera(glm::vec3 cam_position,
           float fov,
           float aspect_ratio,
           float near,
           float far);

    const glm::mat4& ViewMatrix() const { return view_matrix_; }
    const float* GetViewMatrixValuePtr() const { return glm::value_ptr(view_matrix_); }

    const glm::mat4& ProjectionMatrix() const { return projection_matrix_; }
    const float* GetProjectionMatrixValuePtr() const { return glm::value_ptr(projection_matrix_); }

    const glm::vec3& Position() const { return pos_; }

    const glm::vec3& LookVector() const { return look_; }

    void LookAt(glm::vec3 target);
    void SetAspectRatio(float aspect_ratio);

    void Move(Directions dir);
    void Rotate(glm::vec2 offset);

protected:
    void BuildProjectionMatrix_();
    void BuildViewMatrix_();

    glm::vec3 pos_;
    glm::vec3 look_;
    glm::vec3 up_ = glm::vec3(0, 1, 0);
    glm::vec3 world_up_ = glm::vec3(0, 1, 0);

    float yaw_ = -90.0f;
    float pitch_ = 0.0f;
    float move_speed_ = 2.5f;
    float rotate_speed_ = 0.1f;

    float fov_;
    float aspect_ratio_;
    float near_;
    float far_;

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
};

#endif // CAMERA_H
