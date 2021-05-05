    

// layout for vertex INPUT from app
struct VertexInputType
{	
    float4 position : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
	
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
     
    float4 Weights : BLENDWEIGHTS;
    uint4 BoneIndices : BLENDINDICES;
    float4 color : COLOR;
    
    //int SV_InstanceID
};



// layout for vertex output to the pixel shader
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;    

    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;       
    
    float4 color : COLOR;                                            
    float3 viewDirection : TEXCOORD1;
        
};

struct SSAO_PixelInputType
{
    float4 positionV : SV_POSITION;
    //float4 positionV : SV_POSITION;
    float3 positionH : TEXCOORD0;
    
    float2 tex : TEXCOORD1;
    float2 tex2 : TEXCOOR2;
    
    float4 normalV : NORMAL;    

    //float3 tangent : TANGENT;
    //float3 binormal : BINORMAL;       
    
    //float4 color : COLOR;                                            
    //float3 viewDirection : TEXCOORD1;
        
};
