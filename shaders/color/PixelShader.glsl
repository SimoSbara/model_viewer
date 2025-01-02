#version 330 core

in vec3 normal;
in vec3 curPos;
in vec3 color;
out vec4 fragColor;

uniform vec3 lightDir;

void main()
{
	//vec3 lightPos = vec3(0.0f, 2.0f, 0.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(lightPos - curPos);

	float diffuse = max(dot(norm, lightDir), 0.0f);

	fragColor = vec4(color, 1.0f) * vec4(lightColor, 1.0f) * diffuse;
}