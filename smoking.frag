#version 330 core

out vec4 FragColor;
uniform float uTime;
uniform float uParticleLife;

void main()
{
    float lifeProgress = mod(uTime, uParticleLife) / uParticleLife;

    float alpha = 1.0 - lifeProgress;

    FragColor = vec4(vec3(0.5), alpha);
}