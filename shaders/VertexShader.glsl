#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 color;

//uniform float scale;

void main()
{
	//gl_Position = vec4(vPos.x * scale, vPos.y * scale, vPos.z * scale, 1.0);
	gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);
	color = vColor;
}