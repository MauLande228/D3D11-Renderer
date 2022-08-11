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
    
	//Fragment to Light vector data
    const LightVectorData lv = CalculateLightVectorData(lights.viewLightPos, input.worldPos);
    
    const float att = Attenuate(lights.attConst, lights.attLin, lights.attQuad, lv.distToL);
    const float3 diffuse = Diffuse(lights.diffuseColor, lights.diffuseIntensity, att, lv.dirToL, input.normal);
    const float3 specular = Speculate(lights.diffuseColor, lights.diffuseIntensity, input.normal, lv.vToL, input.worldPos, att, SpecularPower);
    
    return float4(saturate((diffuse + lights.ambient) * tex.Sample(splr, input.tc).rgb + specular), 1.0f);
}