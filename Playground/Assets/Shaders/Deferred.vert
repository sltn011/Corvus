#version 450

layout(location = 0) in vec3 InVertexPos;
layout(location = 1) in vec2 InVertexUVCoord;
layout(location = 2) in vec3 InNormal;

layout(location = 0) out vec3 OutVertexPos;
layout(location = 1) out vec2 OutVertexUVCoord;
layout(location = 2) out vec3 OutNormal;

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
    vec4 WorldPosition  = PushConstants.Model * vec4(InVertexPos, 1.0);

    gl_Position         = UBO.ProjectionView * WorldPosition;

    OutVertexPos        = WorldPosition.xyz;
    OutVertexUVCoord    = InVertexUVCoord;
    OutNormal           = normalize(mat3(PushConstants.Model) * InNormal);
}
