#include "pch.hpp"

namespace PixelWorldEngine {
	constexpr char vsPixelWorldDefaultShaderCode[] = "#pragma pack_matrix(row_major)  \n \n#define MAX_RENDER_OBJECT 4 \n \nstruct InputData \n{ \n    float3 position : POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \nstruct OutputData \n{ \n    float4 positionH : SV_POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \ncbuffer Camera : register(b0) \n{ \n    matrix project; \n}; \n \ncbuffer Transform : register(b1) \n{ \n    matrix world; \n}; \n \ncbuffer RenderObjectID : register(b2) \n{ \n    int renderObjectID[MAX_RENDER_OBJECT]; \n    float4 unused[3]; \n}; \n \nTexture2D Texture0 : register(t0); \nTexture2D Texture1 : register(t1); \nTexture2D Texture2 : register(t2); \nTexture2D Texture3 : register(t3); \n \nSamplerState sampler0 : register(s0); \n \nOutputData main(InputData input) \n{ \n    OutputData result; \n \n    result.positionH = mul(float4(input.position, 1.f), world); \n    result.positionH = mul(result.positionH, project); \n \n    result.color = input.color; \n    result.tex0 = input.tex0; \n \n    return result; \n}\n";
	constexpr char psPixelWorldDefaultShaderCode[] = "#pragma pack_matrix(row_major)  \n \n#define MAX_RENDER_OBJECT 4 \n \nstruct InputData \n{ \n    float3 position : POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \nstruct OutputData \n{ \n    float4 positionH : SV_POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \ncbuffer Camera : register(b0) \n{ \n    matrix project; \n}; \n \ncbuffer Transform : register(b1) \n{ \n    matrix world; \n}; \n \ncbuffer RenderObjectID : register(b2) \n{ \n    int renderObjectID[MAX_RENDER_OBJECT]; \n    float4 unused[3]; \n}; \n \nTexture2D Texture0 : register(t0); \nTexture2D Texture1 : register(t1); \nTexture2D Texture2 : register(t2); \nTexture2D Texture3 : register(t3); \n \nSamplerState sampler0 : register(s0); \n \nstatic const float eps = 0.00001f; \n \nfloat4 main(OutputData input) : SV_TARGET \n{ \n    if (renderObjectID[0] != 0) \n    { \n        float4 result = Texture0.Sample(sampler0, input.tex0); \n \n        if (result.a >= eps) return result; \n    } \n \n    if (renderObjectID[1] != 0) \n    { \n        float4 result = Texture1.Sample(sampler0, input.tex0); \n \n        if (result.a >= eps) \n            return result; \n    } \n \n    if (renderObjectID[2] != 0) \n    { \n        float4 result = Texture2.Sample(sampler0, input.tex0); \n \n        if (result.a >= eps) \n            return result; \n    } \n \n    if (renderObjectID[3] != 0) \n    { \n        float4 result = Texture3.Sample(sampler0, input.tex0); \n \n        if (result.a >= eps) \n            return result; \n    }    \n     \n    return float4(0, 0, 0, 0); \n}\n";
	constexpr char vsApplicationDefaultShaderCode[] = "#pragma pack_matrix(row_major)  \n \nstruct InputData \n{ \n    float3 position : POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \nstruct OutputData \n{ \n    float4 positionH : SV_POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \ncbuffer Camera : register(b0) \n{ \n    matrix project; \n}; \n \nTexture2D Texture : register(t0); \n \nSamplerState sample : register(s0); \n \nOutputData main(InputData input) \n{ \n    OutputData result; \n     \n    result.positionH = mul(float4(input.position, 1.0f), project); \n \n    result.color = input.color; \n    result.tex0 = input.tex0; \n \n    return result; \n} \n";
	constexpr char psApplicationDefaultShaderCode[] = "#pragma pack_matrix(row_major)  \n \nstruct InputData \n{ \n    float3 position : POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \nstruct OutputData \n{ \n    float4 positionH : SV_POSITION; \n    float4 color : COLOR; \n    float2 tex0 : TEXCOORD0; \n}; \n \ncbuffer Camera : register(b0) \n{ \n    matrix project; \n}; \n \nTexture2D Texture : register(t0); \n \nSamplerState sample : register(s0); \n \nfloat4 main(OutputData input) : SV_TARGET \n{ \n    return Texture.Sample(sample, input.tex0); \n}\n";

}