#version 330 core

in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform vec3 color;
uniform vec3 lightPos;

void main()
{
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = vec3(0.07f, 0.13f, 0.17f);
	vec3 lightPos = vec3(0.0f, 2.0f, 0.0f);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * color;
	//fragColor = vec4(color, 1.0f);
	fragColor = vec4(result, 1.0f);
}