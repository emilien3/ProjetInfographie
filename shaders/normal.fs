#version 330 core

out vec4 FragColor;

in vec3 Normal;

void main()
{
    vec3 mappedNormal = (Normal + 1.0) / 2.0;
    FragColor = vec4(mappedNormal, 1.0);
}