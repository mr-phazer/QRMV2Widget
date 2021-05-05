#include "layout.hlsli"
#include "VS_ConstBuffers.hlsli"

PixelInputType main(in VertexInputType input)
{
    PixelInputType output;
    
    //output.position = float4(input.position.xyz, 1);        
    
    //output.position.y -= 1;
    
    output.position = mul(float4(input.position.xyz, 1), mWorld);
    float3 worldPosition = output.position.xyz;
    
    output.position = mul(float4(output.position.xyz, 1), View);
    output.position = mul(float4(output.position.xyz, 1), Projection);
    
    
    
    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3) mWorld);
    output.normal = normalize(output.normal);
    
    output.tangent = mul(input.tangent, (float3x3) mWorld);
    output.tangent = normalize(output.tangent);
    
    output.binormal = mul(input.binormal, (float3x3) mWorld);
    output.binormal = normalize(output.binormal);
    
    output.color = input.color;
    
    output.viewDirection.xyz = normalize(
	(float3) ViewI[3] - worldPosition);
    
    //output.viewDirection.xyz = normalize(
    //cameraPosition.xyz   -worldPosition);
    
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    
    
    return output;
}