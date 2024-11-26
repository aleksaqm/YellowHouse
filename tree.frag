#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; //teksturna jedinica
uniform float uWhiteLevel;

void main()
{
	vec4 texColor = texture(uTex, chTex);
	vec3 whiteColor = vec3(1.0, 1.0, 1.0);

	if (chTex.y <= uWhiteLevel){
		texColor.rgb = mix(texColor.rgb, whiteColor, 0.5);
	}
	outCol = texColor;
}