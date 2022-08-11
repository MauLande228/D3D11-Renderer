#include "ShaderUtils.hlsli"
#include "LightData.hlsli"
#include "PointLight.hlsli"

cbuffer Object
{
    float SpecularIntensity;
    float SpecularPower;
    float padding[2];
};

struct VsOut
{
    float3 worldPos : POSITION;
    float3 normal   : NORMAL;
    float2 tc       : TEXCOORD;
    float4 pos      : SV_Position;
};

Texture2D tex;
SamplerState splr;

float4 main(VsOut input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float4 fragmentOutput = { 0, 0, 0, 0 };
    for (unsigned int i = 0; i < 2; i++)
    {
        //Fragment to Light vector data
        const LightVectorData lv = CalculateLightVectorData(lights[i].viewLightPos, input.worldPos);
    
        const float att = Attenuate(lights[i].attConst, lights[i].attLin, lights[i].attQuad, lv.distToL);
        const float3 diffuse = Diffuse(lights[i].diffuseColor, lights[i].diffuseIntensity, att, lv.dirToL, input.normal);
        const float3 specular = Speculate(lights[i].diffuseColor, lights[i].diffuseIntensity, input.normal, lv.vToL, input.worldPos, att, SpecularPower);
        
        fragmentOutput += float4(saturate((diffuse + lights[i].ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);

    }
    
        return fragmentOutput;
}