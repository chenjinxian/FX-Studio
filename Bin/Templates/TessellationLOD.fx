/************* Resources *************/

cbuffer CBufferPerObject
{
	float4x4 WvpXf     : WorldViewProjection	< string UIWidget = "None"; >;
	float4x4 WorldXf   : World					< string UIWidget = "None"; >;
	float4x4 ViewIXf   : ViewInverse			< string UIWidget = "None"; >;
}

cbuffer CBufferPerFrame
{
	float TessellationEdgeFactors[3] <
	> = { 2.0, 2.0, 2.0 };

	float TessellationInsideFactor <
	> = 2.0;

	int MaxTessellationFactor <
	> = 64;

	float MinTessellationDistance <
	> = 2.0f;

	float MaxTessellationDistance <
	> = 20.0f;
}

/************* Data Structures *************/

struct VS_INPUT
{
	float3 Position	: POSITION;
	float2 UV		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float3 Position : POSITION;
	float2 UV		: TEXCOORD0;
};

struct HS_CONSTANT_OUTPUT
{
	float EdgeFactors[3] : SV_TessFactor;
	float InsideFactor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 Position : POSITION;
	float2 UV		: TEXCOORD;
};

struct DS_OUTPUT
{
	float4 HPosition : SV_Position;
	float2 UV : TEXCOORD0;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = IN.Position;
	OUT.UV = IN.UV;

	return OUT;
}

/************* Hull Shaders *************/

HS_CONSTANT_OUTPUT constant_hull_shader(InputPatch<VS_OUTPUT, 3> patch, uint patchID : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

	// Caclulate the center of the patch
	float3 objectCenter = (patch[0].Position.xyz + patch[1].Position.xyz + patch[2].Position.xyz) / 3.0f;
	float3 worldCenter = mul(float4(objectCenter, 1.0f), WorldXf).xyz;

	// Calculate uniform tessellation factor based on distance from the camera
	float tessellationFactor = max(min(MaxTessellationFactor, (MaxTessellationDistance - distance(worldCenter, ViewIXf[3].xyz)) / (MaxTessellationDistance - MinTessellationDistance) * MaxTessellationFactor), 1);

	[unroll]
	for (int i = 0; i < 3; i++)
	{
		OUT.EdgeFactors[i] = tessellationFactor;
	}

	OUT.InsideFactor = tessellationFactor;

	return OUT;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("constant_hull_shader")]
HS_OUTPUT hull_shader(InputPatch<VS_OUTPUT, 3> patch, uint controlPointID : SV_OutputControlPointID)
{
	HS_OUTPUT OUT = (HS_OUTPUT)0;

	OUT.Position = patch[controlPointID].Position;
	OUT.UV = patch[controlPointID].UV;

	return OUT;
}

/************* Domain Shader *************/

[domain("tri")]
DS_OUTPUT domain_shader(HS_CONSTANT_OUTPUT IN, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	DS_OUTPUT OUT = (DS_OUTPUT)0;

	float3 objectPosition = uvw.x * patch[0].Position.xyz + uvw.y * patch[1].Position.xyz + uvw.z * patch[2].Position.xyz;
	float2 uv = uvw.x * patch[0].UV + uvw.y * patch[1].UV + uvw.z * patch[2].UV;

	OUT.HPosition = mul(float4(objectPosition, 1.0f), WvpXf);
	OUT.UV = uv;

	return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(DS_OUTPUT IN) : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

RasterizerState WireFrame
{
	FillMode = WIREFRAME;
};

/************* Techniques *************/

technique11 main11
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetHullShader(CompileShader(hs_5_0, hull_shader()));
		SetDomainShader(CompileShader(ds_5_0, domain_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

		SetRasterizerState(WireFrame);
	}
}