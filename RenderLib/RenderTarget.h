#pragma once
//-------------------------------------------------------------------------------

/// RenderTarget.h

// Direct3d 11 Headers
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"
#include <wrl\client.h>

// author headers
//#include "Scene.h"

#include "MeshData.h"

namespace RenderLib
{
	class Scene;

	class RenderTarget2D
	{
		//RenderTarget2D() {};
	public:
		//static std::shared_ptr<RenderTarget2D> create(Scene*);

		ID3D11Texture2D* getTexture() { return m_cpoTexture.Get(); };
		ID3D11RenderTargetView* getRenderTargetView() { return m_cpoRenderTargetView.Get(); };
		ID3D11ShaderResourceView* getShaderResourceView() { return m_cpoShaderResourceView.Get(); };

		//private:
		bool init(Scene* _poScene, UINT width, UINT height);

		bool init(
			Scene* _poScene,
			UINT Width,
			UINT Height,
			DXGI_FORMAT Format,
			DXGI_SAMPLE_DESC SampleDesc = { 0, 1 },
			D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
			UINT BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE

		);

		//private:

		HRESULT init_impl(
			Scene* _poScene,
			// D3D11_TEXTURE2D_DESC content
			UINT Width,
			UINT Height,
			UINT MipLevels,
			UINT ArraySize,
			DXGI_FORMAT Format,
			DXGI_SAMPLE_DESC SampleDesc,
			D3D11_USAGE Usage,
			UINT BindFlags,
			UINT CPUAccessFlags,
			UINT MiscFlags
		);

		bool generateQuad(Scene* _poScene);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cpoTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_cpoRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cpoShaderResourceView;

		SubMeshData oQuad;
	};
}