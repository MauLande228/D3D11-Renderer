cbuffer CBuf
{
    float4 face_colors[6];
};

struct VsOut
{
    float4 posH : SV_Position;
    float4 color : COLOR;
};

float4 main(VsOut pin) : SV_Target
{
    return pin.color;
}