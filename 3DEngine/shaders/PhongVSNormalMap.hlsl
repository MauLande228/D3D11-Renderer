cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VsIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitan : BITANGENT;
    float2 tc : TEXCOORD;
};

struct VsOut
{
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitan : BITANGENT;
    float2 tc : TEXCOORD;
    float4 pos : SV_Position;
};

VsOut main(VsIn input)
{
    VsOut vsout;
    
    vsout.viewPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    vsout.normal = mul(input.normal, (float3x3) modelView);
    vsout.tangent = mul(input.tangent, (float3x3) modelView);
    vsout.bitan = mul(input.bitan, (float3x3) modelView);
    vsout.pos = mul(float4(input.pos, 1.0f), modelViewProj);
    vsout.tc = input.tc;
    
    return vsout;
}