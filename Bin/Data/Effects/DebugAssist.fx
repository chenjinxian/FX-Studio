/************* Resources *************/

cbuffer CBufferPerFrame
{
	float3 AmbientColor : AMBIENT;
}

cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
}

/************* Data Structures *************/

struct VS_INPUT
{
	float3 ObjectPosition: POSITION;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
};

RasterizerState DisableCulling
{
	CullMode = NONE;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(float4(IN.ObjectPosition, 1.0f), WorldViewProjection);

	return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	return float4(AmbientColor, 1.0f);
}

/************* Techniques *************/

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

		SetRasterizerState(DisableCulling);
	}
}