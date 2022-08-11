#include "ShaderUtils.hlsli"
#include "LightData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjCBuf
{
    float   SpecularIntensity;
    float   SpecularPower;
    bool    NormalMapEnabled;
    float   Padding[1];
};

struct PixelIn
{
    float3 viewPos  : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 bitan    : BITANGENT;
    float2 tc       : TEXCOORD;
    float4 pos      : SV_Position;
};

Texture2D tex;
Texture2D nmap : register(t2);

SamplerState splr;

float4 main(PixelIn input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    if(NormalMapEnabled)
    {
        input.normal = MapNormal(normalize(input.tangent), normalize(input.bitan), input.normal, input.tc, nmap, splr);
    }
    
    float4 fragmentOutput = { 0, 0, 0, 0 };
    for (unsigned int i = 0; i < 2; i++)
    {
        // Fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(lights[i].viewLightPos, input.viewPos);
    
        const float att = Attenuate(lights[i].attConst, lights[i].attLin, lights[i].attQuad, lv.distToL);
        const float3 diffuse = Diffuse(lights[i].diffuseColor, lights[i].diffuseIntensity, att, lv.dirToL, input.normal);
        const float3 specular = Speculate(
        lights[i].diffuseColor,
        lights[i].diffuseIntensity,
        input.normal,
        lv.vToL,
        input.viewPos,
        att,
        SpecularPower);
        
        fragmentOutput += float4(saturate((diffuse + lights[i].ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);
    }
    
    // Final color
    return fragmentOutput;
}