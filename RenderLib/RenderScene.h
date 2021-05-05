/*
 *
 */
#pragma once

#include <stdexcept>

#include <d3d11.h>
#include <D3Dcompiler.h>

 //#include <moc_QDirect3D11Widget.cpp>

#include "..\RenderLib\Scene.h"

#include "..\DirectXTK\Inc\SpriteFont.h"

class RenderScene
{
	bool getFrameBuffer();

	RenderScene(HWND, bool);

	HRESULT creatSwapChain(HWND _hwnd, IDXGISwapChain1** ppDest);

	virtual ~RenderScene();

	//bool createScene(Resources::IResourceContainer* _poResources, const string& _strName = "");

	void release();
	void resetEnvironment();
	void updateViewPort();
	void setSamplers();

	HRESULT createSwapChain1(HWND _hwnd, IDXGISwapChain1** ppDest); // , ID3D11Texture2D* _pBackBuffer, ID3D11RenderTargetView* _pRenderTargetView);
	void run();

	void setRenderText(const string& _str) { m_strStringToRender = _str; };

	void pauseRendering();
	void resumeRendering();

	void pauseLogic()
	{
		m_bLogicRunning = false;
	}

	void resumeLogic()
	{
		m_bLogicRunning = true;
	}

private:
	bool initController();
	bool init1();

	HRESULT createDevice2();

	HRESULT createDeviceAndSwapChain(HWND _hwnd, IDXGISwapChain** ppDest);

	bool loadIBLMapsFromResource(UINT _irr, UINT _rad, UINT _lut);
	void setEnvMaps();
	bool createDeptBufferAndhStencil(unsigned int _width, unsigned int _height);
	bool createSamplerStates();
	bool createRasterizers();

	void beginScene();

	void endScene();
	void endScene1();

	void tick();
	void render();

public:
	RenderLib::Scene* scene() { return m_poScene; };

	HWND const& nativeHandle() const { return m_hWnd; }

	ID3D11Device* device() const { return m_pDevice; }
	ID3D11DeviceContext* deviceContext() { return m_pDeviceContext; }
	//IDXGISwapChain* swapChain() { return m_pSwapChain; }
	IDXGISwapChain* swapChain() { return ((m_bExtendedeSwapChain) ? m_oSwapChainData.m_pSwapChain1 : m_pSwapChain); }

	ID3D11RenderTargetView* TargetView() const { return m_oSwapChainData.m_pRenderTargetView; }

	bool renderActive() const { return m_bRenderActive; }
	void setRenderActive(bool active) { m_bRenderActive = active; }

	D3D11_VIEWPORT* viewPort() { return &m_oViewPort; }
	ID3D11Texture2D* depthStencil() { return m_poDepthStencil; };
	ID3D11DepthStencilView* depthStencilView() { return m_poDepthStencilView; };

	ID3D11Texture2D* m_poDepthStencil = nullptr;
	ID3D11DepthStencilView* m_poDepthStencilView = nullptr;;

	ID3D11DepthStencilState* m_poDepthStencilState_On = nullptr;
	ID3D11DepthStencilState* m_poDepthStencilState_Off = nullptr;

	// IBL textures
	ID3D11ShaderResourceView* m_poSRV_CubeMap_Diffuse = nullptr;
	ID3D11ShaderResourceView* m_poSRV_CubeMap_Specular = nullptr;
	ID3D11ShaderResourceView* m_poSRV_LookUpTexture = nullptr;

	D3DCOLORVALUE* BackColor() { return &m_BackColor; }

	bool ctrlKeyuDown() { return m_bControlDown; };

private:
	DXGI_SAMPLE_DESC m_oSwapChainSampleDesc = { 8, 0 };

	D3D11_VIEWPORT m_oViewPort;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;

	ID3D11RasterizerState* m_poRasterizerState_Solid = nullptr;
	ID3D11RasterizerState* m_poRasterizerState_WireFrame = nullptr;

	SwapChainData m_oSwapChainData;

	// scene
	RenderLib::Scene* m_poScene = nullptr;

	HWND m_hWnd;
	bool m_bDeviceInitialized;

	bool m_bRenderActive;
	bool m_bLogicRunning = true; // is the game logic running, = false = keeps draw but logic is an pause

	bool m_bExtendedeSwapChain = true;

	bool m_bStarted;

	bool m_bControlDown = false;
	bool m_bAltDown = false;
	bool m_bShiftDown = false;

	D3DCOLORVALUE m_BackColor;

	ID3D11SamplerState* m_poSampleStateDefault = nullptr;
	ID3D11SamplerState* m_poSampleStateBRDF = nullptr;
	ID3D11SamplerState* m_poSampleStateNormal = nullptr;
	ID3D11SamplerState* m_poSampleStateMask = nullptr;
	ID3D11SamplerState* m_poSampleStateNormalMap = nullptr;
	ID3D11SamplerState* m_poSampleStatePoint = nullptr;
	ID3D11SamplerState* m_poSampleStateLinear = nullptr;

	// DIrectXTK font stuff
private:
	std::unique_ptr<DirectX::SpriteFont> m_font;
	DirectX::SimpleMath::Vector2 m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	string m_strStringToRender = "";
};

// ############################################################################
// ############################## Utils #######################################
// ############################################################################
#define ReleaseObject(object)                                                                 \
    if ((object) != Q_NULLPTR)                                                                \
    {                                                                                         \
        object->Release();                                                                    \
        object = Q_NULLPTR;                                                                   \
    }
#define ReleaseHandle(object)                                                                 \
    if ((object) != Q_NULLPTR)                                                                \
    {                                                                                         \
        CloseHandle(object);                                                                  \
        object = Q_NULLPTR;                                                                   \
    }

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr)
		: std::runtime_error(HrToString(hr))
		, m_hr(hr)
	{
	}
	HRESULT Error() const { return m_hr; }

private:
	const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr)) { throw HrException(hr); }
}

#define DXCall(func) ThrowIfFailed(func)
