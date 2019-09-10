Texture2D g_tex: register(t0);
SamplerState samTex
{
    Filter = MIN_MAG_MIP_LINEAR;
};
struct VertexIn
{
	float3 Pos  : POSITION;
		float3 NormalL : NORMAL;
		float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION; 
float2 tex : TEXCOORD;  	
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = float4(vin.Pos, 1.0f);
	
    vout.tex = vin.tex;    
    	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
float4 texColor = g_tex.Sample(samTex,pin.tex);
    return texColor;
}

technique11 TextureTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}