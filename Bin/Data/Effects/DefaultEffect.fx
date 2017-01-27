/************* Resources *************/

cbuffer CBufferPerObject
{
	float4x4 WorldITXf : WorldInverseTranspose	< string UIWidget = "None"; >;
	float4x4 WvpXf     : WorldViewProjection	< string UIWidget = "None"; >;
	float4x4 WorldXf   : World					< string UIWidget = "None"; >;
	float4x4 ViewIXf   : ViewInverse			< string UIWidget = "None"; >;

	float3 LightPos : Position <
		string Object = "PointLight0";
		string Space = "World";
	> = { 10.0f, 10.0f, -10.0f };
}

cbuffer CBufferPerFrame
{
	float3 AmbiColor : Ambient <
		string UIName = "Ambient Lighting";
		string UIWidget = "Color";
	> = { 0.1f, 0.1f, 0.1f };

	float3 SurfColor : DIFFUSE <
		string UIName = "Surface Color";
		string UIWidget = "Color";
	> = { 0.8f, 0.8f, 1.0f };

	float Ks <
		string UIName = "Specular Intensity";
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.01;
	> = 0.5;

	float SpecExpon : SpecularPower <
		string UIName = "Specular Power";
		string UIWidget = "slider";
		float UIMin = 1.0;
		float UIMax = 128.0;
		float UIStep = 1.0;
	> = 30.0;
}

/************* Data Structures *************/

struct VS_INPUT
{
	float3 Position	: POSITION;
	float2 UV		: TEXCOORD0;
	float3 Normal	: NORMAL;
};

struct VS_OUTPUT
{
	float4 HPosition : SV_Position;
	float3 WorldNormal : NORMAL;
	float2 UV : TEXCOORD0;
	float3 LightVec : TEXCOORD1;
	float3 WorldView : TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 Po = float4(IN.Position.xyz, 1);
	OUT.HPosition = mul(Po, WvpXf);

	OUT.WorldNormal = normalize(mul(float4(IN.Normal, 0), WorldITXf).xyz);

	float3 Pw = mul(Po, WorldXf).xyz;
	OUT.LightVec = normalize(LightPos - Pw);
	OUT.UV = IN.UV;
	OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);

	return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float3 Hn = normalize(IN.WorldView + IN.LightVec);
	float4 lv = lit(dot(IN.LightVec, IN.WorldNormal), dot(Hn, IN.WorldNormal), SpecExpon);
	float3 DiffResult = SurfColor * (lv.yyy + AmbiColor);
	float3 SpecResult = Ks * lv.zzz;
	return float4((DiffResult + SpecResult).xyz, 1.0);
}

RasterizerState DisableCulling
{
	CullMode = NONE;
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