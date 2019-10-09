#include "LightHelper.fx"

SamplerState gSamTex
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
	float3 PosV : POSITION;
	float3 NormalV : NORMAL;
	float2 Tex		: TEXCOORD;
};

struct VertexOut
{
	float4 HPos		: HPos;
	float4 PosV : SV_POSITION;
	float2 Tex		: TEXCOORD0;
};


VertexOut LightingVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.HPos = float4(vIn.PosV, 1.0f);
	vOut.PosV = vOut.HPos;
	vOut.Tex = vIn.Tex;

	return vOut;
}

float4 LightingPS(VertexOut pIn) : SV_Target
{
	float3 position = gTexPosition.Sample(gSamTex, pIn.Tex).rgb;
	float inShadow = gTexPosition.Sample(gSamTex, pIn.Tex).a;
	float3 normal = gTexNormal.Sample(gSamTex, pIn.Tex).rgb;
	float4 albedoSpec = gTexAlbedoSpec.Sample(gSamTex, pIn.Tex);
	float4 matAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 toEyeW = normalize(gEyePosW - position);
	float4 colorLinear = float4(0.0f, 0.0f, 0.0f, 0.0f);//必须初始化

	if (inShadow == 0.0f)
	{
		float4 A, D, S;
		for (int i = 0; i < DLCount; i++)
		{
			if (length(normal) > 0.0f)
			{
				ComputeDirectionalLight(matAmbient, albedoSpec, gDirLight[i], normal, toEyeW, A, D, S);
				colorLinear += A;
				colorLinear += D;
				colorLinear += S;
			}
		}

		for (int i = 0; i < PLCount; i++)
		{
			if (length(normal) > 0.0f)
			{
				ComputePointLight(matAmbient, albedoSpec, gPointLight[i], position, normal, toEyeW, A, D, S);
				colorLinear += A;
				colorLinear += D;
				colorLinear += S;
			}
		}

		for (int i = 0; i < SLCount; i++)
		{
			if (length(normal) > 0.0f)
			{
				ComputeSpotLight(matAmbient, albedoSpec, gSpotLight[i], position, normal, toEyeW, A, D, S);
				colorLinear += A;
				colorLinear += D;
				colorLinear += S;
			}
		}
	}
	else
	{
		colorLinear = colorLinear * 0.55f;
	}
	//colorLinear += albedoSpec;
	//return float4(position,1.0f);
	// albedoSpec;
	return colorLinear;
	//return float4(normal,1.0f);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, LightingVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, LightingPS()));
	}
};