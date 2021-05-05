#pragma once

#define NUMBER_OF_LIGHTS 1

struct LightSourceData
{
    float3 lightDirection;
    float radiannce;
    float4 lightColor;    
};               

cbuffer PS_LightBuffer : register(b0)
{
    LightSourceData LightData[NUMBER_OF_LIGHTS];
    float4 env_color;        
    float4x4 mEnv;
}

//cbuffer PS_PerFrame_ConstBuffer : register(b0)
//{
//    float4x4 rotEnv;
    
//    float3 lightDirection;
//    float radiannce;

//    float4 ambientLight;
//    float4 diffuseColor;
//    float4 specularColor;
		
//    float ambientFactor;
//    float diffuseFactor;
//    float specularFactor;
//    float roughnessFactor;
    
   
//}

cbuffer PS_PerModel_ConstBuffer : register(b1)
{

    float exposure;
    float reflectivity;
    float ambientlight;
    float reserved;

    uint alpha_used;
    uint render_fresnel;
    uint reversed1;
    uint reversed2;

    float4 color_no_tex;
    
    float4 color1;
    float4 color2;
    float4 color3;              
    
    uint PisTextureSRGB[64];                                                                                                        
    
}