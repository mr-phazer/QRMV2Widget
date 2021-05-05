#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

#include "half.h"

using half_float::half;

typedef half float16;

struct half4
{
	half x = half(0.f);
	half y = half(0.f);
	half z = half(0.f);
	half w = half(0);
};

struct half2
{
	half x = half(0.f);
	half y = half(0.f);
};

namespace RigidModelV2
{
	struct Vertex_Raw_Cinematic/* : public IVertexBase*/
	{
		/*constexpr static size_t _vertex_size = 32;
		size_t size() { return (size_t)32; };
		static constexpr size_t _size = 32;*/

		// offset 0
		half4 position;

		DirectX::PackedVector::XMUBYTE4 bone_indices;
		DirectX::PackedVector::XMUBYTE4 bone_weights;

		DirectX::PackedVector::XMUBYTE4 normal;

		half2 uv;

		DirectX::PackedVector::XMUBYTE4 bitangent;
		DirectX::PackedVector::XMUBYTE4 tangent;

		// offset 8
		/*uint8_t joint_id1 = 0;
		uint8_t joint_id2 = 0;
		uint8_t joint_id3 = 0;
		uint8_t joint_id4 = 0;*/

		// offset 12
		/*uint8_t weight1 = 255;
		uint8_t weight2 = 0;
		uint8_t weight3 = 0;
		uint8_t weight4 = 0;*/

		// 16
	/*	uint8_t norm_x = 0;
		uint8_t norm_y = 0;
		uint8_t norm_z = 0;
		uint8_t norm_w = 0;*/

		// 20

		// 24

		//half u;
		//half v;

		//uint8_t bitangent_x = 0;
		//uint8_t bitangent_y = 0;
		//uint8_t bitangent_z = 0;
		//uint8_t bitangent_w = 0;

		//// 28
		//uint8_t tangent_x = 0;
		//uint8_t tangent_y = 0;
		//uint8_t tangent_z = 0;
		//uint8_t tangent_w = 0;

		// 32
		//rgba8 color = { 0, 0, 0, 1 };
		DirectX::PackedVector::XMUBYTE4 color = { 0.f, 0.f, 0.f, 1.f };
		// 36
	};

	struct Vertex_Raw_Weighted
	{
		half4 position;

		//half pos_x;
		//half pos_y;
		//half pos_z;
		//half pos_w; // offset 8

		DirectX::PackedVector::XMUBYTE2 bone_indices;
		DirectX::PackedVector::XMUBYTE2 bone_weights;

		//int8_t joint_id1;
		//int8_t joint_id2; // offset 10p

		//uint8_t weight1; // vertex->weight = uint8_t_t_norm / 255.0f;
							//	vertex->weight1 = 1.0f - vertex->weight;

		//uint8_t weight2; // offset 12
		DirectX::PackedVector::XMUBYTE4 normal;

		//uint8_t normal_x;
		//uint8_t normal_y;
		//uint8_t normal_z;
		//uint8_t normal_w; // offet 16

		half2 uv;

		//half u;
		//half v; // offet 20

		DirectX::PackedVector::XMUBYTE4 tangent;
		DirectX::PackedVector::XMUBYTE4 bitangent;

		//uint8_t tangent_x;
		//uint8_t tangent_y;
		//uint8_t tangent_z;
		//uint8_t tangent_w; // offet 25

		//uint8_t bitangent_x;
		//uint8_t bitangent_y;
		//uint8_t bitangent_z;
		//uint8_t bitangent_w; // offet 28

		//rgba8 color = { 0, 0, 0, 255 };;
	};

	struct Vertex_Raw_Default {
		half4 position;
		/*float16 pos_x;
		float16 pos_y;
		float16 pos_z;
		float16 pos_w;*/

		half2 uv1;
		//float16 u;
		//float16 v;  // 12
		half2 uv2;

		//float16 textcord_u2;
		//float16 textcord_v2; // 16

		uint8_t normal_x;
		uint8_t normal_y;
		uint8_t normal_z;
		uint8_t normal_w; // 20

		DirectX::PackedVector::XMUBYTE4 normal;

		DirectX::PackedVector::XMUBYTE4 tangent;
		DirectX::PackedVector::XMUBYTE4 bitangent;

		//uint8_t tangent_x;
		//uint8_t tangent_y;
		//uint8_t tangent_z;
		//uint8_t tangent_w;

		//// 24
		//uint8_t bitangent_x;
		//uint8_t bitangent_y;
		//uint8_t bitangent_z;
		//uint8_t bitangent_w; // 28

		DirectX::PackedVector::XMUBYTE4 color = { 0.f, 0.f, 0.f, 0.f };
	};
}