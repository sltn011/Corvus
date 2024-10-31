#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

layout(location = 0) in vec2 InUVCoord;

layout(location = 0) out vec4 OutColor;

layout(set = 1, binding = 0) uniform sampler2D WorldPositionTexture;
layout(set = 1, binding = 1) uniform sampler2D AlbedoTexture;
layout(set = 1, binding = 2) uniform sampler2D NormalTexture;
layout(set = 1, binding = 3) uniform sampler2D DepthTexture;

void main()
{
    vec3 FragColor = texture(AlbedoTexture, InUVCoord).rgb;
    OutColor       = vec4(FragColor, 1.0);
}
