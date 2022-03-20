// TestShader.glsl

#type vertex
#version 460 core

layout (location = 0) in vec4 inPos;

void main()
{
	gl_Position = inPos;
}


#type fragment
#version 460 core

uniform vec3 u_Color;

out vec4 Color;

void main()
{
	Color = vec4(u_Color, 1.0f);
}
