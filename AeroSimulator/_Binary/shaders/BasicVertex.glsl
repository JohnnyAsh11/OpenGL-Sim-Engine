#version 330

layout (location = 0) in vec3 Position_b;
layout (location = 1) in vec3 Color_b;
layout (location = 2) in vec2 UV_b;
layout (location = 3) in vec3 Normal_b;

uniform mat4 WVP;
uniform mat4 InverseTransposeWorld;

out vec3 Color;
out vec3 Normal;
out vec2 UV;

void main()
{
    gl_Position = WVP * vec4(Position_b, 1.0f);
	
    Color = Color_b;
    Normal = normalize(mat3(InverseTransposeWorld) * Normal_b);
    //Normal = Normal_b;
    UV = UV_b;
}