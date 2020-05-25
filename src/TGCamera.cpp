#include "TGCamera.h"

#include <glm/gtc/matrix_transform.hpp>

TGCamera::TGCamera() {
	position_ = glm::vec3(0.0, 0.0, 0.0);
	look_ = glm::vec3(0.0, 0.0, -1.0);
	up_ = glm::vec3(0.0, 1.0, 0.0);
}

TGCamera::TGCamera(TGCamera& c) {
	position_ = c.position_;
	look_ = c.look_;
	up_ = c.up_;
}

TGCamera::TGCamera(glm::vec3 pos, glm::vec3 target) {

	position_ = pos;
	look_ = glm::normalize(target - pos);
	up_ = glm::vec3(0.0, 1.0, 0.0);
}

TGCamera::~TGCamera() {
	// no action needed
}

glm::mat4& TGCamera::view_matrix() const noexcept {
	glm::vec3 target = position_ + look_;
	return glm::lookAt(
		position_,
		target,
		up_
	);
}

void TGCamera::move_up() noexcept {
	move(0.25f * up_);
}

void TGCamera::move_down() noexcept {
	move(-0.25f * up_);
}
void TGCamera::move_left() noexcept {
	glm::vec3 right = glm::normalize(glm::cross(look_, up_));
	move(-0.25f * right);
}
void TGCamera::move_right() noexcept {
	glm::vec3 right = glm::normalize(glm::cross(look_, up_));
	move(0.25f * right);
}

void TGCamera::move_forward() noexcept {
	glm::vec3 right = glm::normalize(glm::cross(look_, up_));
	glm::vec3 forward = glm::normalize(glm::cross(up_, right));

	move(0.25f * forward);
}

void TGCamera::move_backward() noexcept {
	glm::vec3 right = glm::normalize(glm::cross(look_, up_));
	glm::vec3 forward = glm::normalize(glm::cross(up_, right));

	move(-0.25f * forward);
}

void TGCamera::move(glm::vec3 direction) noexcept {
	position_ += direction;
}

void TGCamera::look_at(glm::vec3 target) noexcept {
	look_ = glm::normalize(position_ - target);
}
