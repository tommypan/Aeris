

//VertexShader
cbuffer CBMatrix:register(b0)
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 Pos:POSITION;

	float3 NormalL : NORMAL;	//todo 去掉
	float2 tex : TEXCOORD;
};


struct VertexOut
{
	float4 Pos:SV_POSITION;
	float4 color:COlOR;
};


VertexOut VS(VertexIn ina)
{
	VertexOut outa;
	outa.Pos = mul(float4(ina.Pos, 1.0f), gWorldViewProj);
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
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