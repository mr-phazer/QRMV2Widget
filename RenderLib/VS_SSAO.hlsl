#include "layout.hlsli"
#include "VS_ConstBuffers.hlsli"

PixelInputType main(in VertexInputType vin)
{
    PixelInputType vout;
// Transform to view space.
    float4x4 mWorldView = mul(mWorld, mView);
    vout.position =  mul(float4(vin.position.xyz, 1.0f), mWorldView).xyzw;
    vout.normal = float4(mul(vin.normal.xyz, (float3x3) mWorldInvTransposeView), 1);

// Transform to homogeneous clip space.
    //vout.position = mul(float4(vin.position.xyz, 1.0f), mWorldViewProj);

    // Output vertex attributes for interpolation across triangle.
    vout.tex = mul(float4(vin.tex, 0.0f, 1.0f), mTexTransform).xy;
    
    
    vout.color = 0;
    vout.tangent = 0;
    vout.binormal = 0;
    vout.viewDirection = 0;
     
    
    return vout;
}