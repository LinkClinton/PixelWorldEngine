#pragma pack_matrix(row_major) 

#define MAX_RENDER_OBJECT 4

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

cbuffer RenderObjectID : register(b2)
{
    int4 renderObjectID;
    float4 renderColor;
};

Texture2D Texture0 : register(t0);
Texture2D Texture1 : register(t1);
Texture2D Texture2 : register(t2);
Texture2D Texture3 : register(t3);

SamplerState sampler0 : register(s0);

float4 main(OutputData input) : SV_TARGET
{
    if (renderObjectID[0] == 0 && renderObjectID[1] == 0 && renderObjectID[2] == 0 && renderObjectID[3] == 0)
        return renderColor;

    if (renderObjectID[0] != 0)
    {
        float4 result = Texture0.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = renderColor.a;
            return result;
        }

    }

    if (renderObjectID[1] != 0)
    {
        float4 result = Texture1.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = renderColor.a;
            return result;
        }
    }

    if (renderObjectID[2] != 0)
    {
        float4 result = Texture2.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = renderColor.a;
            return result;
        }
    }

    if (renderObjectID[3] != 0)
    {
        float4 result = Texture3.Sample(sampler0, input.tex0);

        if (result.a > 0)
        {
            result.a = renderColor.a;
            return result;
        }
    }
    
    clip(-1);

    return float4(0, 0, 0, 0);
}