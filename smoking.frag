#version 330 core

out vec4 FragColor;

uniform float uTime;
uniform float uParticleLife;
in vec2 fragCoord;

void main()
{
    float distanceFromCenter = length(fragCoord);

    float lifeProgress = mod(uTime, uParticleLife) / uParticleLife;

    float alpha = (1.0 - lifeProgress) * smoothstep(0.5, 0.0, distanceFromCenter);

    if (alpha < 0.01) 
        discard;  

    FragColor = vec4(vec3(0.5), alpha);
}