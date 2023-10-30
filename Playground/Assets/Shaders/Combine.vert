#version 450

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUVCoord;

layout(location = 0) out vec2 UVCoord;


void main()
{
    gl_Position = vec4(VertexPos, 1.0);
    UVCoord = VertexUVCoord;
}
