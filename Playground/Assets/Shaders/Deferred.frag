#version 450

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InUVCoord;
layout(location = 2) in vec3 InNormal;

layout(location = 0) out vec4 OutPosition;
layout(location = 1) out vec4 OutAlbedo;
layout(location = 2) out vec4 OutNormal;

layout(set = 1, binding = 0) uniform sampler2D TextureAlbedo;

void main()
{
    OutPosition = vec4(InPosition, 1.f);
    OutAlbedo   = texture(TextureAlbedo, InUVCoord);
    OutNormal   = vec4(InNormal, 1.f);
}
