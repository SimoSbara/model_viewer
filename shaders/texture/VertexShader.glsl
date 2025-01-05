#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vUV;
layout (location = 3) in vec3 vNorm;

out vec3 normal;
out vec3 curPos;
out vec2 texCoord;

uniform float scale;
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	vec3 finalPos = vec3(vPos.x * scale, vPos.y * scale, vPos.z * scale);
	curPos = vec3(model * vec4(finalPos, 1.0f));

	gl_Position = camMatrix * vec4(curPos, 1.0);

	normal = mat3(transpose(inverse(model))) * vNorm;
	texCoord = vUV;
}