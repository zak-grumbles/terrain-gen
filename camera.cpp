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
    BuildViewMatrix_();
}

void Camera::LookAt(glm::vec3 target) {
    look_ = glm::normalize(target - pos_);
    BuildViewMatrix_();
}

void Camera::SetAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    BuildProjectionMatrix_();
}

void Camera::Move(Directions dir)
{
    if((dir & Directions::kForward) == Directions::kForward)
    {
        pos_ += look_ * move_speed_;
    }
    if((dir & Directions::kBackward) == Directions::kBackward)
    {
        pos_ -= look_ * move_speed_;
    }

    glm::vec3 up = glm::vec3(0, 1, 0);
    if((dir & Directions::kUp) == Directions::kUp)
    {
        pos_ += up * move_speed_;
    }
    if((dir & Directions::kDown) == Directions::kDown)
    {
        pos_ -= up * move_speed_;
    }

    glm::vec3 right = glm::normalize(glm::cross(look_, up));
    if((dir & Directions::kLeft) == Directions::kLeft)
    {
        pos_ -= right * move_speed_;
    }
    if((dir & Directions::kRight) == Directions::kRight)
    {
        pos_ += right * move_speed_;
    }

    // Trigger recreation of view matrix
    BuildViewMatrix_();
}

void Camera::Rotate(glm::vec2 offset)
{
    offset *= rotate_speed_;

    yaw_ += offset.x;
    pitch_ += offset.y;

    look_ = glm::vec3(
                glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_)),
                glm::sin(glm::radians(pitch_)),
                glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_)));

    look_ = glm::normalize(look_);
    auto right = glm::normalize(glm::cross(look_, world_up_));
    up_ = glm::normalize(glm::cross(right, look_));

    BuildViewMatrix_();
}

void Camera::BuildProjectionMatrix_() {
    projection_matrix_ = glm::perspective(fov_, aspect_ratio_, near_, far_);
}

void Camera::BuildViewMatrix_()
{
    view_matrix_ = glm::lookAt(pos_, pos_ + look_, up_);
}
