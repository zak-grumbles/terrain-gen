#ifndef TG_VERTEX_H
#define TG_VERTEX_H

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include <array>

struct Vertex {
	glm::vec3 pos;

	static vk::VertexInputBindingDescription binding_description();
	static std::array<vk::VertexInputAttributeDescription, 1> attribute_descriptions();

	Vertex(glm::vec3 position);
};

struct UniformBufferObject {
	alignas(16)glm::mat4 model;
	alignas(16)glm::mat4 view;
	alignas(16)glm::mat4 proj;
};

#endif