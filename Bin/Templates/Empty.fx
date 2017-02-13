/************* Resources *************/

cbuffer CBufferPerObject
{
	float4x4 WvpXf : WorldViewProjection < string UIWidget = "None"; >;
}

/************* Vertex Shader *************/

float4 vertex_shader(float3 position : POSITION) : SV_Position
{
	return mul(float4(position.xyz, 1.0), WvpXf);
}

/************* Pixel Shader *************/

float4 pixel_shader() : SV_Target
{
	return float4(1.0, 1.0, 1.0, 1.0);
}

RasterizerState DisableCulling
{
	FillMode = WIREFRAME;
};

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
