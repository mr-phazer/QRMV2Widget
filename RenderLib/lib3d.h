#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <algorithm>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

#include "..\DirectXTex\DDSTextureLoader\DDSTextureLoader11.h"

#include "..\SystemLib\ISerializeabe.h"

using namespace DirectX;

struct lib3d
{
	static constexpr XMFLOAT4X4 identity4x4()
	{
		return
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	};

	static inline constexpr float SNORM16_To_Float(int16_t in)
	{
		if (in == 32767)
			return 1.f;

		if (in == -32768)
			return -1.f;

		float c = in;
		return (c / (32767.0f));
	}

	static constexpr double rad(double deg)
	{
		return (deg / 360.0) * 6.283185307;
	}

	static constexpr float rad(float deg)
	{
		return (deg / 360.0f) * XM_2PI;
	}

	static inline XMFLOAT3 normalize_vector(const XMFLOAT3& _normal)
	{
		float len = length_vector(_normal);

		XMFLOAT3 result = _normal;

		result.x /= len;
		result.y /= len;
		result.z /= len;

		return result;
	}

	inline static float length_vector(XMFLOAT3 _normal)
	{
		return
			sqrt(
			_normal.x * _normal.x +
			_normal.y * _normal.y +
			_normal.z * _normal.z

			);
	}

	inline static float length_vector(XMFLOAT4 _normal)
	{
		return
			sqrt(
			_normal.x * _normal.x +

			_normal.y * _normal.y +

			_normal.z * _normal.z +

			_normal.w * _normal.w

			);
	}

	/*static XMFLOAT3 unpack_normal8(uint32_t data)
	{
		float x = (((data) & 0xFF) / 127.5f - 1.0f);
		float y = ((((data) >> 8) & 0xFF) / 127.5f - 1.0f);
		float z = ((((data) >> 16) & 0xFF) / 127.5f - 1.0f);

		return { x,y,z };
	}*/

	static XMFLOAT4 mul_quat(XMFLOAT4 q1, XMFLOAT4 q2)
	{
		XMVECTOR _q1 = XMLoadFloat4(&q1);
		XMVECTOR _q2 = XMLoadFloat4(&q2);

		XMVECTOR _q3 = XMQuaternionMultiply(_q1, _q2);

		XMFLOAT4 q3;
		XMStoreFloat4(&q3, _q3);

		return q3;
	}

	static uint8_t float_to_unorm8(float f)
	{
		float rf = (f + 1.0f) * 127.0f;
		uint8_t b = rf;

		return b;
	}

	inline static constexpr float unpack_weight(uint8_t b)
	{
		return static_cast<float>(b) / 255.0f;
	}

