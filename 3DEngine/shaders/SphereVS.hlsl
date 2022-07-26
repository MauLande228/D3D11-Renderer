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
    //float3 pos : SV_Position;
};

struct VsOut
{
    float4 posH : SV_Position;
    float4 color : COLOR;
};

VsOut main(VsIn input)
{
    VsOut vout;
    
    vout.posH = mul(float4(input.pos, 1.0f), transform);
    vout.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    
    return vout;
}