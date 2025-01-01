#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 2) in vec2 vUV;

out vec2 texCoord;

uniform float scale;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main()
{
	gl_Position = proj * view * model * vec4(vPos.x * scale, vPos.y * scale, vPos.z * scale, 1.0);
	texCoord = vUV;
}