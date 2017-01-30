/************* Resources *************/

cbuffer CBufferPerObject
{
	float4x4 WorldITXf : WorldInverseTranspose	< string UIWidget = "None"; >;
	float4x4 WvpXf     : WorldViewProjection	< string UIWidget = "None"; >;
	float4x4 WorldXf   : World					< string UIWidget = "None"; >;
	float4x4 ViewIXf   : ViewInverse			< string UIWidget = "None"; >;

	float3 Lamp0Pos : Position <
		string Object = "PointLight0";
		string UIName = "Lamp 0 Position";
		string Space = "World";
	> = { -0.5f, 2.0f, 1.25f };

	float3 Lamp0Color : Specular <
		string Object = "Pointlight0";
		string UIName = "Lamp 0 Color";
		string UIWidget = "Color";
	> = { 1.0f, 1.0f, 1.0f };
}

cbuffer CBufferPerFrame
{
	float3 AmbiColor : Ambient <
		string UIName = "Ambient Light";
		string UIWidget = "Color";
	> = { 0.07f, 0.07f, 0.07f };

	float Ks <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.05;
		string UIName = "Specular";
	> = 0.4;

	float Eccentricity <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.0001;
		string UIName = "Highlight Eccentricity";
	> = 0.3;

	float Bump <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 3.0;
		float UIStep = 0.01;
		string UIName = "Bumpiness";
	> = 1.0;

	float Kr <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.01;
		string UIName = "Reflection Strength";
	> = 0.5;
}

Texture2D ColorTexture : DIFFUSE <
	string ResourceName = "DefaultColor.dds";
	string UIName = "Diffuse Texture";
	string ResourceType = "2D";
>;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D NormalTexture <
	string ResourceName = "DefaultBumpNormal.dds";
	string UIName = "Normal-Map Texture";
	string ResourceType = "2D";
>;

SamplerState NormalSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

TextureCube EnvTexture : ENVIRONMENT <
	string ResourceName = "DefaultReflection.dds";
	string UIName = "Environment";
	string ResourceType = "Cube";
>;

SamplerState EnvSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

/************* Data Structures *************/

struct VS_INPUT
{
	float3 Position	: POSITION;
	float2 UV		: TEXCOORD0;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float3 Binormal : BINORMAL;
};

struct VS_OUTPUT
{
	float4 HPosition		: SV_Position;
	float3 WorldNormal		: NORMAL;
	float3 WorldTangent		: TANGENT;
	float3 WorldBinormal	: BINORMAL;
	float2 UV				: TEXCOORD0;
	float3 LightVec			: TEXCOORD1;
	float3 WorldView		: TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 Po = float4(IN.Position.xyz, 1);
	OUT.HPosition = mul(Po, WvpXf);

	OUT.WorldNormal = normalize(mul(float4(IN.Normal, 0), WorldITXf).xyz);
	OUT.WorldTangent = normalize(mul(float4(IN.Tangent, 0), WorldITXf).xyz);
	OUT.WorldBinormal = normalize(mul(float4(IN.Binormal, 0), WorldITXf).xyz);

	float3 Pw = mul(Po, WorldXf).xyz;
	OUT.LightVec = normalize(Lamp0Pos - Pw);
#ifdef FLIP_TEXTURE_Y
	OUT.UV = float2(IN.UV.x, (1.0 - IN.UV.y));
#else
	OUT.UV = IN.UV;
#endif
	OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);

	return OUT;
}

void blinn_shading(VS_OUTPUT IN,
	float3 LightColor,
	float3 Nn,
	float3 Ln,
	float3 Vn,
	out float3 DiffuseContrib,
	out float3 SpecularContrib)
{
	float3 Hn = normalize(Vn + Ln);
	float hdn = dot(Hn, Nn);
	float3 R = reflect(-Ln, Nn);
	float rdv = dot(R, Vn);
	rdv = max(rdv, 0.001);
	float ldn = dot(Ln, Nn);
	ldn = max(ldn, 0.0);
	float ndv = dot(Nn, Vn);
	float hdv = dot(Hn, Vn);
	float eSq = Eccentricity * Eccentricity;
	float distrib = eSq / (rdv * rdv * (eSq - 1.0) + 1.0);
	distrib = distrib * distrib;
	float Gb = 2.0 * hdn * ndv / hdv;
	float Gc = 2.0 * hdn * ldn / hdv;
	float Ga = min(1.0, min(Gb, Gc));
	float fresnelHack = 1.0 - pow(ndv, 5.0);
	hdn = distrib * Ga * fresnelHack / ndv;
	DiffuseContrib = ldn * LightColor;
	SpecularContrib = hdn * Ks * LightColor;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float3 bump = Bump * (2.0 * NormalTexture.Sample(NormalSampler, IN.UV).xyz - 1.0);
	float3x3 tbn = float3x3(IN.WorldTangent, IN.WorldBinormal, IN.WorldNormal);
	float3 WorldNormal = mul(bump, tbn);
	WorldNormal = normalize(WorldNormal);

	float3 diffContrib;
	float3 specContrib;
	blinn_shading(IN, Lamp0Color, WorldNormal, IN.LightVec, IN.WorldView, diffContrib, specContrib);
	float3 diffuseColor = ColorTexture.Sample(ColorSampler, IN.UV).rgb;
	float3 result = specContrib + (diffuseColor * (diffContrib + AmbiColor));

	float3 reflVec = -reflect(IN.WorldView, WorldNormal);
	float3 reflColor = Kr * EnvTexture.Sample(EnvSampler, reflVec).rgb;
	result += diffuseColor * reflColor;

	return float4(result, 1);
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