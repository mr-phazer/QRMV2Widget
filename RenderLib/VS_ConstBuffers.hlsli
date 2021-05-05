#pragma once

cbuffer PerModelConstantBuffer : register(b0)
{
    float4x4 mWorld; // world matrix for object
    float4x4 View; // view matrix
    float4x4 Projection; // projection matrix
    float4x4 ViewI; // projection matrix
    float4x4 mRotEnv; // env transform
    float4x4 mWorldInvTransposeView;
    float4x4 mWorldViewProj;
    float4x4 mTexTransform;
    
    
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
    
    uint show_reflections;
    uint is_diffuse_gamma;
    uint is_specular_gamma;
    uint has_alpha;                  
    
    float4x4 tranforms[256];
    float4x4 inverse[256];
};

cbuffer VS_PerFrame_ConstantBuffer{
    
    float4x4 mView;
    float4x4 mProjection;
    float4x4 mViewInverse;
        
    float4x4 eyePosition;
    
    
}
