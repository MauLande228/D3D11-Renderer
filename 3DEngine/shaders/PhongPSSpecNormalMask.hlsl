#include "ShaderUtils.hlsli"
#include "LightData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    bool NormalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

struct PixelIn
{
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitan : BITANGENT;
    float2 tc : TEXCOORD;
    float4 pos : SV_Position;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;

SamplerState splr;

float4 main(PixelIn input) : SV_TARGET
{
    float4 dtex = tex.Sample(splr, input.tc);
    
    clip(dtex.a < 0.1f ? -1 : 1);
    if(dot(input.normal, input.viewPos) >= 0.0f)
    {
        input.normal = -input.normal;
    }
    
    input.normal = normalize(input.normal);
    if (NormalMapEnabled)
    {
        input.normal = MapNormal(normalize(input.tangent), normalize(input.bitan), input.normal, input.tc, nmap, splr);
    }
    
    // Fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(lights.viewLightPos, input.viewPos);
    
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, input.tc);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    const float att = Attenuate(lights.attConst, lights.attLin, lights.attQuad, lv.distToL);
    const float3 diffuse = Diffuse(lights.diffuseColor, lights.diffuseIntensity, att, lv.dirToL, input.normal);
    const float3 specularReflected = Speculate(
        specularReflectionColor,
        1.0f,
        input.normal,
        lv.vToL,
        input.viewPos,
        att,
        specularPower);
    
    // Final color = attenuate diffuse and ambient by diffuse texture color add specular reflected 
    return float4(saturate((diffuse + lights.ambient) * dtex.rgb + specularReflected), dtex.a);
}