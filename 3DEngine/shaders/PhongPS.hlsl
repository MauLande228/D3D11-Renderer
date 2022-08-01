cbuffer Light
{
    float3 lightPos;
    float3 Ambient;
    float3 DiffuseColor;
    float DiffuseIntensity;
    float AttConst;
    float AttLin;
    float AttQuad;
};

cbuffer Object
{
    float SpecularIntensity;
    float SpecularPower;
    float padding[2];
};

struct VsOut
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 tc : TEXCOORD;
    float4 pos : SV_Position;
};

Texture2D tex;
SamplerState splr;

float4 main(VsOut input) : SV_TARGET
{
	//Fragment to Light vector data
    const float3 vToL = lightPos - input.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    //Diffuse attenuation
    const float att = 1.0f / (AttConst + AttLin * distToL + AttQuad * (distToL * distToL));
    
    //Diffise intensity
    const float3 diffuse = DiffuseColor * DiffuseIntensity * att * max(0.0f, dot(dirToL, input.normal));
    
    //Reflected light vector
    const float3 w = input.normal * dot(vToL, input.normal);
    const float3 r = w * 2.0f - vToL;
    
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (DiffuseColor * DiffuseIntensity) * SpecularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(input.worldPos))), SpecularPower);
    
    //Final color
    return float4(saturate((diffuse + Ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);
}