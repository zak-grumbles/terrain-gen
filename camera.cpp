#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

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

void Camera::Move(int directionFlags)
{
    if(directionFlags & CameraDirections::kForward)
    {
        pos_ += look_ * 0.25f;
    }
    else if(directionFlags & CameraDirections::kBackward)
    {
        pos_ -= look_ * 0.25f;
    }
    else if(directionFlags & CameraDirections::kRight)
    {
        glm::vec3 right = glm::normalize(glm::cross(look_, glm::vec3(0, 1, 0)));
        pos_ += right * 0.25f;
    }
    else if(directionFlags & CameraDirections::kLeft)
    {
        glm::vec3 right = glm::normalize(glm::cross(look_, glm::vec3(0, 1, 0)));
        pos_ -= right * 0.25f;
    }
    else if(directionFlags & CameraDirections::kUp)
    {
        pos_ += glm::vec3(0, 0.25f, 0);
    }
    else if(directionFlags & CameraDirections::kDown)
    {
        pos_ -= glm::vec3(0, 0.25f, 0);
    }

    BuildViewMatrix_();
}

void Camera::BuildProjectionMatrix_()
{
    projection_matrix_ = glm::perspective(fov_, aspect_ratio_, near_, far_);
}

void Camera::BuildViewMatrix_()
{
    view_matrix_ = glm::lookAt(pos_, pos_ + look_, glm::vec3(0, 1, 0));
}
