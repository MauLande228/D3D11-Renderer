cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VsIn
{
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 tc       : TEXCOORD;
};

struct VsOut
{
    float3 worldPos : POSITION;
    float3 normal   : NORMAL;
    float2 tc       : TEXCOORD;
    float4 pos      : SV_Position;
};

VsOut main(VsIn input )
{
    VsOut vsout;
    
    vsout.worldPos = (float3) mul(float4(input.pos, 1.0f), modelView);
    vsout.normal = mul(input.normal, (float3x3) modelView);
    vsout.pos = mul(float4(input.pos, 1.0f), modelViewProj);
    vsout.tc = input.tc;
    
    return vsout;
}