	static constexpr XMFLOAT3 byte4_to_snorm_float3(PackedVector::XMBYTE4 _vIn)
	{
		return
		{
			(static_cast<float>(_vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(_vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(_vIn.z) / 127.0f) - 1.f
		};
	}

	static constexpr XMFLOAT4 byte4_to_snorm_float4(PackedVector::XMBYTE4 _vIn)
	{
		return
		{
			(static_cast<float>(_vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(_vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(_vIn.z) / 127.0f) - 1.f,
			(static_cast<float>(_vIn.w) / 127.0f) - 1.f
		};
	}

	static constexpr float unorm8_to_float(uint8_t b)
	{
		float i = b;

		//float r1 =  ((i / 255.f) * 2.f) - 1.f;
		float r1 = (i / 127.0f) - 1.f;

		return r1;
	};

	/*static constexpr float unorm8_to_float(uint8_t b)
	{
		return ((float)b) / 127.5f - 1.f;
	};*/

	/*static constexpr uint8_t norm_float_to_byte(float f)
	{
		float ret = (f + 1 ) * 127.0f;
		return ret;
	};*/

	//thread_local static  XMFLOAT3 oFloat3Temp;;
	static inline XMFLOAT3& normalize3(const XMFLOAT3& _o)
	{
		static XMFLOAT3 oFloat3Temp;;
		// calc length
		float len = sqrt(_o.x * _o.x + _o.y * _o.y + _o.z * _o.z);

		// dive element by length
		oFloat3Temp.x = _o.x / len;
		oFloat3Temp.y = _o.y / len;
		oFloat3Temp.z = _o.z / len;

		return oFloat3Temp;
	}

	//thread_local static  XMFLOAT4 oFloat3Temp2;;
	static inline XMFLOAT4& normalize4_AsVec3(const XMFLOAT4& _o)
	{
		XMFLOAT4 oFloat3Temp2;
		// calc length
		float len = sqrt(_o.x * _o.x + _o.y * _o.y + _o.z * _o.z);

		// dive element by length
		oFloat3Temp2.x = _o.x / len;
		oFloat3Temp2.y = _o.y / len;
		oFloat3Temp2.z = _o.z / len;
		oFloat3Temp2.w = _o.w;

		return oFloat3Temp2;
	}

	//static inline uint8_t floatToSNorm8(float _f)
	//{
	//	float c = std::clamp(_f, -1.0f, 1.0f);

	//	c = c * (127 - 1);
	//
	//	if (c >= 0)
	//		c = c + 0.5f;
	//	else c = c - 0.5f;

	//	c = trunc(c);

	//	uint8_t ret = c;

	//	return ret;
	//
	//}

	//static constexpr inline float UNorm8ToFloat(uint8_t in)
	//{
	//	float ret = in;
	//	ret = (ret / 127.5f) - 1.f;
	//
	//	return ret;
	//}

	//static inline uint8_t floatToUNorm8(float _f)
	//{
	//	float c = std::clamp(_f, 0.0f, 1.0f);

	//	c = c * (127);
	//
	//	if (c >= 0)
	//		c = c + 0.5f;
	//	else c = c - 0.5f;

	//	c = trunc(c);

	//	uint8_t ret = c;

	//	return ret;
	//
	//}

	static inline void _release(ID3D11DeviceChild* poRef)
	{
		ULONG ref_count = 0;
		if (poRef != nullptr)
			ref_count = poRef->Release();

		if (ref_count == 0)
			poRef = nullptr;
	}

	static inline XMFLOAT4X4 mul_matrix4x4(XMFLOAT4X4& m1, XMFLOAT4X4& m2)
	{
	}
	//static constexpr float uint8_to_float1(uint8_t _in)
	//{
	//	return  ((((float) _in) / 255.0f) * 2.0f) - 1.0f;
	//}

	static constexpr bool isSRGB(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			return true;

		default:
			return false;
		}

		return false;
	}

	//static HRESULT getCompiledPixelShaderFromrResource(
	//	ID3D11Device* _poDevice,
	//	ID3D11PixelShader** _poDest,

	//	UINT res_id,
	//	SIZE_T* size = NULL, LPVOID* _poByteCode = NULL)

	//{
	//	HRESULT hr = S_OK;
	//	HRSRC hrsc = FindResource(H_INST, MAKEINTRESOURCE(res_id), RT_RCDATA);
	//	HGLOBAL hg = LoadResource(H_INST, hrsc);
	//	void* p = LockResource(hg);

	//	DWORD dwSourceSize = SizeofResource(
	//		H_INST,
	//		hrsc
	//	);

	//	hr = _poDevice->CreatePixelShader(
	//		p,
	//		dwSourceSize,
	//		nullptr,

	//		_poDest
	//	);

	//	assert(SUCCEEDED(hr));

	//	if (size)
	//		*size = dwSourceSize;

	//	if (_poByteCode)
	//		*_poByteCode = p;

	//	return hr;
	//};

	//static HRESULT getCompiledVertexShaderFromrResource(
	//	ID3D11Device* _poDevice,
	//	ID3D11VertexShader** _poDest,
	//	UINT res_id,
	//	SIZE_T* size = NULL, LPVOID* _poByteCode = NULL)

	//{
	//	HRESULT hr = S_OK;
	//	HRSRC hrsc = FindResource(H_INST, MAKEINTRESOURCE(res_id), RT_RCDATA);
	//	HGLOBAL hg = LoadResource(H_INST, hrsc);
	//	void* p = LockResource(hg);

	//	DWORD dwSourceSize = SizeofResource(
	//		H_INST,
	//		hrsc
	//	);

	//	hr = _poDevice->CreateVertexShader(
	//		p,
	//		dwSourceSize,
	//		nullptr,
	//		_poDest
	//	);

	//	assert(SUCCEEDED(hr));

	//	if (size)
	//		*size = dwSourceSize;

	//	if (_poByteCode)
	//		*_poByteCode = p;

	//	return hr;
	//};

	//static void box_blur(DirectX::XMFLOAT4* _pRGBA, uint32_t width, uint32_t height)
	//{
	//	//XMBYT* pNew = new XMFLOAT4 [width * height];
	//	auto image = [_pRGBA, width, height](int x, int y) -> XMFLOAT4
	//	{
	//		return _pRGBA[y * width + x];
	//	};

	//	auto new_image = [_pRGBA, width, height](int x, int y) -> XMFLOAT4
	//	{
	//		return _pRGBA[y * width + x];
	//	};

	//	for (size_t y = 0; y < height; y++)
	//		for (size_t x = 0; x < width; x++)
	//		{
	//			// Kernel would not fit!
	//			if (x < 1 || y < 1 || x + 1 >= width || y + 1 >= height)
	//				continue;
	//			// Set P to the average of 9 pixels:
	//			//		X X X
	//			//		X P X
	//			//		X X X
	//				// Calculate average.
	//			XMFLOAT3 sum = { 0,0,0 };

	//			sum.x = image(x - 1, y + 1).x; // Top left
	//			sum.y = image(x - 1, y + 1).y; // Top left
	//			sum.z = image(x - 1, y + 1).z; // Top left

	//			sum.x += image(x + 0, y + 1).x; // Top center
	//			sum.y += image(x + 0, y + 1).y; // Top center
	//			sum.z += image(x + 0, y + 1).z; // Top center

	//			sum.x += image(x + 1, y + 1).x; // Top right
	//			sum.y += image(x + 1, y + 1).y; // Top right
	//			sum.z += image(x + 1, y + 1).z; // Top right

	//			sum.x += image(x - 1, y + 0).x; // Mid left
	//			sum.y += image(x - 1, y + 0).y; // Mid left
	//			sum.z += image(x - 1, y + 0).z; // Mid left

	//			sum.x += image(x + 0, y + 0).x;  // Current pixel
	//			sum.y += image(x + 0, y + 0).y;  // Current pixel
	//			sum.z += image(x + 0, y + 0).z;  // Current pixel

	//			sum.x += image(x + 1, y + 0).x;  // Mid right
	//			sum.y += image(x + 1, y + 0).y;  // Mid right
	//			sum.z += image(x + 1, y + 0).z;  // Mid right

	//			sum.x += image(x - 1, y - 1).x;  // Low left
	//			sum.y += image(x - 1, y - 1).y;  // Low left
	//			sum.z += image(x - 1, y - 1).z;  // Low left

	//			sum.x += image(x + 0, y - 1).x;  // Low center
	//			sum.y += image(x + 0, y - 1).y;  // Low center
	//			sum.z += image(x + 0, y - 1).z;  // Low center

	//			sum.x += image(x + 1, y - 1).x;  /// Low right
	//			sum.y += image(x + 1, y - 1).y;  /// Low right
	//			sum.z += image(x + 1, y - 1).z;  /// Low right

	//			//image[x, y] = Sum / 9;
	//		}
	//	//Return newImage;
	//}

	//static void gaussian_blur(uint32_t iHeight, uint32_t iWidth)
	//{
	//	// gaussian filter kernel
	//	float gauss[3][3] = { {1, 2, 1},
	//						  {2, 4, 2},
	//	                      {1, 2, 1} };

	//	int convHeight;     // heighLPARAM lParam of convolution filter, gaussian in this case
	//	int convWidth;      // width value of convolution filter, gaussian in this case

	//	//////gaussian blur/////////
	//	for (int i = 0; i < iHeight; i++) {
	//		for (int j = 0; j < iWidth; j++) {
	//			gaussPixel = 0;
	//			for (int x = 0; x < convHeight; x++) {
	//				for (int y = 0; y < convWidth; y++) {
	//					gaussPixel += OldImage[x - convWidth / 2 + i, y - convHeight / 2 + j] * gauss[i, j];
	//				}
	//			}
	//			NewImage[x, y] = gaussPixel;
	//		}
	//	}

	//}
};

//thread_local XMFLOAT3 lib3d::oFloat3Temp;
//thread_local XMFLOAT4 lib3d::oFloat3Temp2;