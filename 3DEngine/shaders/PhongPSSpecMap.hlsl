cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attenuationConst;
    float attenuationLin;
    float attenuationQuad;
};

struct PixelIn
{
    float3 worldPos : POSITION;
    float3 n : NORMAL;
    float2 tc : TEXCOORD;
};

Texture2D tex;
Texture2D spec;
SamplerState splr;

float4 main(PixelIn input) : SV_TARGET
{
    //Fragment to Light vector data
    const float3 vToL = lightPos - input.worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    //Diffuse attenuation
    const float att = 1.0f / (attenuationConst + attenuationLin * distToL + attenuationQuad * (distToL * distToL));
    
    //Diffise intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
    
    //Reflected light vector
    const float3 w = input.n * dot(vToL, input.n);
    const float3 r = w * 2.0f - vToL;
    
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float4 specularSample = spec.Sample(splr, input.tc);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(input.worldPos))), specularPower);
    
    //Final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular * specularReflectionColor), 1.0f);
}