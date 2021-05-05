#include "layout.hlsli"
#include "VS_ConstBuffers.hlsli"

PixelInputType main(in VertexInputType input)
{
    PixelInputType output;
        
    float4x4 PM[4];
    PM[0] = mul(inverse[input.BoneIndices.x], tranforms[input.BoneIndices.x]);
    PM[1] = mul(inverse[input.BoneIndices.y], tranforms[input.BoneIndices.y]);
    PM[2] = mul(inverse[input.BoneIndices.z], tranforms[input.BoneIndices.z]);
    PM[3] = mul(inverse[input.BoneIndices.w], tranforms[input.BoneIndices.w]);    

    output.position = 0;
    output.normal = 0;
    output.tangent = 0;
    output.binormal = 0;
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.position += input.Weights[i] * mul(input.position, PM[i]);
        output.normal += input.Weights[i] * mul(input.normal, (float3x3) PM[i]); // only use rotation part (3x3) of matrices
        output.tangent += input.Weights[i] * mul(input.tangent, (float3x3) PM[i]);
        output.binormal += input.Weights[i] * mul(input.binormal, (float3x3) PM[i]);
    }
    
    //  world transform
    output.position = mul(output.position, mWorld);
    
    // save world space position for camera calculations
    float3 worldPosition = output.position.xyz;                 

    output.position = mul(float4(output.position.xyz, 1), View);
    output.position = mul(float4(output.position.xyz, 1), Projection);

    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----    
    output.normal = mul(output.normal, (float3x3) mWorld);
    output.normal = normalize(output.normal);
    
    output.tangent = mul(output.tangent, (float3x3) mWorld);
    output.tangent = normalize(output.tangent);
    
    output.binormal = mul(output.binormal, (float3x3) mWorld);
    output.binormal = normalize(output.binormal);                                                                
        
    // UVs
    output.tex = input.tex;                             
    
    // calculate camera view direction for pixel shader    
    output.viewDirection.xyz = normalize(
	(float3) ViewI[3] - worldPosition);                                    
                               
    // vertex color (never used)
    output.color = input.color;
    
    return output;
}