

const char* vertexSource =
"#version 150 core\n"
"void main(){"
"	gl_TexCoord[0] = gl_MultiTexCoord0;"
"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"}";

const char* fragmentSource =
"#version 150 core\n"
"uniform vec3 light;"
"uniform sampler2D tex;"
"uniform sampler2D bmp;"

"void main(){"
"	vec3 norm = normalize(texture(tex, gl_TexCoord[0].st).rgb * 2.0 - 1.0;"
"	vec3 light_dir = normalize(vec3(light.x, light.y, light.z));"
"	float diffuse = max(dot(norm, light_dir), 0.0);"
"	vec3 final_color = diffuse * texture(tex, gl_TexCoord[0].st).rgb;"
"	gl_FragColor = vec4(final_color, 1.0);"
"}";