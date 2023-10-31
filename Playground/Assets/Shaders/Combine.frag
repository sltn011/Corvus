#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

layout(location = 0) in vec2 InUVCoord;

layout(location = 0) out vec4 OutColor;

layout(set = 0, binding = 0) uniform sampler2D WorldPositionTexture;
layout(set = 0, binding = 1) uniform sampler2D AlbedoTexture;
layout(set = 0, binding = 2) uniform sampler2D NormalTexture;
layout(set = 0, binding = 3) uniform sampler2D DepthTexture;

void main()
{
    vec3 FragColor                  = texture(AlbedoTexture, InUVCoord).rgb;

    vec3 GammaCorrectedFragColor    = pow(FragColor, vec3(INV_GAMMA));
    OutColor                        = vec4(GammaCorrectedFragColor, 1.0);
}
