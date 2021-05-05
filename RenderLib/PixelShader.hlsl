struct VertexPixel
{

    float4 position : SV_POSITION;
    //float2 uv : TEXCOORD;
    //float3 normal : NORMAL;
    //float3 tangent : TANGENT;
    //float3 bitangent : BINORMAL;

            //float4 bin : TEXCOORD1;
};



float4 main(in VertexPixel input) : SV_TARGET
{

    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}