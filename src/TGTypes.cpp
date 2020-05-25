#include "TGTypes.h"

vk::VertexInputBindingDescription Vertex::binding_description() {
	vk::VertexInputBindingDescription desc(
		0,
		sizeof(Vertex),
		vk::VertexInputRate::eVertex
	);

	return desc;
}

std::array<vk::VertexInputAttributeDescription, 1> Vertex::attribute_descriptions() {

	std::array<vk::VertexInputAttributeDescription, 1> descriptions;

	// inPosition
	descriptions[0].binding = 0;
	descriptions[0].location = 0;
	descriptions[0].format = vk::Format::eR32G32B32Sfloat;
	descriptions[0].offset = offsetof(Vertex, pos);

	return descriptions;
}

Vertex::Vertex(glm::vec3 position) : pos(position) { 
}
