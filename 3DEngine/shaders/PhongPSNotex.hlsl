#include "ShaderUtils.hlsli"
#include "LightData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};


float4 main(float3 viewPos : POSITION, float3 Normal : NORMAL) : SV_TARGET
{
    // normalize the mesh normal
    Normal = normalize(Normal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(lights.viewLightPos, viewPos);
	// attenuation
    const float att = Attenuate(lights.attConst, lights.attLin, lights.attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(lights.diffuseColor, lights.diffuseIntensity, att, lv.dirToL, Normal);
    // specular
    const float3 specular = Speculate(
        specularColor.rgb, 1.0f, Normal,
        lv.vToL, viewPos, att, specularPower
    );
	// final color
    return float4(saturate((diffuse + lights.ambient) * materialColor.rgb + specular), 1.0f);
}