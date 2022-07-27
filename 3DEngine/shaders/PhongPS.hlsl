cbuffer Light
{
    float3 lightPos;
};

static const float3 MaterialColor = { 0.7f, 0.7f, 0.9f };
static const float3 Ambient = { 0.05f, 0.05f, 0.05f };
static const float3 DiffuseColor = { 1.0f, 1.0f, 1.0f };
static const float DiffuseIntensity = 1.0f;
static const float AttConst = 1.0f;
static const float AttLin = 0.045f;
static const float AttQuad = 0.0075f;

struct VsOut
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

float4 main(VsOut vsin) : SV_TARGET
{
	// Fragment to light vector data
    const float3 vToL = lightPos - vsin.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    // Diffuse attenuation
    const float att = 1.0f / (AttConst + AttLin * distToL * AttQuad * (distToL * distToL));
    
    // Diffuse intensity
    const float3 diffuse = DiffuseColor * DiffuseIntensity * att * max(0.0f, dot(dirToL, vsin.normal));
    
    // Final color
    return float4(saturate(diffuse + Ambient), 1.0f);
}