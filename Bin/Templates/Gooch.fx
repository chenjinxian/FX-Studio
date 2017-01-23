
#define FLIP_TEXTURE_Y

cbuffer CBufferPerObject
{
	float4x4 WorldITXf : WorldInverseTranspose < string UIWidget = "None"; >;
	float4x4 WvpXf : WorldViewProjection < string UIWidget = "None"; >;
	float4x4 WorldXf : World < string UIWidget = "None"; >;
	float4x4 ViewIXf : ViewInverse < string UIWidget = "None"; >;

	float3 Lamp0Pos : Position <
		string Object = "PointLight0";
		string UIName = "Lamp 0 Position";
		string Space = "World";
	> = { -0.5f,2.0f,1.25f };

	float3 Lamp0Color : Specular <
		string UIName = "Lamp 0 Color";
		string Object = "Pointlight0";
		string UIWidget = "Color";
	> = { 1.0f,1.0f,1.0f };
}

cbuffer CBufferPerFrame
{
	float3 AmbiColor : Ambient <
		string UIName = "Ambient Light";
		string UIWidget = "Color";
	> = { 0.07f,0.07f,0.07f };

	float3 WarmColor <
		string UIName = "Gooch Warm Tone";
		string UIWidget = "Color";
	> = { 0.5f, 0.4f, 0.05f };

	float3 CoolColor <
		string UIName = "Gooch Cool Tone";
		string UIWidget = "Color";
	> = { 0.05f, 0.05f, 0.6f };

	float GlossTop <
		string UIWidget = "slider";
		float UIMin = 0.1;
		float UIMax = 0.95;
		float UIStep = 0.01;
		string UIName = "Gloss Edge";
	> = 0.65;

	float Ks <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.05;
		string UIName = "Specular";
	> = 0.4;

	float GlossDrop <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 1.0;
		float UIStep = 0.01;
		string UIName = "Gloss Falloff";
	> = 0.25;

	float GlossEdge <
		string UIWidget = "slider";
		float UIMin = 0.0;
		float UIMax = 0.5;
		float UIStep = 0.01;
		string UIName = "Gloss Edge";
	> = 0.25;

	float SpecExpon : SpecularPower <
		string UIWidget = "slider";
		float UIMin = 1.0;
		float UIMax = 128.0;
		float UIStep = 1.0;
		string UIName = "Specular Power";
	> = 5.0;

	float3 SurfaceColor : DIFFUSE <
		string UIName = "Surface";
		string UIWidget = "Color";
	> = { 1.0f, 1.0f, 1.0f };
}

struct VS_INPUT {
	float3 Position	: POSITION;
	float2 UV		: TEXCOORD0;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
};

struct VS_OUTPUT
{
	float4 HPosition	: SV_Position;
	float3 WorldNormal	: NORMAL;
	float3 WorldTangent	: TANGENT;
	float3 WorldBinormal : BINORMAL;
	float2 UV		: TEXCOORD0;
	float3 LightVec	: TEXCOORD1;
	float3 WorldView	: TEXCOORD2;
};

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	float4 Po = float4(IN.Position.xyz, 1);
	OUT.HPosition = mul(Po, WvpXf);

	OUT.WorldNormal = normalize(mul(float4(IN.Normal, 0), WorldITXf).xyz);
	OUT.WorldTangent = normalize(mul(float4(IN.Tangent, 0), WorldITXf).xyz);
	OUT.WorldBinormal = cross(OUT.WorldNormal, OUT.WorldTangent);

	float3 Pw = mul(Po, WorldXf).xyz;
	OUT.LightVec = normalize(Lamp0Pos - Pw);
#ifdef FLIP_TEXTURE_Y
	OUT.UV = float2(IN.UV.x, (1.0 - IN.UV.y));
#else /* !FLIP_TEXTURE_Y */
	OUT.UV = IN.UV;
#endif /* !FLIP_TEXTURE_Y */
	OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);

	return OUT;
}

float glossy_drop(float v,
	uniform float top,
	uniform float bot,
	uniform float drop)
{
	return (drop+smoothstep(bot,top,v)*(1.0-drop));
}

void gooch_shading(vertexOutput IN,
			float3 LightColor,
			float3 Nn,
			float3 Ln,
			float3 Vn,
			out float3 DiffuseContrib,
			out float3 SpecularContrib)
{
	float3 Hn = normalize(Vn + Ln);
	float ldn = dot(Ln,Nn);
	float hdn = dot(Hn,Nn);
	float4 litV = lit(ldn,hdn,SpecExpon);
	float goochFactor = (1.0 + ldn) / 2.0;
	float3 toneColor = lerp(CoolColor,WarmColor,goochFactor);
	DiffuseContrib = toneColor;
	float spec = litV.y * litV.z;
	spec *= glossy_drop(spec,GlossTop,(GlossTop-GlossEdge),GlossDrop);
	SpecularContrib = spec * Ks * LightColor;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float3 diffContrib;
	float3 specContrib;
	gooch_shading(IN, Lamp0Color, IN.WorldNormal, IN.LightVec, IN.WorldView, diffContrib, specContrib);
	float3 diffuseColor = SurfaceColor;
	float3 result = specContrib + (diffuseColor * (diffContrib + AmbiColor));

	return float4(result,1);
}

RasterizerState DisableCulling
{
	CullMode = NONE;
};

technique10 main10
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, pixel_shader()));

		SetRasterizerState(DisableCulling);
	}
}
