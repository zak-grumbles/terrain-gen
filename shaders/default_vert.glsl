#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 pos;
out vec3 original_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	mat4 mvp = proj * view * model;
	original_pos = aPos;
	pos = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = pos;
}