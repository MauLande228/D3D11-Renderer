#include "ShaderUtils.hlsli"
#include "LightData.hlsli"
#include "PointLight.hlsli"

struct PixelIn
{
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tc : TEXCOORD;
    float3 CamPos : CAMERAPOSITION;
    float4 pos : SV_Position;
};

Texture2D albedoMap;
Texture2D metallicMap;
Texture2D normalMap;
Texture2D roughnessMap;

SamplerState textureSampler;

static const float PI = 3.14159265359;

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;
    
    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

float4 main(PixelIn input) : SV_TARGET
{
    float4 albedo = albedoMap.Sample(textureSampler, input.tc);
    float metallic = metallicMap.Sample(textureSampler, input.tc).r;
    float roughness = roughnessMap.Sample(textureSampler, input.tc).r;
    float3 Normal = input.normal * normalMap.Sample(textureSampler, input.tc).rgb;
    
    float ao = 1.0f;
    
    float3 N = normalize(Normal);
    float3 V = normalize(input.CamPos.xyz - input.viewPos.xyz);
    float3 R = reflect(-V, N);
    
    float3 albedoC = albedo.rgb;
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedoC, metallic);
    
    
    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < 2; i++)
    {
        const LightVectorData lv = CalculateLightVectorData(lights[i].viewLightPos, input.viewPos);
        float distance = length(lv.vToL);
        const float attenuation = Attenuate(lights[i].attConst, lights[i].attLin, lights[i].attQuad, lv.distToL);
        const float3 diffuse = Diffuse(lights[i].diffuseColor, lights[i].diffuseIntensity, attenuation, lv.dirToL, input.normal);
        float3 radiance = diffuse * attenuation;
        
        float3 L = normalize(lights[i].viewLightPos - input.viewPos.xyz);
        float3 H = normalize(V + L);
        
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);
        
        float3 kS = F;
        float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
        kD *= 1.0f - metallic;
        
        float3 numerator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
        float3 specular = numerator / max(denominator, 0.001f);
        
        float NdotL = max(dot(N, L), 0.0f);
        Lo += (kD * albedoC / PI + specular) * radiance * NdotL;
    }
    
    float3 ambient = float3(0.03f, 0.03f, 0.03f) * albedoC * ao;
    float3 color = ambient + Lo;
    
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    float4 fragColor = float4(color, 1.0f);
    
    return fragColor;
}