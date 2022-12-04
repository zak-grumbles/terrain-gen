#include "camera.h"

#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

Directions operator|(Directions lhs, Directions rhs)
{
    return static_cast<Directions>(
        static_cast<int>(lhs) | static_cast<int>(rhs)
    );
}

Directions operator&(Directions lhs, Directions rhs)
{
    return static_cast<Directions>(
        static_cast<int>(lhs) & static_cast<int>(rhs)
    );
}

Camera::Camera(glm::vec3 cam_position,
               float fov,
               float aspect_ratio,
               float near,
               float far) :
    pos_(cam_position),
    fov_(fov),
    aspect_ratio_(aspect_ratio),
    near_(near),
    far_(far)  {

    BuildProjectionMatrix_();
}

void Camera::LookAt(glm::vec3 target) {
    look_ = glm::normalize(target - pos_);
    view_matrix_ = glm::lookAt(pos_, target, glm::vec3(0, 1, 0));
}

void Camera::SetAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    BuildProjectionMatrix_();
}

void Camera::Move(Directions dir)
{
    if((dir & Directions::kForward) == Directions::kForward)
    {
        pos_ += look_;
    }
    if((dir & Directions::kBackward) == Directions::kBackward)
    {
        pos_ -= look_;
    }

    glm::vec3 up = glm::vec3(0, 1, 0);
    if((dir & Directions::kUp) == Directions::kUp)
    {
        pos_ += up;
    }
    if((dir & Directions::kDown) == Directions::kDown)
    {
        pos_ -= up;
    }

    glm::vec3 right = glm::normalize(glm::cross(look_, up));
    if((dir & Directions::kLeft) == Directions::kLeft)
    {
        pos_ -= right;
    }
    if((dir & Directions::kRight) == Directions::kRight)
    {
        pos_ += right;
    }

    // Trigger recreation of view matrix
    LookAt(pos_ + look_);
}

void Camera::Rotate(float degrees, glm::vec3 axis)
{
    look_ = glm::normalize(glm::rotate(look_, glm::radians(degrees) * rotate_speed_, axis));
    view_matrix_ = glm::lookAt(pos_, pos_ + look_, glm::vec3(0, 1, 0));
}

void Camera::BuildProjectionMatrix_() {
    projection_matrix_ = glm::perspective(fov_, aspect_ratio_, near_, far_);
}
