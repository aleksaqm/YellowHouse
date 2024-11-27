#version 330 core

layout(location = 0) in vec2 aPos;

uniform float uTime;
uniform vec2 uStartPosition;
uniform float uParticleLife;

out vec2 fragCoord;

void main()
{
    float lifeProgress = mod(uTime, uParticleLife) / uParticleLife;

    float verticalOffset = lifeProgress * 0.5;

    float windEffect = sin(uTime * 3.0 + gl_InstanceID) * 0.1 * lifeProgress;

    float scale = mix(0.1, 0.5, lifeProgress);

    vec2 transformedPos = vec2(aPos.x * scale + windEffect, aPos.y * scale + verticalOffset);

    transformedPos += uStartPosition;

    gl_Position = vec4(transformedPos, 0.0, 1.0);
    fragCoord = aPos;
}