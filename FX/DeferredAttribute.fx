Texture2D g_tex: register(t0);
SamplerState samTex: register(s0);

struct Material
{
	float4 ambient;
	float specGloss;//w表示高光强度 
	float4 reflect;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;//先保留
	Material gMaterial;
	
	float4x4 customMVP;//todo
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
	float4 hPos		: POSITION0;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
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
	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProj);//忘记了float3转换为float4，w分离则是0，那么计算结果完全错误，渲染不出来
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.normal   = mul(normalize(vIn.normal), (float3x3)gWorld);
	vOut.tex      = vIn.tex;

	return vOut;
}

PixelOutDeferred BasicPS(VertexOut pIn)
{
	PixelOutDeferred pOut;
	pOut.position = mul(pIn.hPos, gWorld);
	pOut.normal = float4(pIn.normal, 1.0f);
	float4 texColor = g_tex.Sample(samTex, pIn.tex);
	pOut.albedoSpec = float4(texColor.xyz, gMaterial.specGloss);
	return pOut;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_5_0, BasicPS()));
	}
};