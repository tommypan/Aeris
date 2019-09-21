Texture2D g_tex: register(t0);
SamplerState samTex: register(s0);


cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;//世界矩阵的逆矩阵的转置
	float4x4 gWorldViewProj;
	Material gMaterial;
};

struct VertexIn
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 hPos		: HPOS;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};

struct Material
{
	float4 ambient;
	float4 specular;//w表示高光强度
	float4 reflect;
};

struct PixelOutDeferred
{
	float4 position : SV_Target0;//世界坐标，用于光照计算方向
	float4 normal   : SV_Target1;//本地坐标
	float4 albedoSpec  : SV_Target2;
};

VertexOut BasicVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.position = mul(vOut.hPos, gWorldViewProj);
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.normal   = mul(normalize(vIn.normal), (float3x3)gWorld);
	vOut.tex      = vIn.tex;

	return vOut;
}

PixelOutDeferred BasicPS(VertexOut pIn)
{
	PixelOutDeferred pOut;
	pOut.position = pIn.hPos;
	pOut.normal = float4(pIn.normal, 1.0f);
	float4 texColor = g_tex.Sample(samTex, pIn.tex)
	pOut.albedoSpec = float4(texColor.xyz, specular.w);
	return pOut;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, BasicVS()));
		SetPixelShader(CompileShader(ps_5_0, BasicPS()));
	}
};