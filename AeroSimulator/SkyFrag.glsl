#version 330 core

in vec3 TextureCoordinates;
out vec4 FragColor;

uniform samplerCube Skybox;

void main()
{    
    FragColor = texture(Skybox, TextureCoordinates);
}