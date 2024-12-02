#version 330 core

out vec4 FragColor;
uniform float uTime;

void main()
{
    float alpha = max(1.0 - (uTime * 0.3), 0.0); 

    FragColor = vec4(1.0, 1.0, 1.0, alpha);
}