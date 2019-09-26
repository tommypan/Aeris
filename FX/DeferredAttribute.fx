Texture2D gMainTex: register(t0);
Texture2D gShadowTex: register(t1);

SamplerState gSamTex: register(s0);

struct Material
{
	float SpecGloss;//w��ʾ�߹�ǿ�� 
	float4 Reflect;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4x4 gWorldInvTranspose;//�ȱ���
	Material gMaterial;
	float4x4 gCustomMVP;
};

struct VertexIn
{
	float3 PosV : POSITION;
	float3 NormalV	: Normal;
	float2 Tex		: TEXCOORD;
};

struct VertexOut
{
	float4 PosC : SV_POSITION;
	float4 PosH		: POSITION0;
	float4 PosD		: POSITION1;
	float3 Normal	: Normal;
	float2 Tex		: TEXCOORD0;
};



struct PixelOutDeferred
{
	float4 Position : SV_Target0;//��������ϵ�����ڹ��ռ��㷽��
	float4 Normal   : SV_Target1;//��������ϵ
	float4 AlbedoSpec  : SV_Target2;
};

VertexOut BasicVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.PosC = mul(float4(vIn.PosV, 1.0f), gWorldViewProj);//������float3ת��Ϊfloat4��w��������0����ô��������ȫ���󣬱��ü�����
	vOut.PosH = float4(vIn.PosV, 1.0f);
	vOut.PosD = mul(float4(vIn.PosV, 1.0f), gCustomMVP);
	vOut.Normal   = mul(vIn.NormalV, (float3x3)gWorldInvTranspose);//4,4���о���ԭ������������ϵ��ʾ������������ֱ��������Normal��ps�׶�ת��û���壬ֻ���˷���Դ
	vOut.Tex      = vIn.Tex;

	return vOut;
}

PixelOutDeferred BasicPS(VertexOut pIn)
{
	PixelOutDeferred pOut;

	//world position
	pOut.Position = mul(pIn.PosH, gWorld);

	//shadow
	float2 shadowCoord;
	shadowCoord.x = (pIn.PosD.x / pIn.PosD.w)* 0.5f + 0.5f;
	shadowCoord.y = (pIn.PosD.y / pIn.PosD.w)*(-0.5f) + 0.5f;
	float depth = pIn.PosD.z / pIn.PosD.w;
	float bias = 0.001f;
	float ShadowMapDepth = gShadowTex.Sample(gSamTex, shadowCoord).r;
	ShadowMapDepth = ShadowMapDepth + bias;
	pOut.Position.w = 1.0f;//����Ӱ��
	if (ShadowMapDepth >= depth)
	{
		pOut.Position.w = 0.0f;//����
	}

	pOut.Normal = normalize(float4(pIn.Normal, 0.0f));
	float4 texColor = gMainTex.Sample(gSamTex, pIn.Tex);
	pOut.AlbedoSpec = float4(texColor.xyz, gMaterial.SpecGloss);
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