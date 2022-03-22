// TestShader.glsl

#type vertex
#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec4 VertexColor;

void main()
{
	VertexColor = vec4(inColor, 1.0f);
	gl_Position = vec4(inPos, 1.0f);
}


#type fragment
#version 460 core

in vec4 VertexColor;
out vec4 FragColor;

void main()
{
	FragColor = VertexColor;
}
