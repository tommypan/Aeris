

//VertexShader
cbuffer CBMatrix:register(b0)
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosC:POSITION;
	float3 NormalV : NORMAL;
	float2 Tex : TEXCOORD;
};


struct VertexOut
{
	float4 PosC:SV_POSITION;
	float4 Color:Color;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut outa;
	outa.PosC = mul(float4(vIn.PosC, 1.0f), gWorldViewProj);
	return outa;
}

float4 PS(VertexOut pIn) : SV_Target
{
	return float4(1,1,1,1);
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