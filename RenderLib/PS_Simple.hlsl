#include "layout.hlsli"
#include "textures.hlsli"
#include "tone_mapping.hlsli"



float4 main(in PixelInputType input) : SV_TARGET
{
    float4 position = input.position;
    float2 tex=  input.tex;
    
    float3 normal =  input.normal;
    float3 tangent = input.tangent;
    float3 binormal = input.binormal;    
    
	return float4(input.color.rgb, 1.0f);
}