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
	
    float4 fragmentOutput = { 0, 0, 0, 0 };
    for (unsigned int i = 0; i < 2; i++)
    {
        // fragment to light vector data
        const LightVectorData lv = CalculateLightVectorData(lights[i].viewLightPos, viewPos);
	// attenuation
        const float att = Attenuate(lights[i].attConst, lights[i].attLin, lights[i].attQuad, lv.distToL);
	// diffuse
        const float3 diffuse = Diffuse(lights[i].diffuseColor, lights[i].diffuseIntensity, att, lv.dirToL, Normal);
    // specular
        const float3 specular = Speculate(
        specularColor.rgb, 1.0f, Normal,
        lv.vToL, viewPos, att, specularPower
        );
        
        fragmentOutput += float4(saturate((diffuse + lights[i].ambient) * materialColor.rgb + specular), 1.0f);
    }
    
	// final color
    return fragmentOutput;
}