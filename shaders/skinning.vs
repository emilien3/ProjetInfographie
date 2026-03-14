#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec2 aBoneIDs;
layout (location = 4) in vec2 aWeights; 

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

out vec2 BoneWeights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    mat4 boneTransform = finalBonesMatrices[aBoneIDs[0]] * aWeights[0] + finalBonesMatrices[aBoneIDs[1]] * aWeights[1];

    vec4 totalPosition = boneTransform * vec4(aPos, 1.0);
    
    gl_Position = projection * view * model * totalPosition;
    
    Normal = mat3(model * boneTransform) * aNormal; 
    FragPos = vec3(model * totalPosition);
    TexCoords = aTexCoords;

    BoneWeights = aWeights;
}

