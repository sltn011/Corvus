#version 450

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexColor;

layout(location = 0) out vec3 FragColor;

layout(set = 0, binding = 0) uniform MatricesUBO {
    mat4 Model;
    mat4 ProjectionView;
} UBO;

void main()
{
    gl_Position = UBO.ProjectionView * UBO.Model * vec4(VertexPos, 1.0);
    FragColor = VertexColor;
}
