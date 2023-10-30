#version 450

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUVCoord;

layout(location = 0) out vec3 WorldPosition;
layout(location = 1) out vec2 UVCoord;

layout(set = 0, binding = 0) uniform MatricesUBO 
{
    mat4 ProjectionView;
} UBO;

layout(push_constant) uniform constants
{
    mat4 Model;
} PushConstants;

void main()
{
    vec4 Position = PushConstants.Model * vec4(VertexPos, 1.0);
    gl_Position = UBO.ProjectionView * Position;
    WorldPosition = Position.xyz;
    UVCoord = VertexUVCoord;
}
