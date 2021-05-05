#pragma once

#include "BaseNode.h"
#include "..\SystemLib\tools.h"

#include "half.h"
#include "vertex_layouts.h"

#include <string>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

#include "..\SystemLib\ISerializeabe.h"

#pragma comment( lib, "user32" )          // link against the win32 library
#pragma comment( lib, "d3d11.lib" )       // direct3D library
#pragma comment( lib, "dxgi.lib" )        // directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler
//#pragma comment( lib, "D3D11SDKLAYERS.lib" ) // shader compiler

typedef  half_float::half half;

struct float16_4
{
	half x = half(0);
	half y = half(0);
	half z = half(0);
	half w = half(0);
};

struct float16_2
{
	half x = half(0);
	half y = half(0);
};

struct xyzw8
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
	uint8_t w;
};

namespace VertexFormats
{
	struct cinematic
	{
		float16_4 position;
		float16_2 uv;
		xyzw8 normal;
		float16_2 uv2;
		xyzw8 tangent;
		xyzw8 bitangent;
	};
}
struct unorm8_4
{
	union {
		struct {
			uint8_t x;
			uint8_t y;
			uint8_t z;
			uint8_t w;
		};

		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
	};
};

namespace RenderLib
{
	class Scene;

	class IDrawableNode :
		public BaseNode //, public SubMeshData
	{
		friend class Scene;

	protected:
		IDrawableNode()
		{
		};
		IDrawableNode(const string& _strName) : BaseNode(_strName) {};
		virtual bool init(Scene* _poScene);

	public:
		static IDrawableNode* create(Scene* _poScene, const string& _strName);

		/*SystemLib::return_value createVertexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size,

			D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC,
			UINT        BindFlags = D3D11_BIND_VERTEX_BUFFER,
			UINT        CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
		);
*/

//SystemLib::return_value createVertexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size, ID3D11Buffer** _ppBuffer);

//SystemLib::return_value createIndexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size);

//SystemLib::return_value createInputLayou(ID3D11Device* _poDevice, const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc, UINT _size);

//SystemLib::return_value loadVertexShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath);
//SystemLib::return_value loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath);

	private:
		virtual void allocateMeshDataContainer();

	protected:
		//std::shared_ptr<uint8_t> spPixelShaderCodeRaw = nullptr;
		//std::shared_ptr<uint8_t> spVerterShaderCodeRaw = nullptr;

		size_t vertex_shader_code_size = 0;
		size_t pixel_shader_code_size = 0;

		ID3D10Blob* poVertexShaderCode = nullptr;
		ID3D10Blob* poPixelShaderCode = nullptr;

	public:
		DirectX::XMFLOAT3 vBBMin = { 0,0,0 };
		DirectX::XMFLOAT3 vBBMax = { 0,0,0 };

		//ID3D11InputLayout* poInputLayout = nullptr;

		/*ID3D11VertexShader* poVertexShader = nullptr;
		ID3D11PixelShader* poPixelShader = nullptr;

		ID3D11Buffer* poVertexBuffer = nullptr;
		ID3D11Buffer* poIndexBuffer = nullptr;;*/

		//UINT uiIndexCount = 0;
	};
}