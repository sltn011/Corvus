#version 450

layout(location = 0) in vec3 InVertexPos;
layout(location = 1) in vec2 InVertexUVCoord;
layout(location = 2) in vec3 InVertexNormal;
layout(location = 3) in vec4 InVertexTangent;

layout(location = 0) out vec3 OutVertexPos;
layout(location = 1) out vec2 OutVertexUVCoord;
layout(location = 2) out mat3 OutTBN;

layout(set = 0, binding = 0) uniform CameraUBO 
{
    mat4 ProjectionView;
    mat4 View;
    mat4 Projection;
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

    vec3 T              = normalize(mat3(PushConstants.Model) * InVertexTangent.xyz);
	vec3 N              = normalize(mat3(PushConstants.Model) * InVertexNormal);
	T                   = normalize(T - dot(T, N) * N);
	vec3 B              = cross(N, T);
	OutTBN              = mat3(T, B, N);
}
