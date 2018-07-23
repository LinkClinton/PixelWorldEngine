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
    uint id : SV_INSTANCEID;
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
    int4 mergeTextureFormat;
    float4 unused[3];
};

StructuredBuffer<InstanceData> instanceData : register(t0);

Texture2DArray Texture : register(t1);

SamplerState sampler0 : register(s0);

OutputData main(InputData input, uint id : SV_INSTANCEID)
{
    OutputData result;

    result.positionH = mul(float4(input.position, 1.f), instanceData[id].worldTransform);
    result.positionH = mul(result.positionH, project);

    result.color = input.color;
    result.tex0 = mul(float4(input.tex0, 0, 1), instanceData[id].texcoordTransform).xy;
    result.id = id;

    return result;
}