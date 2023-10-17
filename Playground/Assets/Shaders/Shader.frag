#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

layout(location = 0) in vec2 UVCoord;

layout(location = 0) out vec4 OutColor;

layout(set = 1, binding = 0) uniform sampler2D Texture;

void main()
{
    vec3 FragColor = texture(Texture, UVCoord).xyz;
    vec3 GammaCorrectedFragColor = pow(FragColor, vec3(INV_GAMMA));
    OutColor = vec4(GammaCorrectedFragColor, 1.0);
}
