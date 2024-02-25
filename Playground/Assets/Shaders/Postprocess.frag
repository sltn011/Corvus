#version 450

#define GAMMA (2.2f)
#define INV_GAMMA (1.f / GAMMA)

// Settings for FXAA.
#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define QUALITY(q) ((q) < 5 ? 1.0 : ((q) > 5 ? ((q) < 10 ? 2.0 : ((q) < 11 ? 4.0 : 8.0)) : 1.5))
#define ITERATIONS 12
#define SUBPIXEL_QUALITY 0.75

layout(location = 0) in vec2 InUVCoord;

layout(location = 0) out vec4 OutColor;

layout(set = 0, binding = 1) uniform RenderTargetUBO 
{
    vec4 RTSizeFull;
} UBO;

layout(set = 1, binding = 0) uniform sampler2D CombinedTexture;

float RGB2Luma(vec3 rgb)
{
    return sqrt(dot(rgb, vec3(0.299f, 0.587f, 0.114f)));
}

vec3 ApplyInvGamma(vec3 rgb)
{
	return pow(rgb, vec3(INV_GAMMA));
}

vec3 FXAA()
{
    vec3 ColorCenter		= ApplyInvGamma(texture(CombinedTexture, InUVCoord).rgb);
    
    float LumaCenter		= RGB2Luma(ColorCenter);
	float LumaDown 			= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2( 0,-1)).rgb));
	float LumaUp 			= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2( 0, 1)).rgb));
	float LumaLeft 			= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2(-1, 0)).rgb));
	float LumaRight			= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2( 1, 0)).rgb));
    
	float LumaMin			= min(LumaCenter, min(min(LumaDown, LumaUp), min(LumaLeft, LumaRight)));
	float LumaMax			= max(LumaCenter, max(max(LumaDown, LumaUp), max(LumaLeft, LumaRight)));
    
	float LumaRange			= LumaMax - LumaMin;

    if(LumaRange < max(EDGE_THRESHOLD_MIN, LumaMax * EDGE_THRESHOLD_MAX))
	{
		return ColorCenter;
	}
    
	float LumaDownLeft 		= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2(-1,-1)).rgb));
	float LumaUpRight 		= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2( 1, 1)).rgb));
	float LumaUpLeft 		= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2(-1, 1)).rgb));
	float LumaDownRight		= RGB2Luma(ApplyInvGamma(textureLodOffset(CombinedTexture, InUVCoord, 0.0, ivec2( 1,-1)).rgb));

	float LumaDownUp		= LumaDown + LumaUp;
	float LumaLeftRight		= LumaLeft + LumaRight;
	float LumaLeftCorners	= LumaDownLeft + LumaUpLeft;
	float LumaDownCorners	= LumaDownLeft + LumaDownRight;
	float LumaRightCorners	= LumaDownRight + LumaUpRight;
	float LumaUpCorners		= LumaUpRight + LumaUpLeft;

	
	float EdgeHorizontal	= abs(-2.0 * LumaLeft + LumaLeftCorners) 
							+ abs(-2.0 * LumaCenter + LumaDownUp) * 2.0 
							+ abs(-2.0 * LumaRight + LumaRightCorners);

	float EdgeVertical		= abs(-2.0 * LumaUp + LumaUpCorners) 
							+ abs(-2.0 * LumaCenter + LumaLeftRight) * 2.0
							+ abs(-2.0 * LumaDown + LumaDownCorners);

	bool IsHorizontal		= (EdgeHorizontal >= EdgeVertical);
	float StepLength		= IsHorizontal ? UBO.RTSizeFull.w : UBO.RTSizeFull.z;
	
	float Luma1				= IsHorizontal ? LumaDown : LumaLeft;
	float Luma2				= IsHorizontal ? LumaUp : LumaRight;
	float Gradient1			= Luma1 - LumaCenter;
	float Gradient2			= Luma2 - LumaCenter;
	float GradientScaled	= 0.25 * max(abs(Gradient1), abs(Gradient2));
	
	float LumaLocalAverage	= 0.0;
	bool Is1Steeper			= abs(Gradient1) >= abs(Gradient2);
	if(Is1Steeper)
	{
		StepLength			= -StepLength;
		LumaLocalAverage	= 0.5 * (Luma1 + LumaCenter);
	} 
	else 
	{
		LumaLocalAverage	= 0.5 * (Luma2 + LumaCenter);
	}

	vec2 CurrentUV			= InUVCoord;
	if(!IsHorizontal)
	{
		CurrentUV.x			+= StepLength * 0.5;
	} 
	else 
	{
		CurrentUV.y			+= StepLength * 0.5;
	}

	vec2 Offset				= IsHorizontal ? vec2(UBO.RTSizeFull.z, 0.0) : vec2(0.0, UBO.RTSizeFull.w);
	vec2 UV1				= CurrentUV - Offset * QUALITY(0);
	vec2 UV2				= CurrentUV + Offset * QUALITY(0);

	float LumaEnd1			= RGB2Luma(ApplyInvGamma(textureLod(CombinedTexture, UV1, 0.0).rgb));
	float LumaEnd2			= RGB2Luma(ApplyInvGamma(textureLod(CombinedTexture, UV2, 0.0).rgb));
	LumaEnd1				-= LumaLocalAverage;
	LumaEnd2				-= LumaLocalAverage;
	
	bool Reached1			= abs(LumaEnd1) >= GradientScaled;
	bool Reached2			= abs(LumaEnd2) >= GradientScaled;
	bool ReachedBoth		= Reached1 && Reached2;

	if(!Reached1)
	{
		UV1					-= Offset * QUALITY(1);
	}
	if(!Reached2)
	{
		UV2					+= Offset * QUALITY(1);
	}

	if(!ReachedBoth)
	{
		for(int i = 2; i < ITERATIONS; i++)
		{
			if(!Reached1)
			{
				LumaEnd1	= RGB2Luma(ApplyInvGamma(textureLod(CombinedTexture, UV1, 0.0).rgb));
				LumaEnd1	= LumaEnd1 - LumaLocalAverage;
			}
			if(!Reached2)
			{
				LumaEnd2	= RGB2Luma(ApplyInvGamma(textureLod(CombinedTexture, UV2, 0.0).rgb));
				LumaEnd2	= LumaEnd2 - LumaLocalAverage;
			}

			Reached1		= abs(LumaEnd1) >= GradientScaled;
			Reached2		= abs(LumaEnd2) >= GradientScaled;
			ReachedBoth		= Reached1 && Reached2;
			
			if(!Reached1)
			{
				UV1			-= Offset * QUALITY(i);
			}
			if(!Reached2)
			{
				UV2			+= Offset * QUALITY(i);
			}
			
			if(ReachedBoth)
			{ 
				break;
			}
		}
	}

	float Distance1			= IsHorizontal ? (InUVCoord.x - UV1.x) : (InUVCoord.y - UV1.y);
	float Distance2			= IsHorizontal ? (UV2.x - InUVCoord.x) : (UV2.y - InUVCoord.y);
	bool IsDirection1		= Distance1 < Distance2;
	float DistanceFinal		= min(Distance1, Distance2);
	float EdgeThickness		= (Distance1 + Distance2);
	
	bool IsLumaCenterLess	= LumaCenter < LumaLocalAverage;
	bool CorrectVariation1	= (LumaEnd1 < 0.0) != IsLumaCenterLess;
	bool CorrectVariation2	= (LumaEnd2 < 0.0) != IsLumaCenterLess;
	bool CorrectVariation	= IsDirection1 ? CorrectVariation1 : CorrectVariation2;

	float PixelOffset		= -DistanceFinal / EdgeThickness + 0.5;
	float FinalOffset		= CorrectVariation ? PixelOffset : 0.0;

	float LumaAverage		= (1.0/12.0) * (2.0 * (LumaDownUp + LumaLeftRight) + LumaLeftCorners + LumaRightCorners);

	float SubPixelOffset1	= clamp(abs(LumaAverage - LumaCenter) / LumaRange, 0.0, 1.0);
	float SubPixelOffset2	= (-2.0 * SubPixelOffset1 + 3.0) * SubPixelOffset1 * SubPixelOffset1;
	float SubPixelOffset	= SubPixelOffset2 * SubPixelOffset2 * SUBPIXEL_QUALITY;

	FinalOffset				= max(FinalOffset, SubPixelOffset);

	vec2 FinalUV			= InUVCoord;
	if(!IsHorizontal)
	{
		FinalUV.x			+= FinalOffset * StepLength;
	} 
	else 
	{
		FinalUV.y			+= FinalOffset * StepLength;
	}
	
    return ApplyInvGamma(textureLod(CombinedTexture, FinalUV, 0.0).rgb);
}

void main()
{
    vec3 FragColor = texture(CombinedTexture, InUVCoord).rgb;

    FragColor      = pow(FragColor, vec3(INV_GAMMA));

    OutColor       = vec4(FXAA(), 1.0);
}
