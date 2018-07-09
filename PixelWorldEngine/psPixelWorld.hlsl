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

float2 getTexcoord(float2 originTexcoord, int renderObjectID, int maxRenderObjectID)
{
    float distance = 1.0f / maxRenderObjectID;

    return float2((renderObjectID - 1 + originTexcoord.x) * distance, originTexcoord.y);
}

float4 main(OutputData input, uint id : SV_INSTANCEID) : SV_TARGET
{
    if (instanceData[id].renderObjectID[0] == 0 && instanceData[id].renderObjectID[1] == 0 &&
        instanceData[id].renderObjectID[2] == 0 && instanceData[id].renderObjectID[3] == 0)
        return instanceData[id].renderColor;

    if (instanceData[id].renderObjectID[0] != 0)
    {
        float2 texcoord = getTexcoord(input.tex0, instanceData[id].renderObjectID[0],
            maxRenderObjectID[0]);

        float4 result = Texture0.Sample(sampler0, texcoord);

        if (result.a > 0)
        {
            result.a *= instanceData[id].renderColor.a;
            return result;
        }
    }

    if (instanceData[id].renderObjectID[1] != 0)
    {
        float2 texcoord = getTexcoord(input.tex0, instanceData[id].renderObjectID[1],
            maxRenderObjectID[0]);

        float4 result = Texture0.Sample(sampler0, texcoord);

        if (result.a > 0)
        {
            result.a *= instanceData[id].renderColor.a;
            return result;
        }
    }

    if (instanceData[id].renderObjectID[2] != 0)
    {
        float2 texcoord = getTexcoord(input.tex0, instanceData[id].renderObjectID[2],
            maxRenderObjectID[0]);

        float4 result = Texture0.Sample(sampler0, texcoord);

        if (result.a > 0)
        {
            result.a *= instanceData[id].renderColor.a;
            return result;
        }
    }

    if (instanceData[id].renderObjectID[3] != 0)
    {
        float2 texcoord = getTexcoord(input.tex0, instanceData[id].renderObjectID[3],
            maxRenderObjectID[0]);

        float4 result = Texture0.Sample(sampler0, texcoord);

        if (result.a > 0)
        {
            result.a *= instanceData[id].renderColor.a;
            return result;
        }
    }
    
    clip(-1);

    return float4(0, 0, 0, 0);
}