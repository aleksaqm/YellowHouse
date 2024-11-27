#version 330 core

out vec4 outCol;
uniform float uDarkening;

void main()
{
	vec3 rgb = vec3(0.54, 0.13, 0.0);

	outCol = vec4(uDarkening*rgb, 1.0);
}