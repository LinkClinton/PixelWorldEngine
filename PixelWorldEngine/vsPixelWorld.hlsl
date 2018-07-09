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
    uint id : SV_INSTANCEID;
};

struct InstanceData
{
    int4 renderObjectID;
    matrix worldTransform;
    float4 renderColor;
};

cbuffer Camera : register(b0)
{
    matrix project;
};

cbuffer RenderConfig : register(b1)
{
    int4 maxRenderObjectID;
    float4 unused[3];
};

Texture2D Texture0 : register(t0);

StructuredBuffer<InstanceData> instanceData : register(t1);

SamplerState sampler0 : register(s0);

OutputData main(InputData input, uint id : SV_INSTANCEID)
{
    OutputData result;

    result.positionH = mul(float4(input.position, 1.f), instanceData[id].worldTransform);
    result.positionH = mul(result.positionH, project);

    result.color = input.color;
    result.tex0 = input.tex0;
    result.id = id;

    return result;
}