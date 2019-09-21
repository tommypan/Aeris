#include "LightHelper.fx"

SamplerState samTex: register(s0);
Texture2D gTexPosition: register(t0);
Texture2D gTexNormal: register(t1);
Texture2D gTexAlbedoSpec: register(t2);

const int DLCount = 1;
const int PLCount = 4;
const int SLCount = 1;

cbuffer cbPerFrame
{
	DirectionalLight gDirLight[DLCount];
	PointLight gPointLight[PLCount];
	SpotLight gSpotLight[SLCount];
	float3 gEyePosW;			//¹Û²ìµã
};

struct VertexIn
{
	float3 position : POSITION;
	float2 tex		: TEXCOORD;
};

struct VertexOut
{
	float4 hPos		: HPOS;
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};

struct PixelOutBasic
{
	float4 color : SV_Target;
};

VertexOut LightingVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.position = vOut.hPos;
	vOut.tex = vIn.tex;

	return vOut;
}

PixelOutBasic LightingPS(VertexOut pIn)
{
	PixelOutBasic pOut;
	float3 position = gTexPosition.Sample(samTex, pIn.tex).rgb;
	float3 normal = gTexNormal.Sample(samTex, pIn.tex).rgb;
	float3 diffuse = gTexAlbedoSpec.Sample(samTex, pIn.tex).rgb;
	float specular = gTexAlbedoSpec.Sample(samTex, pIn.tex).a;
	float4 ambient = float3(0.0f, 0.0f, 0.0f);

	float4 colorLinear;
	float4 A, D, S;
	for (int i = 0; i < DLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputeDirectionalLight(ambient, diffuse, gDirLight.direction, normal, gEyePosW - position, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
			//float3 colorLinear = lambertian * diffuse + specular * float3(1.0f, 1.0f, 1.0f);//need do
			//float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
		}
	}

	for (int i = 0; i < PLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputePointLight(ambient, diffuse, gDirLight.gPointLight[i], position, normal, gEyePosW - position, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
		}
	}

	for (int i = 0; i < SLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputeSpotLight(ambient, diffuse, gDirLight.gSpotLight[i], position, normal, gEyePosW - position, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
		}
	}


	pOut.color = float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
	//pOut.color = float4(diffuse, 1.0f);
	return pOut;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, LightingVS()));
		SetPixelShader(CompileShader(ps_5_0, LightingPS()));
	}
};