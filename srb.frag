#version 330 core

uniform float uTransparency;

in vec3 chCol;
out vec4 outCol;

void main()
{
	outCol = vec4(chCol, uTransparency);
}