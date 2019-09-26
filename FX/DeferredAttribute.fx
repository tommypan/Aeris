Texture2D g_tex: register(t0);
Texture2D shadow_tex: register(t1);

SamplerState samTex: register(s0);

struct Material
{
	float specGloss;//w表示高光强度 
	float4 reflect;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;//先保留
	Material gMaterial;
	float4x4 customMVP;
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
	float4 dPos		: POSITION1;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};



struct PixelOutDeferred
{
	float4 position : SV_Target0;//世界坐标系，用于光照计算方向
	float4 normal   : SV_Target1;//世界坐标系
	float4 albedoSpec  : SV_Target2;
};

VertexOut BasicVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProj);//忘记了float3转换为float4，w分量则是0，那么计算结果完全错误，被裁剪完了
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.dPos = mul(float4(vIn.position, 1.0f), customMVP);
	vOut.normal   = mul(vIn.normal, (float3x3)gWorldInvTranspose);//4,4行列决定原点在另外坐标系显示，对于向量可直接舍弃；normal在ps阶段转换没意义，只会浪费资源
	vOut.tex      = vIn.tex;

	return vOut;
}

PixelOutDeferred BasicPS(VertexOut pIn)
{
	PixelOutDeferred pOut;

	//world position
	pOut.position = mul(pIn.hPos, gWorld);

	//shadow
	float2 shadowCoord;
	shadowCoord.x = (pIn.dPos.x / pIn.dPos.w)* 0.5f + 0.5f;
	shadowCoord.y = (pIn.dPos.y / pIn.dPos.w)*(-0.5f) + 0.5f;
	float depth = pIn.dPos.z / pIn.dPos.w;
	float bias = 0.001f;
	float ShadowMapDepth = shadow_tex.Sample(samTex, shadowCoord).r;
	ShadowMapDepth = ShadowMapDepth + bias;
	pOut.position.w = 1.0f;//在阴影中
	if (ShadowMapDepth >= depth)
	{
		pOut.position.w = 0.0f;//不在
	}

	pOut.normal = normalize(float4(pIn.normal, 0.0f));
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