#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

in vec2 BoneWeights; 

void main()
{    
    FragColor = vec4(BoneWeights.x, BoneWeights.y, 0.0, 1.0);
}