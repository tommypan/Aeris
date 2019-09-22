#include "LightHelper.fx"

SamplerState samTex
{
    Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D gTexPosition: register(t0);
Texture2D gTexNormal: register(t1);
Texture2D gTexAlbedoSpec: register(t2);

static const int DLCount = 1;
static const int PLCount = 4;
static const int SLCount = 1;

cbuffer cbPerFrame
{
	DirectionalLight gDirLight[DLCount];
	PointLight gPointLight[PLCount];
	SpotLight gSpotLight[SLCount];
	float3 gEyePosW;			//观察点
};

struct VertexIn
{
	float3 position : POSITION;
	float3 NormalL : NORMAL;
	float2 tex		: TEXCOORD;
};

struct VertexOut
{
	float4 hPos		: HPOS;
	float4 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
};


VertexOut LightingVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.position = vOut.hPos;
	vOut.tex = vIn.tex;

	return vOut;
}

float4 LightingPS(VertexOut pIn): SV_Target
{
	float3 position = gTexPosition.Sample(samTex, pIn.tex).rgb;
	float3 normal = gTexNormal.Sample(samTex, pIn.tex).rgb;
	float4 albedoSpec = gTexAlbedoSpec.Sample(samTex, pIn.tex);
	//float specular = gTexAlbedoSpec.Sample(samTex, pIn.tex).a;
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 toEyeW = normalize(gEyePosW - position);

	float4 colorLinear = float4(0.0f, 0.0f, 0.0f, 0.0f);//必须初始化
	float4 A, D, S;
	for (int i = 0; i < DLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputeDirectionalLight(ambient, albedoSpec, gDirLight[i], normal, toEyeW, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
			//float3 colorLinear = lambertian * albedoSpec + specular * float3(1.0f, 1.0f, 1.0f);//need do
			//float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
		}
	}

	for (int i = 0; i < PLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputePointLight(ambient, albedoSpec, gPointLight[i], position, normal, toEyeW, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
		}
	}

	for (int i = 0; i < SLCount; i++)
	{
		if (length(normal) > 0.0f)
		{
			ComputeSpotLight(ambient, albedoSpec, gSpotLight[i], position, normal, toEyeW, A, D, S);
			colorLinear += A;
			colorLinear += D;
			colorLinear += S;
		}
	}


	float4 color = float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
	//float4 color = float4(0.0f,0.0f,0.0f, 0.0f);
	//return float4(position,1.0f);
	return albedoSpec;
	//return float4(normal,1.0f);
	//return color;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, LightingVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_5_0, LightingPS()));
	}
};