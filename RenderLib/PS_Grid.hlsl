#include "layout.hlsli"
#include "textures.hlsli"
#include "tone_mapping.hlsli"



float4 main(in PixelInputType input) : SV_TARGET
{
    float4 position = input.position;
    float2 tex = input.tex;
    
    float3 normal = input.normal;
    float3 tangent = input.tangent;
    float3 binormal = input.binormal;
        
//float4 color = input.color* (input.position.z;
    float4 color = input.color;
    //input.position.z;
    
    
    return float4(color.rgb, 1.0f);
}