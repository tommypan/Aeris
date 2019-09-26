Texture2D g_tex: register(t0);
Texture2D shadow_tex: register(t1);

SamplerState samTex: register(s0);

struct Material
{
	float specGloss;//w��ʾ�߹�ǿ�� 
	float4 reflect;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;//�ȱ���
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
	float4 position : SV_Target0;//��������ϵ�����ڹ��ռ��㷽��
	float4 normal   : SV_Target1;//��������ϵ
	float4 albedoSpec  : SV_Target2;
};

VertexOut BasicVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.position = mul(float4(vIn.position, 1.0f), gWorldViewProj);//������float3ת��Ϊfloat4��w��������0����ô��������ȫ���󣬱��ü�����
	vOut.hPos = float4(vIn.position, 1.0f);
	vOut.dPos = mul(float4(vIn.position, 1.0f), customMVP);
	vOut.normal   = mul(vIn.normal, (float3x3)gWorldInvTranspose);//4,4���о���ԭ������������ϵ��ʾ������������ֱ��������normal��ps�׶�ת��û���壬ֻ���˷���Դ
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
	pOut.position.w = 1.0f;//����Ӱ��
	if (ShadowMapDepth >= depth)
	{
		pOut.position.w = 0.0f;//����
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