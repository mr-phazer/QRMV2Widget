#include "layout.hlsli"



cbuffer PerModelConstantBuffer
{
    float4x4 mWorld; // world matrix for object
    float4x4 View; // view matrix
    float4x4 Projection; // projection matrix
    float4x4 ViewI; // projection matrix
    float4x4 mRotEnv; // projection matrix
	
    float3 cameraPosition;
    int is_weapon;
	
    float3 cameraLookAt;
    int static_model;
    
    float4 color;
    
    float4x4 rot_x;
    float4x4 rot_y;
    float4x4 rot_z;
    float4x4 mWeapon;
  
    float4 rot;
    
    float4x4 tranforms[256];
    float4x4 inverse[256];
};

struct CPerModelConstantBuffer
{
    float4x4 mWorld; // world matrix for object
    float4x4 View; // view matrix
    float4x4 Projection; // projection matrix
    float4x4 ViewI; // projection matrix
    float4x4 mRotEnv; // projection matrix
	
    float3 cameraPosition;
    int is_weapon;
	
    float3 cameraLookAt;
    int static_model;
    
    float4 color;
    
    float4x4 rot_x;
    float4x4 rot_y;
    float4x4 rot_z;
    float4x4 mWeapon;
  
    float4 rot;
    
    //float4x4 tranforms[256];
    //float4x4 inverse[256];
};


cbuffer test
{
    CPerModelConstantBuffer PVSBuffers[100];
};
//#define _d3d11_MAX_INSTACES 4096

//ConstantBuffer<PerModelConstantBuffer> MyConstBuffer[_d3d11_MAX_INSTACES] : register(b0, space1);

PixelInputType main(in VertexInputType input/*, uint inst_id : SV_InstanceID*/)
{
    PixelInputType output;    
    
    
    output.position = mul(input.position, mWorld);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
                                               
    output.normal = mul(input.normal, (float3x3) mWorld);
    output.tangent = mul(input.tangent, (float3x3) mWorld);
    output.binormal = mul(input.binormal, (float3x3) mWorld);
    
    output.color = input.color;
    
    return output;
    
}