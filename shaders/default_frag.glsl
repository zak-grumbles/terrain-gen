#version 330 core

in vec4 pos;
in vec3 original_pos;

out vec4 FragColor;

void main()
{
	if(original_pos.y <= 4.0f)
	{
		FragColor = vec4(0.871f, 0.878f, 0.706f, 1.0f);
	}
	else if(original_pos.y > 4.0f && original_pos.y <= 15.0f)
	{
		FragColor = vec4(0.114f, 0.420f, 0.153f, 1.0f);
	}
	else if(original_pos.y > 15.0f && original_pos.y <= 25.0f)
	{
		FragColor = vec4(0.710f, 0.710f, 0.710f, 1.0f);
	}
	else if(original_pos.y > 25.0f)
	{
		FragColor = vec4(0.969f, 1.0f, 0.980f, 1.0f);
	}
}