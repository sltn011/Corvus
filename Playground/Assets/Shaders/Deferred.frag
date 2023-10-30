#version 450

layout(location = 0) in vec3 WorldPosition;
layout(location = 1) in vec2 UVCoord;

layout(location = 0) out vec4 OutPosition;
layout(location = 1) out vec4 OutAlbedo;
layout(location = 2) out vec4 OutNormal;

layout(set = 1, binding = 0) uniform sampler2D Albedo;

void main()
{
    OutPosition = vec4(WorldPosition, 1.f);
    OutAlbedo = texture(Albedo, UVCoord);
    OutNormal = vec4(0.f, 0.f, 1.f, 1.f);
}
