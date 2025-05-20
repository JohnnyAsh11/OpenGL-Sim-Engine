#version 330 core
layout(location = 0) in vec3 position_b;

out vec3 TextureCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TextureCoordinates = position_b;
    vec4 pos = projection * view * vec4(position_b, 1.0);
    gl_Position = pos.xyww;
}