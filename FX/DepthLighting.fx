#include "LightHelper.fx"

Texture2D gMainTex: register(t0);
Texture2D gShadowTex: register(t1);
//SamplerState gSamTex
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = WRAP;  
//    AddressV = WRAP;
//};

SamplerState gSamTex: register( s0 );  
SamplerState gClampSampleType:register(s1);


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

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;//世界矩阵的逆矩阵的转置
	float4x4 gWorldViewProj;
	Material gMaterial;
	float4x4 gCustomMVP;
};

struct VertexIn
{
	float3 PosV    : POSITION;	//顶点坐标
	float3 NormalV : NORMAL;	//顶点法线
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosC    : SV_POSITION;	//投影后的坐标
	float3 PosW    : POSITION0;		//世界变换后的坐标
	float4 PosD    : POSITION1;		//深度相机空间的pos
	float3 NormalW : NORMAL;		//世界变换后的顶点法线
	float2 Tex : TEXCOORD; 
};

struct PSOut
{
	float4 Color    : SV_Target0;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vout;

	vout.PosW = mul(float4(vIn.PosV, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vIn.NormalV, (float3x3)gWorldInvTranspose);

	vout.PosC = mul(float4(vIn.PosV, 1.0f), gWorldViewProj);
	vout.PosD = mul(float4(vIn.PosV, 1.0f), gCustomMVP);
	vout.Tex = vIn.Tex;    
	return vout;
}

PSOut PS(VertexOut pIn)
{
	PSOut psout; //不能用out，会与标准库重名(晕)
	//反射颜色
	float4 texColor = gMainTex.Sample(gSamTex,pIn.Tex);
	float4 albedoSpec = float4(texColor.xyz, gMaterial.SpecGloss);


	float2 shadowCoord;
	shadowCoord.x = (pIn.PosD.x/pIn.PosD.w)* 0.5f+ 0.5f;
	shadowCoord.y = (pIn.PosD.y/pIn.PosD.w)*(-0.5f) + 0.5f;//todo * -o.5f？ 
	float depth = pIn.PosD.z/pIn.PosD.w;
	float bias = 0.001f;
	float4 matAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//减去阴影偏斜量
	float ShadowMapDepth = gShadowTex.Sample(gSamTex, shadowCoord).r;
	ShadowMapDepth = ShadowMapDepth + bias;
	//if (saturate(shadowCoord.x) == shadowCoord.x&&saturate(shadowCoord.y) == shadowCoord.y)
	{
		if (ShadowMapDepth >= depth)
		{
			//插值运算有可能使法线不再单位化，重新单位化法线
			pIn.NormalW = normalize(pIn.NormalW);
			
			//顶点到观察点向量，归一化
			float3 toEyeW = normalize(gEyePosW - pIn.PosW);

			//初始化颜色值全部为0
			float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

			//每个光源计算后得到的环境光、漫反射光、高光
			float4 A, D, S;

			//每个光源计算后将ADS更新到最终结果中
			for (int i = 0; i < DLCount; i++)
			{
				ComputeDirectionalLight(matAmbient, albedoSpec, gDirLight[i], pIn.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			for (int i = 0; i < PLCount; i++)
			{
				ComputePointLight(matAmbient, albedoSpec, gPointLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			for (int i = 0; i < SLCount; i++)
			{
				ComputeSpotLight(matAmbient, albedoSpec, gSpotLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			 texColor = ambient + diffuse + spec;
		}
		else
		{
			texColor  = texColor * 0.55f;
		}
	}
	float4 result = float4(texColor.x, texColor.y, texColor.z, 0.5f);
	psout.Color = result;
	return psout;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		//SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}