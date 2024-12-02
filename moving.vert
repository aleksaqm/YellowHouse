#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje
out vec2 chTex;

uniform float uXpos;
uniform float uFlip; // 1.0 za normalno, -1.0 za horizontalno preslikano

void main()
{
	gl_Position = vec4(inPos.x + uXpos, inPos.y, 0.0, 1.0);
	chTex = vec2(inTex.x * uFlip, inTex.y);
}