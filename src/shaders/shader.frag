#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec4 vert;

layout(location = 0) out vec4 outColor;

void main() {
    float y = -vert.y;
	
	if(y <= 0.5) {
		outColor = vec4(0.871, 0.878, 0.706, 1.0);
	}
	else if(y > 0.5 && y <= 5.0) {
		outColor = vec4(0.114, 0.420, 0.153, 1.0);
	}
	else if(y > 5.0f && y <= 7.0) {
		outColor = vec4(0.710, 0.710, 0.710, 1.0);
	}
	else {
		outColor = vec4(0.969, 1.0, 0.980, 1.0);
	}
}