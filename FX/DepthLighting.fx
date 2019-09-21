#include "LightHelper.fx"

Texture2D g_tex: register(t0);
Texture2D shadow_tex: register(t1);
//SamplerState samTex
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = WRAP;  
//    AddressV = WRAP;
//};

SamplerState samTex: register( s0 );  
SamplerState ClampSampleType:register(s1);


const int DLCount = 1;
const int PLCount = 4;
const int SLCount = 1;
cbuffer cbPerFrame
{
	DirectionalLight gDirLight[1];
	PointLight gPointLight[4];
	SpotLight gSpotLight[1];
	float3 gEyePosW;			//观察点
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;//世界矩阵的逆矩阵的转置
	float4x4 gWorldViewProj;
	Material gMaterial;
	
	float4x4 customMVP;
};

struct VertexIn
{
	float3 PosL    : POSITION;	//顶点坐标
	float3 NormalL : NORMAL;	//顶点法线
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;	//投影后的坐标
	float3 PosW    : POSITION0;		//世界变换后的坐标
	float4 PosD    : POSITION1;		//深度相机空间的pos
	float3 NormalW : NORMAL;		//世界变换后的顶点法线
	float2 tex : TEXCOORD; 
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.PosD = mul(float4(vin.PosL, 1.0f), customMVP);
	vout.tex = vin.tex;    
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float2 shadowCoord;
	//shadowCoord.x = (pin.PosD.x/pin.PosD.w)/2 + 0.5f;
	//shadowCoord.y = (pin.PosD.x/pin.PosD.w)/2 + 0.5f;//todo * -o.5f？ 
	shadowCoord.x = (pin.PosD.x/pin.PosD.w)* 0.5f+ 0.5f;
	shadowCoord.y = (pin.PosD.y/pin.PosD.w)*(-0.5f) + 0.5f;//todo * -o.5f？ 
	float depth = pin.PosD.z/pin.PosD.w;

	float bias;
	//设置偏斜量
	bias = 0.001f;

	float4 texColor = g_tex.Sample(samTex,pin.tex);
	//减去阴影偏斜量
	float ShadowMapDepth = shadow_tex.Sample(samTex, shadowCoord).r;
	ShadowMapDepth = ShadowMapDepth + bias;
	//if (saturate(shadowCoord.x) == shadowCoord.x&&saturate(shadowCoord.y) == shadowCoord.y)
	{
		if (ShadowMapDepth >= depth)
		{
			//插值运算有可能使法线不再单位化，重新单位化法线
			pin.NormalW = normalize(pin.NormalW);
			
			//顶点到观察点向量，归一化
			float3 toEyeW = normalize(gEyePosW - pin.PosW);

			//初始化颜色值全部为0
			float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

			//每个光源计算后得到的环境光、漫反射光、高光
			float4 A, D, S;

			//每个光源计算后将ADS更新到最终结果中
			for (int i = 0; i < DLCount; i++)
			{
				ComputeDirectionalLight(gMaterial.ambient, gMaterial.specular, gDirLight[i], pin.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			for (int i = 0; i < PLCount; i++)
			{
				ComputePointLight(gMaterial.ambient, gMaterial.specular, gPointLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			for (int i = 0; i < SLCount; i++)
			{
				ComputeSpotLight(gMaterial.ambient, gMaterial.specular, gSpotLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
				ambient += A;
				diffuse += D;
				spec += S;
			}

			float4 litColor = ambient + diffuse + spec;

			//最终颜色透明度使用漫反射光的
			texColor = texColor + litColor;
		}
	}
	float4 result = float4(texColor.x, texColor.y, texColor.z, 0.5f);
	return result;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}