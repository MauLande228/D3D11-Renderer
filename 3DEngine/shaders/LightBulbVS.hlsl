cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
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
    float4 pos : SV_Position;
};

VsOut main(VsIn input)
{
    VsOut vout;
    
    vout.pos = mul(float4(input.pos, 1.0f), modelViewProj);
    
    return vout;
}