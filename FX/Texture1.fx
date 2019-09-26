Texture2D gMainTex: register(t0);
SamplerState samState
{
	Filter = MIN_MAG_MIP_LINEAR;
};
struct VertexIn
{
	float3 PosV  : POSITION;
	float3 NormalV : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosC  : SV_POSITION;
	float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.PosC = float4(vIn.PosV, 1.0f);
	vout.Tex = vIn.Tex;
	return vout;
}

float4 PS(VertexOut pIn) : SV_Target
{
	float4 texColor = gMainTex.Sample(samState,pIn.Tex);
	//float4 result = float4(texColor.x,texColor.y,texColor.z,1);
	return texColor;
}

technique11 TextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}