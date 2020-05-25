#ifndef TG_CAMERA_H
#define TG_CAMERA_H

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtx/quaternion.hpp>

class TGCamera {

private:
	glm::vec3 position_;
	glm::vec3 look_;
	glm::vec3 up_;

public:
	TGCamera();
	TGCamera(TGCamera& c);
	TGCamera(glm::vec3 pos, glm::vec3 target);

	~TGCamera();

	glm::mat4& view_matrix() const noexcept;

	void move_up() noexcept;
	void move_down() noexcept;
	void move_left() noexcept;
	void move_right() noexcept;
	void move_forward() noexcept;
	void move_backward() noexcept;

	void move(glm::vec3 direction) noexcept;
	void look_at(glm::vec3 target) noexcept;
};

#endif