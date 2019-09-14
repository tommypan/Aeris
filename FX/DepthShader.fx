

//VertexShader
cbuffer CBMatrix:register(b0)
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 Pos:POSITION;
	float4 color:COLOR;
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
	outa.color = ina.color;
	return outa;
}

float4 PS(VertexOut outa) : SV_Target
{
	return outa.color;
}