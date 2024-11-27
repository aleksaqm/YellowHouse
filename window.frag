#version 330 core

uniform float uTransparency;

in vec3 chCol;
out vec4 outCol;
uniform float uDarkening;

void main()
{
	outCol = vec4(uDarkening*chCol, uTransparency);
}