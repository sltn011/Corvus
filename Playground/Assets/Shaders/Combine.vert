#version 450

layout(location = 0) in vec3 InVertexPos;
layout(location = 1) in vec2 InVertexUVCoord;

layout(location = 0) out vec2 OutVertexUVCoord;


void main()
{
    gl_Position         = vec4(InVertexPos, 1.0);
    OutVertexUVCoord    = InVertexUVCoord;
}
