cbuffer CBuf
{
    matrix transform;
};

struct VsIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENTU;
    float2 tc : TEXCOORD;
};

struct VsOut
{
    float4 posH : SV_Position;
    float2 tc : TEXCOORD;
};

VsOut main(VsIn input)
{
    VsOut vout;
    
    vout.posH = mul(float4(input.pos, 1.0f), transform);
    vout.tc = input.tc;
    
    return vout;
}