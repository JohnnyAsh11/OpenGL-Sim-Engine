#version 330

in vec3 Color;
in vec3 Normal;
in vec2 UV;

out vec4 Fragment;

uniform sampler2D Texture;
uniform sampler2D NormalMap;

void main()
{
    Fragment = texture(Texture, UV);
}