cbuffer CBuf
{
    float4 Color;
};

float4 main() : SV_TARGET
{
    return Color;
}