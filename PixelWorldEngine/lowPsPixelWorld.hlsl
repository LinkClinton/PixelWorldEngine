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
    int4 mergeTextureFormat;
    float4 unused[3];
};

cbuffer InstanceDatas : register(b2)
{
    InstanceData instanceData[100];
}

Texture2D Texture0 : register(t1);
Texture2D Texture1 : register(t2);
Texture2D Texture2 : register(t3);
Texture2D Texture3 : register(t4);

SamplerState sampler0 : register(s0);

float4 main(OutputData input, uint id : SV_INSTANCEID) : SV_TARGET
{
    if (instanceData[id].setting[0] == 0)
        return instanceData[id].renderColor;

    int which = instanceData[id].setting[1];
    int format = mergeTextureFormat[which];

    float4 texColor = float4(0, 0, 0, 0);

    if (which == 0)
    {
        if (format == PIXELFORMAT_A8)
            texColor = float4(1.0f, 1.0f, 1.0f, Texture0.Sample(sampler0, input.tex0).a);
        else
            texColor = Texture0.Sample(sampler0, input.tex0);
    }

    if (which == 1)
    {
        if (format == PIXELFORMAT_A8)
            texColor = float4(1.0f, 1.0f, 1.0f, Texture1.Sample(sampler0, input.tex0).a);
        else
            texColor = Texture1.Sample(sampler0, input.tex0);
    }

    if (which == 2)
    {
        if (format == PIXELFORMAT_A8)
            texColor = float4(1.0f, 1.0f, 1.0f, Texture2.Sample(sampler0, input.tex0).a);
        else
            texColor = Texture2.Sample(sampler0, input.tex0);
    }

    if (which == 3)
    {
        if (format == PIXELFORMAT_A8)
            texColor = float4(1.0f, 1.0f, 1.0f, Texture3.Sample(sampler0, input.tex0).a);
        else
            texColor = Texture3.Sample(sampler0, input.tex0);
    }
    
    return texColor * instanceData[id].renderColor;
}