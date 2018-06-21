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

cbuffer Transform : register(b1)
{
    matrix world;
};

Texture2D Texture0 : register(t0);

SamplerState sampler0 : register(s0);

OutputData main(InputData input)
{
    OutputData result;

    result.positionH = mul(float4(input.position, 1.f), world);
    result.positionH = mul(result.positionH, project);

    result.color = input.color;
    result.tex0 = input.tex0;

    return result;
}