#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;			//观察点
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;//世界矩阵的逆矩阵的转置
	float4x4 gWorldViewProj;
	Material gMaterial;
};

struct VertexIn
{
	float3 PosL    : POSITION;	//顶点坐标
	float3 NormalL : NORMAL;	//顶点法线
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;	//投影后的坐标
	float3 PosW    : POSITION;		//世界变换后的坐标
	float3 NormalW : NORMAL;		//世界变换后的顶点法线
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
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
	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;

	//最终颜色透明度使用漫反射光的
	litColor.a = gMaterial.diffuse.a;

	return litColor;
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