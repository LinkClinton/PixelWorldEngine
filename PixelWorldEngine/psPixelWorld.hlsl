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

Texture2D Texture0 : register(t1);
Texture2D Texture1 : register(t2);
Texture2D Texture2 : register(t3);
Texture2D Texture3 : register(t4);


SamplerState sampler0 : register(s0);

float4 main(OutputData input, uint id : SV_INSTANCEID) : SV_TARGET
{
    if (instanceData[id].setting[0] == 0)
        return instanceData[id].renderColor;

    if (instanceData[id].setting[1] == 0)
    {
        float4 result = Texture0.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = result.a * instanceData[id].renderColor.a;

            return result;
        }
    }

    if (instanceData[id].setting[1] == 1)
    {
        float4 result = Texture1.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = result.a * instanceData[id].renderColor.a;

            return result;
        }
    }

    if (instanceData[id].setting[1] == 2)
    {
        float4 result = Texture2.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = result.a * instanceData[id].renderColor.a;

            return result;
        }
    }

    if (instanceData[id].setting[1] == 3)
    {
        float4 result = Texture3.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = result.a * instanceData[id].renderColor.a;

            return result;
        }
    }
   
    
    clip(-1);

    return float4(0, 0, 0, 0);
}