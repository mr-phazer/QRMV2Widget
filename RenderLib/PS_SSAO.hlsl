#include "VS_ConstBuffers.hlsli"
#include "layout.hlsli"													



float4 main(in PixelInputType pin) : SV_TARGET
{                     
    float3 normal = normalize(pin.normal);        
    
    return float4(normal.xyz, pin.position.z);
}