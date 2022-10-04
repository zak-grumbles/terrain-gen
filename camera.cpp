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
    view_matrix_ = glm::lookAt(pos_, target, glm::vec3(0, 1, 0));
}

void Camera::SetAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    BuildProjectionMatrix_();
}

void Camera::BuildProjectionMatrix_() {
    projection_matrix_ = glm::perspective(fov_, aspect_ratio_, near_, far_);
}
