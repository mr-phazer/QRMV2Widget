#pragma once

// stl
#include <memory>
#include <fstream>

// direct3d 11 includes

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

// author includes
#include "..\SystemLib\return_value.h"
#include "..\SystemLib\tools.h"
#include "..\DirectXTex\DDSTextureLoader\DDSTextureLoader11.h"
#include "..\RenderLib\vertex_layouts.h"

using namespace DirectX;
namespace res_tools
{
	template <class TYPE>
	std::shared_ptr<TYPE> make_shared_resource(TYPE* _poRaw)
	{
		std::shared_ptr<TYPE> spoShared = std::shared_ptr<TYPE>(_poRaw,
			[](TYPE* _poRes)
			{
				_poRes->Release();
			}
		);

		return spoShared;
	};

	extern SystemLib::return_value loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath, ID3D11PixelShader** ppDest);
	extern SystemLib::return_value loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::wstring& _strPath, ID3D11PixelShader** ppDest);

	//std::pair< ID3D11PixelShader*, std::shared_ptr<uint8_t>>
	//	loadPixelShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath)
	//{
	//	size_t pixel_shader_code_size = tools::getFileSize(_strPath);

	//	auto spPixelShaderCodeRaw = std::shared_ptr<uint8_t>(new uint8_t[pixel_shader_code_size],
	//		[](uint8_t* p)
	//		{
	//			delete p;
	//		}
	//	);

	//	std::pair< ID3D11PixelShader*, std::shared_ptr<uint8_t>> tempPair;
	//	std::ifstream in(_strPath, std::ios::binary);
	//	in.read((char*)tempPair.second.get(), pixel_shader_code_size);
	//	in.close();

	//	HRESULT hr = _poDevice->CreatePixelShader(
	//		tempPair.second.get(),
	//		pixel_shader_code_size,
	//		nullptr,
	//		&tempPair.first
	//	);

	//	assert(SUCCEEDED(hr));

	//	return tempPair;
	//}

	extern SystemLib::return_value createInputLayout2(
		ID3D11Device* _poDevice,
		ID3D11InputLayout** ppInputLayout,
		const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
		UINT layou_elelemnts,
		vector<uint8_t> _vecCodeRaw);

	extern 	std::pair<ID3D11VertexShader*, vector<uint8_t>> loadVertexShaderFromDisk2(ID3D11Device* _poDevice, const std::string& _strPath);

	extern 	std::pair<ID3D11VertexShader*, vector<uint8_t>> loadVertexShaderFromDisk(ID3D11Device* _poDevice, const std::wstring& _strPath);

	//return_value loadVSFromDiskAndCreateInputLayout(
	//	ID3D11Device* _poDevice,
	//	const std::string& _strPath,
	//	ID3D11PixelShader** ppDestVS,
	//	ID3D11InputLayout** ppDestInputLayout)
	//{
	//	return S_OK;
	//};

	static bool loadCubeMapTextureFromResource(ID3D11Device* _poDevice, int _resId1,
		ID3D11Resource** _poTexture,
		ID3D11ShaderResourceView** textureView)
	{
		HRSRC hrsc = NULL;;
		HGLOBAL hg = NULL;;
		uint8_t* p = NULL;;
		DWORD dwSourceSize = 0;;

		auto H_INST = GetModuleHandle(NULL);

		hrsc = FindResource(H_INST, MAKEINTRESOURCE(_resId1), RT_RCDATA);
		if (!hrsc) return false;

		hg = LoadResource(H_INST, hrsc);
		if (!hg) return false;

		p = (uint8_t*)LockResource(hg);
		if (!p) return false;

		dwSourceSize = SizeofResource(
			H_INST,
			hrsc);

		HRESULT hr = DirectX::CreateDDSTextureFromMemoryEx(_poDevice, p, dwSourceSize, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE,
			true, _poTexture, textureView);

		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	};

	SystemLib::return_value createVertexBuffer(ID3D11Device* _poDevice, ID3D11Buffer** ppoVertexBuffer, const void* _pvData, UINT _data_size,

		D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC,
		UINT        BindFlags = D3D11_BIND_VERTEX_BUFFER,
		UINT        CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
	);
};
