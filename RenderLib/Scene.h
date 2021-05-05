#pragma once

#include <stdexcept>
#include <vector>
#include <map>
#include <memory>

#include <d3d11.h>
#include <wrl\client.h>
#include "Camera.h"

#include "..\QtRME_GUI\render_resources.h"
#include "..\RenderLib\render_const_bufers.h"
#include "..\RenderLib\RenderTarget.h"

#include "ResourceManager.h"
#include "Shader.h"

struct SwapChainData
{
	/*_GFXSwapChainData(
		IDXGISwapChain1* _pSwapChain1,
		ID3D11Texture2D* m_pBackBuffer,
		ID3D11RenderTargetView* m_pRenderTargetView
	)*/
	IDXGISwapChain1* m_pSwapChain1 = nullptr;
	ID3D11Texture2D* m_pBackBuffer = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
};

namespace RenderLib
{
	class BaseNode;
	class IDrawableNode;
	class QDirect3D11Widget;
	class RendererQueue;
	class SSAO_RenderTechnique;

	class Scene
	{
	private:
		Scene();
	public:
		std::string m_strName = "";

		void pause() { m_bRenderingRunning = false; };
		void resume() { m_bRenderingRunning = true; };

		void updateVSPerModelConstBuffer();

		void update_PS_PerFrameConstBuffer();
		void update_PS_PerMeshConstBuffer();

		void update_PS_PerMeshConstBuffer(const MaterialData&);

		void updatePSLightConstBuffer();

		VS_PerModel_ConstantBuffer m_VS_PerModel_ConstantBuffer;
		VS_PerFrame_ConstantBuffer m_VS_PerFrame_ConstantBuffer;

		PS_PerFrame_ConstBuffer m_PS_PerFrame_ConstantBuffer;
		PS_PerMesh_ConstBuffer m_PS_PerMesh_ConstantBuffer;

		PS_PerFrameLightBuffer m_oPS_LightBuffer;

		ResourceManager oResourceManger;

		void setDefaultPixelShader(ID3D11PixelShader* _poPS) { m_poDefaultPixelShader = _poPS; }
		ID3D11PixelShader* getDefaultPixelShader() const { return m_poDefaultPixelShader; };

	public:

		static Scene* create(ID3D11Device*, ID3D11DeviceContext*, const std::string& _strName, Resources::IResourceContainer* _pRes);
		void setSwapChainPtr(SwapChainData* _poSwapChainData) { m_poSwapChainData = _poSwapChainData; };

		ID3D11Device* getDevice() const;
		ID3D11DeviceContext* getDeviceContext() const;

		void render(ID3D11DeviceContext*);

		std::shared_ptr<RenderLib::SSAO_RenderTechnique> spoRendererTechnique;

		Resources::IResourceContainer* getResources();
		RenderLib::BaseNode* getRootNode();
		Camera& getCamera();

		void addObject(RenderLib::BaseNode*);

		void _setPixelShaderResource(size_t _index, ID3D11PixelShader* _Resource);
		ID3D11PixelShader* getPixelShaderResource(size_t _index);

		void _setVertexShaderResource(size_t _index, ID3D11VertexShader* _Resource);
		ID3D11VertexShader* getVertexShaderResource(size_t _index);

	public:
		RenderLib::RenderTarget2D oSSAORenderTarget;

		ID3D11RenderTargetView* m_poRenderTargetView = NULL;

		shared_ptr<Shader::Shader> poSSAOSHader;

	private:
		Camera m_oCamera;

		SwapChainData* m_poSwapChainData = nullptr;

		std::shared_ptr <Resources::IResourceContainer> m_spoResources;
		std::shared_ptr<BaseNode> m_spoRootNode;

		ID3D11Device* m_poDevice = nullptr;
		ID3D11DeviceContext* m_poDeviceContext = nullptr;

		std::vector<std::shared_ptr<BaseNode>> vecObjects;

		std::map<size_t, ID3D11VertexShader*> m_mapVertexShaders;
		std::map<size_t, ID3D11PixelShader*> m_mapPixelShaders;

		// per frame
		ID3D11Buffer* m_poVertexShaderPerModelConstBuffer = nullptr;
		ID3D11Buffer* m_poVSPerFrameConstBuffer = nullptr;

		// per model
		ID3D11Buffer* m_poPixelShaderPerFrameConstBuffer = nullptr;
		ID3D11Buffer* m_poPixelShaderPerMeshConstBuffer = nullptr;

		ID3D11Buffer* m_poLightConstBufferr = nullptr;

		ID3D11PixelShader* m_poDefaultPixelShader = nullptr;

		bool m_bRenderingRunning = true;
	private:
		bool createConstBuffers();
		bool createLightBuffer();

		//public:
		//	DXGI_SAMPLE_DESC m_oSwapChainSampleDesc = { 8, 0 };

		//	D3D11_VIEWPORT m_oViewPort;
		//	ID3D11Device* m_pDevice = nullptr;
		//	ID3D11DeviceContext* m_pDeviceContext = nullptr;
		//	IDXGISwapChain* m_pSwapChain = nullptr;

		//	ID3D11RasterizerState* m_poRasterizerState_Solid = nullptr;
		//	ID3D11RasterizerState* m_poRasterizerState_WireFrame = nullptr;

		//	SwapChainData m_oSwapChainData;

		//	ID3D11Texture2D* m_poDepthStencil = nullptr;
		//	ID3D11DepthStencilView* m_poDepthStencilView = nullptr;;

		//	ID3D11DepthStencilState* m_poDepthStencilState_On = nullptr;
		//	ID3D11DepthStencilState* m_poDepthStencilState_Off = nullptr;

		//	// IBL textures
		//	ID3D11ShaderResourceView* m_poSRV_CubeMap_Diffuse = nullptr;
		//	ID3D11ShaderResourceView* m_poSRV_CubeMap_Specular = nullptr;
		//	ID3D11ShaderResourceView* m_poSRV_LookUpTexture = nullptr;
	};
	extern shared_ptr<Scene> g_spoScene;
}