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

cbuffer ObjectCBuf
{
    bool NormalMapEnabled;
    float padding[3];
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
    if (NormalMapEnabled)
    {
        // Build transform rotation into tangent space
        const float3x3 tanToView = float3x3(
            normalize(input.tangent),
            normalize(input.bitan),
            normalize(input.normal));
        
        // Unpack normal from map into tangent space
        const float3 normalSample = nmap.Sample(splr, input.tc).xyz;
        input.normal = normalSample * 2.0f - 1.0f;
        
        // Bring normal from taspace into view space
        input.normal = mul(input.normal, tanToView);

    }
    
    //Fragment to Light vector data
    const float3 vToL = lightPos - input.viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    //Diffuse attenuation
    const float att = 1.0f / (attenuationConst + attenuationLin * distToL + attenuationQuad * (distToL * distToL));
    
    //Diffise intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, input.normal));
    
    //Reflected light vector
    const float3 w = input.normal * dot(vToL, input.normal);
    const float3 r = w * 2.0f - vToL;
    
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float4 specularSample = spec.Sample(splr, input.tc);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(input.viewPos))), specularPower);
    
    //Final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, input.tc).rgb + specular * specularReflectionColor), 1.0f);
}