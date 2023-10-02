#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

layout(location = 0) in vec3 FragColor;

layout(location = 0) out vec4 OutColor;

void main()
{
    vec3 GammaCorrectedFragColor = pow(FragColor, vec3(INV_GAMMA));
    OutColor = vec4(GammaCorrectedFragColor, 1.0);
}
