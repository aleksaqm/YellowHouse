#version 330 core

layout(location = 0) in vec2 aPos;
uniform float uTime; 

void main()
{
    float verticalOffset = aPos.y + uTime * 0.5;

    float horizontalOffset = sin(uTime * 2.0) * 0.1;

    vec2 transformedPos = vec2(aPos.x + horizontalOffset, verticalOffset);

    gl_Position = vec4(transformedPos, 0.0, 1.0);
}