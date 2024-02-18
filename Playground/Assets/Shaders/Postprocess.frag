#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

layout(location = 0) in vec2 InUVCoord;

layout(location = 0) out vec4 OutColor;

layout(set = 0, binding = 1) uniform RenderTargetUBO 
{
    vec4 RTSizeFull;
} UBO;

layout(set = 1, binding = 0) uniform sampler2D CombinedTexture;

void main()
{
    vec3 FragColor = texture(CombinedTexture, InUVCoord).rgb;

    FragColor      = pow(FragColor, vec3(INV_GAMMA));

    OutColor       = vec4(FragColor.yxz, 1.0);
}
