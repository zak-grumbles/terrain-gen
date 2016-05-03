#version 120

uniform vec3 lightPos;
uniform sampler2D grass;
uniform sampler2D grass_bmp;

void main(){
	vec3 normal = normalize(texture2D(grass_bmp, gl_TexCoord[0].st).rgb);
	vec3 light_pos = normalize(vec3(lightPos.x, lightPos.y, lightPos.z));

	float diff = max(dot(normal, light_pos), 0.0);

	vec3 final_color = diff * texture2D(grass, gl_TexCoord[0].st).rgb;

	gl_FragColor = vec4(final_color, 1.0);
}
