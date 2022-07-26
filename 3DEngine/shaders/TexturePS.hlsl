struct VsOut
{
    float4 posH : SV_Position;
    float2 tc : TEXCOORD;
};

Texture2D tex;
SamplerState splr;

float4 main(VsOut pin) : SV_Target
{
    return tex.Sample(splr, pin.tc);
}