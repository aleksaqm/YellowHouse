#version 330 core

out vec4 FragColor;

uniform float uParticleLife;

in vec2 fragCoord;
in float vLifeProgress;

void main()
{
    float distanceFromCenter = length(fragCoord);

    float alpha = (1.0 - vLifeProgress) * smoothstep(0.5, 0.0, distanceFromCenter);

    if (alpha < 0.01)
        discard;

    float brightness = mix(0.2, 0.8, vLifeProgress);
    FragColor = vec4(vec3(brightness), alpha);
}