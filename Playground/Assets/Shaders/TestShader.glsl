// TestShader.glsl

#type vertex
#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTex;

uniform mat4 u_Transform;
uniform mat4 u_ProjView;

out vec2 TextureCoord;

void main()
{
	TextureCoord = inTex;
	gl_Position = u_ProjView * u_Transform * vec4(inPos, 1.0f);
}


#type fragment
#version 460 core

uniform sampler2D u_Texture;

in vec2 TextureCoord;
out vec4 FragColor;

void main()
{
	FragColor = texture(u_Texture, TextureCoord);
}
