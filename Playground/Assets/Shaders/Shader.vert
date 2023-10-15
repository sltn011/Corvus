#version 450

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexColor;

layout(location = 0) out vec3 FragColor;

layout(set = 0, binding = 0) uniform MatricesUBO {
    mat4 ProjectionView;
} UBO;

layout(push_constant) uniform constants
{
    mat4 Model;
} PushConstants;

void main()
{
    gl_Position = UBO.ProjectionView * PushConstants.Model * vec4(VertexPos, 1.0);
    FragColor = VertexColor;
}
