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
	float3 gEyePosW;			//�۲��
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;//��������������ת��
	float4x4 gWorldViewProj;
	Material gMaterial;
	float4x4 gCustomMVP;
};

struct VertexIn
{
	float3 PosV    : POSITION;	//��������
	float3 NormalV : NORMAL;	//���㷨��
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosC    : SV_POSITION;	//ͶӰ�������
	float3 PosW    : POSITION0;		//����任�������
	float4 PosD    : POSITION1;		//�������ռ��pos
	float3 NormalW : NORMAL;		//����任��Ķ��㷨��
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
	PSOut psout; //������out�������׼������(��)
	//������ɫ
	float4 texColor = gMainTex.Sample(gSamTex,pIn.Tex);
	float4 albedoSpec = float4(texColor.xyz, gMaterial.SpecGloss);


	float2 shadowCoord;
	shadowCoord.x = (pIn.PosD.x/pIn.PosD.w)* 0.5f+ 0.5f;
	shadowCoord.y = (pIn.PosD.y/pIn.PosD.w)*(-0.5f) + 0.5f;//todo * -o.5f�� 
	float depth = pIn.PosD.z/pIn.PosD.w;
	float bias = 0.001f;
	float4 matAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//��ȥ��Ӱƫб��
	float ShadowMapDepth = gShadowTex.Sample(gSamTex, shadowCoord).r;
	ShadowMapDepth = ShadowMapDepth + bias;
	//if (saturate(shadowCoord.x) == shadowCoord.x&&saturate(shadowCoord.y) == shadowCoord.y)
	{
		if (ShadowMapDepth >= depth)
		{
			//��ֵ�����п���ʹ���߲��ٵ�λ�������µ�λ������
			pIn.NormalW = normalize(pIn.NormalW);
			
			//���㵽�۲����������һ��
			float3 toEyeW = normalize(gEyePosW - pIn.PosW);

			//��ʼ����ɫֵȫ��Ϊ0
			float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
			float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

			//ÿ����Դ�����õ��Ļ����⡢������⡢�߹�
			float4 A, D, S;

			//ÿ����Դ�����ADS���µ����ս����
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