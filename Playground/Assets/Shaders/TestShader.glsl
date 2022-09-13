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

vec4  GetAlbedo(vec2 TextureCoord);
vec3  GetNormal(vec2 TextureCoord);
float GetRoughness(vec2 TextureCoord);
float GetMetallic(vec2 TextureCoord);

struct Material
{
#ifdef USE_ALBEDO_MAP
	sampler2D Albedo;
#endif
#ifdef USE_ALBEDO_VAL
	vec4 Albedo;
#endif

#ifdef USE_NORMAL_MAP
	sampler2D Normal;
#endif
#ifdef USE_NORMAL_VAL
	vec3 Normal;
#endif

#ifdef USE_ROUGHNESS_MAP
	sampler2D Roughness;
#endif
#ifdef USE_ROUGHNESS_VAL
	float Roughness;
#endif

#ifdef USE_METALLIC_MAP
	sampler2D Metallic;
#endif
#ifdef USE_METALLIC_VAL
	float Metallic;
#endif
};
uniform Material u_Material;

in vec2 TextureCoord;
out vec4 FragColor;

void main()
{
	vec4 Color = GetAlbedo(TextureCoord);
	FragColor = Color;
}

#ifdef USE_ALBEDO_MAP
	vec4  GetAlbedo(vec2 TextureCoord) { return texture(u_Material.Albedo, TextureCoord); }
#endif
#ifdef USE_ALBEDO_VAL
	vec4  GetAlbedo(vec2 TextureCoord) { return u_Material.Albedo; }
#endif

#ifdef USE_NORMAL_MAP
	vec3  GetNormal(vec2 TextureCoord) { return texture(u_Material.Normal, TextureCoord).xyz; }
#endif
#ifdef USE_NORMAL_VAL
	vec3  GetNormal(vec2 TextureCoord) { return u_Material.Normal; }
#endif

#ifdef USE_ROUGHNESS_MAP
	float  GetRoughness(vec2 TextureCoord) { return texture(u_Material.Roughness, TextureCoord).x; }
#endif
#ifdef USE_ROUGHNESS_VAL
	float  GetRoughness(vec2 TextureCoord) { return u_Material.Roughness; }
#endif

#ifdef USE_METALLIC_MAP
	float  GetMetallic(vec2 TextureCoord) { return texture(u_Material.Metallic, TextureCoord).x; }
#endif
#ifdef USE_METALLIC_VAL
	float  GetMetallic(vec2 TextureCoord) { return u_Material.Metallic; }
#endif
