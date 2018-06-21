#pragma pack_matrix(row_major) 

struct InputData
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 tex0 : TEXCOORD0;
};

struct OutputData
{
    float4 positionH : SV_POSITION;
    float4 color : COLOR;
    float2 tex0 : TEXCOORD0;
};

cbuffer Camera : register(b0)
{
    matrix project;
};

Texture2D Texture : register(t0);

SamplerState sample : register(s0);

float4 main(OutputData input) : SV_TARGET
{
    return Texture.Sample(sample, input.tex0);
}