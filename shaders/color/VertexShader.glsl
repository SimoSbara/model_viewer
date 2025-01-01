#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 3) in vec3 vNorm;

out vec3 normal;
out vec3 fragPos;

uniform float scale;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main()
{
	vec3 finalPos = vec3(vPos.x * scale, vPos.y * scale, vPos.z * scale);

	gl_Position = proj * view * model * vec4(finalPos, 1.0);
	fragPos = vec3(model * vec4(finalPos, 1.0));

	normal = mat3(transpose(inverse(model))) * vNorm;
	//normal = vNorm;
}