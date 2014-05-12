//=============================================================================
// lighting.fx by Frank Luna (C) 2008 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "lighthelper.fx"
 
int numberOfLights;
int numberOfSpotLights;

cbuffer cbPerFrame
{
	Light pointLights[30];
	Light ambientLight;
	Light spotLights[30];
	Light spotLight;
	Light teleportLight;
	Light splashLight;
	int gLightType; 
	float3 gEyePosW;
	int useTex;
	int useSplashLight;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWVP;
	float4x4 gTexMtx;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gSpecMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float4 diffuse : DIFFUSE;
	float4 spec    : SPECULAR;
	float2 texC	   : TEXCOORD;
};

struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float4 diffuse : DIFFUSE;
    float4 spec    : SPECULAR;
	float2 texC	   : TEXCOORD;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	// Transform to world space space.
	vOut.posW    = mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.diffuse = vIn.diffuse;
	vOut.spec    = vIn.spec;
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);

	return vOut;
}
 
float4 PS(VS_OUT pIn) : SV_Target
{
	// Interpolating normal can make it not be of unit length so normalize it.
    pIn.normalW = normalize(pIn.normalW);

	
	float3 normalW = pIn.normalW;
	// Get materials from texture maps.
	float4 diffuse;
	float4 spec;
	if (useTex == 1)
	{
		diffuse = gDiffuseMap.Sample( gTriLinearSam, pIn.texC );
		spec    = gSpecMap.Sample( gTriLinearSam, pIn.texC );
		//normalW = gSpecMap.Sample( gTriLinearSam, pIn.texC );
	}
	else
	{
		diffuse = pIn.diffuse;
		spec = pIn.spec;
	}
   
   
    SurfaceInfo v = {pIn.posW, normalW, diffuse, spec};
    
    float3 litColor;
    
	for (int i=0; i<numberOfLights; ++i)
	{
		litColor += PointLight(v, pointLights[i], gEyePosW);
		if (litColor.x > 1.0f)
			litColor.x = 1.0f;
		if (litColor.y > 1.0f)
			litColor.y = 1.0f;
		if (litColor.z > 1.0f)
			litColor.z = 1.0f;
	}

	litColor += ambientLight.ambient;

	for (int i = 0; i < numberOfSpotLights; i++) {
		litColor += Spotlight(v, spotLights[i], gEyePosW);
	}
	litColor += Spotlight(v, spotLight, gEyePosW);
	litColor += Spotlight(v, teleportLight, gEyePosW);

	if (useSplashLight == 1)
		litColor = ParallelLight(v, splashLight, gEyePosW);
	   
    return float4(litColor, pIn.diffuse.a);
}

technique10 LightTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}



