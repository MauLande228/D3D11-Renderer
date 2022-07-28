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
    float3 MaterialColor;
    float SpecularIntensity;
    float SpecularPower;
};

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
    
    // Reflected light vector
    const float3 w = vsin.normal * dot(vToL, vsin.normal);
    const float3 r = w * 2.0f - vToL;
    
    // Calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (DiffuseColor * DiffuseIntensity) * SpecularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(vsin.worldPos))), SpecularPower);
    
    // Final color
    return float4(saturate((diffuse + Ambient + specular) * MaterialColor), 1.0f);
}