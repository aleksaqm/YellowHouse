#version 330 core

out vec4 outCol;

uniform float uPulse;

void main()
{
	outCol = vec4(1.0, 1.0, uPulse, 1.0);
}