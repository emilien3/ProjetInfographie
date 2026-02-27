
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform float refractionRatio;

void main()
{             
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), refractionRatio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
} 