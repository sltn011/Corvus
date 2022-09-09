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

struct Material
{
	sampler2D AlbedoMap;
	vec4      AlbedoValue;
	bool      bHasAlbedoMap;
	
	sampler2D NormalMap;
	vec3      NormalValue;
	bool      bHasNormalMap;
	
	sampler2D RoughnessMap;
	float     RoughnessValue;
	bool      bHasRoughnessMap;
	
	sampler2D MetallicMap;
	float     MetallicValue;
	bool      bHasMetallicMap;
};
uniform Material u_Material;

in vec2 TextureCoord;
out vec4 FragColor;

void main()
{
	vec4 Color = vec4(u_Material.bHasAlbedoMap ? texture(u_Material.AlbedoMap, TextureCoord) : u_Material.AlbedoValue);
	FragColor = Color;
}
