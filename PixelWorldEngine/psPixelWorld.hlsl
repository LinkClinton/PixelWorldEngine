#pragma pack_matrix(row_major) 

#define PIXELFORMAT_R8G8B8A8 28
#define PIXELFORMAT_A8 65

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

struct InstanceData
{
    int4 setting;
    matrix worldTransform;
    matrix texcoordTransform;
    float4 renderColor;
};

cbuffer Camera : register(b0)
{
    matrix project;
};

cbuffer RenderConfig : register(b1)
{
    float4 unused[4];
};

StructuredBuffer<InstanceData> instanceData : register(t0);

Texture2DArray Texture0 : register(t1);
Texture2DArray Texture1 : register(t2);

SamplerState sampler0 : register(s0);

float4 main(OutputData input, uint id : SV_INSTANCEID) : SV_TARGET
{
    if (instanceData[id].setting[0] == 0)
        return instanceData[id].renderColor;

    int which = instanceData[id].setting[1];
    
    float4 texColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (instanceData[id].setting[2] == 0) texColor = Texture0.Sample(sampler0, float3(input.tex0, which));
    if (instanceData[id].setting[2] == 1) texColor = float4(1.0f, 1.0f, 1.0f, Texture1.Sample(sampler0, float3(input.tex0, which)).w);

    return texColor * instanceData[id].renderColor;
}