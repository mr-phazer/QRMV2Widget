/*
 *
 */
#pragma comment(lib, "d3d11.lib")
#pragma comment( lib, "dxguid.lib")

#include "..\DirectXTK\Inc\ScreenGrab.h"

#include "RenderScene.h"
#include "..\QtRME_GUI\resource.h"

#include "..\RenderLib\lib3d.h"
#include "..\RenderLib\res_tools.h"
#include "..\RenderLib\IDrawableNode.h"

constexpr int FPS_LIMIT = 60.0f;
constexpr int MS_PER_FRAME = (int)((1.0f / FPS_LIMIT) * 1000.0f);

RenderScene::RenderScene(HWND _hwnd, bool _bExtendedSwapChain)
	: QWidget(parent)
	, m_pDevice(Q_NULLPTR)
	, m_pDeviceContext(Q_NULLPTR)
	, m_pSwapChain(Q_NULLPTR)
	, m_oSwapChainData({ 0 })
	, m_hWnd(_hwnd)
	, m_bDeviceInitialized(false)
	, m_bRenderActive(false)
	, m_bStarted(false)
	, m_BackColor{ 0.0f, 0.135f, 0.481f, 1.0f }
	, m_bExtendedeSwapChain(_bExtendedSwapChain)
{
	m_bDeviceInitialized = initController();

	run();
}

HRESULT RenderScene::creatSwapChain(HWND _hwnd, IDXGISwapChain1** ppDest)
{
	return E_NOTIMPL;
}

RenderScene::~RenderScene()
{
	int i = 1;
}

//bool RenderScene::createScene(Resources::IResourceContainer* _poResources, const string& _strName)
//{
//	m_poScene = RenderLib::Scene::create(m_pDevice, m_pDeviceContext, _strName, _poResources);
//
//	m_poScene->setSwapChainPtr(&m_oSwapChainData);
//	return (nullptr != m_poScene);
//}

HRESULT RenderScene::createDevice2()
{
	_log_action("Attempting to create Device");
	// create device
	D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};
#ifdef _DEBUG
	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		NULL,
		&m_pDeviceContext);

	assert(SUCCEEDED(hr));

	///_assert(SUCCEEDED(hr));
#else
	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_poDevice,
		NULL,
		&m_poContext);
	assert(SUCCEEDED(hr));;
#endif
	_log_action("Device created. Sucess");

	return hr;
}

void RenderScene::release()
{
	m_bDeviceInitialized = false;

	ReleaseObject(m_oSwapChainData.m_pRenderTargetView);

	if (m_bExtendedeSwapChain)
	{
		ReleaseObject(m_oSwapChainData.m_pSwapChain1);
	}
	else
	{
		ReleaseObject(m_pSwapChain);
	}

	ReleaseObject(m_poSampleStateDefault);
	ReleaseObject(m_poSampleStateBRDF);
	ReleaseObject(m_poSampleStateLinear);
	ReleaseObject(m_poSampleStateMask);
	ReleaseObject(m_poSampleStatePoint);
	ReleaseObject(m_poSampleStateNormal);

	ReleaseObject(m_poSRV_CubeMap_Diffuse);
	ReleaseObject(m_poSRV_CubeMap_Specular);

	ReleaseObject(m_pDeviceContext);
	ReleaseObject(m_pDevice);
}

void RenderScene::run()
{
	//m_qTimer.start(MS_PER_FRAME);
	m_bRenderActive = m_bStarted = true;
}

void RenderScene::pauseRendering()
{/*
	if (!m_qTimer.isActive() || !m_bStarted) return;

	if (m_bExtendedeSwapChain)
		disconnect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame1);
	else
		disconnect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);

	m_qTimer.stop();*/
	m_bRenderActive = false;
}

void RenderScene::resumeRendering()
{
	/*if (m_qTimer.isActive() || !m_bStarted) return;

	if (m_bExtendedeSwapChain)
		connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame1);
	else
		connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);

	m_qTimer.start(MS_PER_FRAME);*/
	m_bRenderActive = true;
}

//void RenderScene::showEvent(QShowEvent* event)
//{
//	if (false)
//		if (!m_bDeviceInitialized)
//		{
//			m_bDeviceInitialized = initController();
//			run();
//			//emit deviceInitialized(m_bDeviceInitialized);
//		}
//
//	QWidget::showEvent(event);
//}
//
//void RenderScene::keyPressEvent(QKeyEvent* event)
//{
//	if (event->key() == Qt::Key::Key_Control)
//	{
//		m_bControlDown = true;
//	}
//}
//
//void RenderScene::keyReleaseEvent(QKeyEvent* event)
//{
//	if (event->key() == Qt::Key::Key_Control)
//	{
//		m_bControlDown = false;
//	}
//}

bool RenderScene::initController()
{
	if (m_bExtendedeSwapChain)
	{
		return init1();
	}

	DXCall(
		createDeviceAndSwapChain(m_hWnd, &m_pSwapChain)
	);

	//m_pSwapChain->SetFullscreenState(true, nullptr);

	resetEnvironment();
	createDeptBufferAndhStencil(, );
	createSamplerStates();
	loadIBLMapsFromResource(IDR_DDS_IRR, IDR_DDS_RAD, IDR_DDS_LUT);

	//emit deviceInitialized(true);

	//connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);
	//pauseFrames();

	return true;
}

bool RenderScene::loadIBLMapsFromResource(UINT _irr, UINT _rad, UINT _lut)
{
	bool b1 = res_tools::loadCubeMapTextureFromResource(m_pDevice, _irr, NULL, &m_poSRV_CubeMap_Diffuse);
	bool b2 = res_tools::loadCubeMapTextureFromResource(m_pDevice, _rad, NULL, &m_poSRV_CubeMap_Specular);
	bool b3 = res_tools::loadCubeMapTextureFromResource(m_pDevice, _lut, NULL, &m_poSRV_LookUpTexture);

	return b1 && b2 && b3;
}

void RenderScene::setEnvMaps()
{
	ID3D11ShaderResourceView* ppShaderResourceViews[3];

	ppShaderResourceViews[0] = m_poSRV_CubeMap_Diffuse;
	ppShaderResourceViews[1] = m_poSRV_CubeMap_Specular;
	ppShaderResourceViews[2] = m_poSRV_LookUpTexture;

	m_pDeviceContext->PSSetShaderResources(
		0,
		3,
		ppShaderResourceViews
	);
}

bool RenderScene::createDeptBufferAndhStencil(unsigned int _width, unsigned int _height)
{
	// release interface it allocated
	if (m_poDepthStencil != nullptr)
		m_poDepthStencil->Release();

	if (m_poDepthStencilView != nullptr)
		m_poDepthStencilView->Release();

	if (m_poDepthStencilState_On != nullptr)
		m_poDepthStencilState_On->Release();

	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = width();
	depthTextureDesc.Height = height();
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;

	depthTextureDesc.SampleDesc = m_oSwapChainSampleDesc;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* DepthStencilTexture;
	hr = m_pDevice->CreateTexture2D(&depthTextureDesc, NULL, &m_poDepthStencil);

	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	::ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthTextureDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = m_pDevice->CreateDepthStencilView(m_poDepthStencil, &dsvDesc, &m_poDepthStencilView);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0x00;
	dsDesc.StencilWriteMask = 0x00;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_On);

	dsDesc.DepthEnable = false;
	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_Off);

	assert(SUCCEEDED(hr));

	m_pDeviceContext->OMSetDepthStencilState(m_poDepthStencilState_On, 1);

	return SUCCEEDED(hr);
}

bool RenderScene::createSamplerStates()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStateDefault);
	assert(SUCCEEDED(hr));

	//////////////////////////////////////////////////////////////////////////////////////////
	// Texture Sampler for BRDF look up texture
	//////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	//samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStateBRDF);
	assert(SUCCEEDED(hr));

	//////////////////////////////////////////////////////////////////////////////////////////

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	//samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStateNormal);
	assert(SUCCEEDED(hr));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStateMask);
	assert(SUCCEEDED(hr));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStatePoint);
	assert(SUCCEEDED(hr));

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_poSampleStateLinear);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}
//void RenderScene::onFrame1()
//{
//	//if (m_bLogicRunning) tick();
//
//	//beginScene();
//	//render();
//
//	//endScene1();
//}
//
//void RenderScene::onFrame()
//{
//	if (m_bLogicRunning) tick();
//
//	beginScene();
//	render();
//	endScene();
//}

void RenderScene::beginScene()
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_oSwapChainData.m_pRenderTargetView, NULL);
	m_pDeviceContext->ClearRenderTargetView(m_oSwapChainData.m_pRenderTargetView,
		reinterpret_cast<const float*>(&m_BackColor));
}

void RenderScene::endScene()
{
	//	if (FAILED(m_pSwapChain->Present(1, 0))) { onReset(); }
}

void RenderScene::tick()
{
	// TODO: Update your scene here. For aesthetics reasons, only do it here if it's an
	// important component, otherwise do it in the MainWindow.
	// m_pCamera->Tick();

	//emit ticked();
}

void RenderScene::render()
{
	// TODO: Present your scene here. For aesthetics reasons, only do it here if it's an
	// important component, otherwise do it in the MainWindow.
	// m_pCamera->Apply();

	if (!scene())
		return;

	scene()->getCamera().calculateMatrices(width(), height());
	setSamplers();

	auto pTarget = TargetView();
	float color[4] = { 0, 0, 0, 1 };
	deviceContext()->OMSetRenderTargets(1, &pTarget, depthStencilView());
	deviceContext()->ClearRenderTargetView(pTarget, color);

	// clear z buffer (and stenicl buffer)
	deviceContext()->ClearDepthStencilView(
		depthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// load cube maps for env ambient PBR lighting
// TODO: make it so there can more than one of these
	setEnvMaps();
	m_poScene->render(deviceContext());

	deviceContext()->OMSetDepthStencilState(m_poDepthStencilState_Off, 1);

	m_fontPos.x = 0; //width() / 2.f;
	m_fontPos.y = 0; // height() / 2.f;

	m_spriteBatch->Begin();

	const wchar_t* output = L"Hello World";

	DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_font->MeasureString(output) / 2.f;

	DirectX::SimpleMath::Vector3 vScale = { 0.6, 0.6, 0.6 };
	DirectX::SimpleMath::Color vColor = { 0.5, 0.5, 0.5, 1 };

	m_font->DrawString(m_spriteBatch.get(), m_strStringToRender.c_str(),
		m_fontPos, vColor, 0.f, origin);

	vScale = { 0.5, 0.5, 0.5 };
	vColor = { 1, 1, 1, 1 };

	m_font->DrawString(m_spriteBatch.get(), m_strStringToRender.c_str(),
		m_fontPos, vColor, 0.f, origin);

	m_spriteBatch->End();

	deviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);

	// TODO: shall this remain disabled
	/* is there is any reason to render stuff from the main window at all? */

	//emit rendered(this);
}

void RenderScene::closeEvent(QCloseEvent* event)
{
	event->ignore();
	emit detachedWindowClose();
}

void RenderScene::onReset()
{
	ID3D11Texture2D* pBackBuffer = Q_NULLPTR;
	ReleaseObject(m_oSwapChainData.m_pRenderTargetView);
	DXCall(m_pSwapChain->ResizeBuffers(0, width(), height(), DXGI_FORMAT_UNKNOWN, 0));
	DXCall(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	DXCall(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_oSwapChainData.m_pRenderTargetView));
	ReleaseObject(pBackBuffer);

	updateViewPort();
	createDeptBufferAndhStencil();
}
void RenderScene::onReset1()
{
	ID3D11Texture2D* pBackBuffer = Q_NULLPTR;
	ReleaseObject(m_oSwapChainData.m_pRenderTargetView);
	DXCall(m_oSwapChainData.m_pSwapChain1->ResizeBuffers(0, width(), height(), DXGI_FORMAT_UNKNOWN, 0));
	DXCall(m_oSwapChainData.m_pSwapChain1->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	DXCall(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_oSwapChainData.m_pRenderTargetView));
	ReleaseObject(pBackBuffer);

	updateViewPort();
	createDeptBufferAndhStencil();
}

void RenderScene::resetEnvironment()
{
	// TODO: Add your own custom default environment, i.e:
	// m_pCamera->resetCamera();

	if (m_bExtendedeSwapChain)
		onReset1();
	else
		onReset();

	if (!m_bRenderActive) tick();
}

void RenderScene::updateViewPort()
{
	//// Set viewport
	m_oViewPort.TopLeftX = 0;
	m_oViewPort.TopLeftY = 0;
	m_oViewPort.Width = (FLOAT)width();
	m_oViewPort.Height = (FLOAT)height();
	m_oViewPort.MinDepth = 0.f;
	m_oViewPort.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &m_oViewPort);
	//_log_action_success("Created Viewport.");
}

void RenderScene::setSamplers()
{
	ID3D11SamplerState* pSamplers[6] = {
		m_poSampleStateLinear,
		m_poSampleStateDefault,
		m_poSampleStateBRDF,
		m_poSampleStateNormal,
		m_poSampleStateMask,
		m_poSampleStatePoint
	};
	m_pDeviceContext->PSSetSamplers(0, 6, pSamplers);
}

HRESULT RenderScene::createDeviceAndSwapChain(HWND _hwnd, IDXGISwapChain** ppDest)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width();
	sd.BufferDesc.Height = height();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT iCreateFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	iCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, iCreateFlags, featureLevels,
		_countof(featureLevels), D3D11_SDK_VERSION, &sd, ppDest, &m_pDevice,
		&featureLevel, &m_pDeviceContext);

	// try and create software swap chain if faile
	if (hr != S_OK)
	{
		DXCall(D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_SOFTWARE, NULL, iCreateFlags, featureLevels,
			_countof(featureLevels), D3D11_SDK_VERSION, &sd, ppDest, &m_pDevice,
			&featureLevel, &m_pDeviceContext));
	}

	return hr;
}

HRESULT RenderScene::createSwapChain1(HWND _hwnd, IDXGISwapChain1** ppDest)/*, ID3D11Texture2D* _pBackBuffer, ID3D11RenderTargetView* _pRenderTargetView)*/
{
	//----------------- get Factory2 interface ---------------
	IDXGIDevice2* pDXGIDevice;
	HRESULT hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory2* pIDXGIFactory;
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);

	//--------- create swap chain ------------------
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc1;
	m_oSwapChainSampleDesc = { 8, 0 };
	swapChainDesc1.Width = width();
	swapChainDesc1.Height = height();
	swapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc1.Stereo = FALSE;
	swapChainDesc1.SampleDesc = m_oSwapChainSampleDesc;
	swapChainDesc1.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc1.BufferCount = 3;
	swapChainDesc1.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
	swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc1.Flags = 0;

	IDXGISwapChain1** p = &m_oSwapChainData.m_pSwapChain1;
	hr = pIDXGIFactory->CreateSwapChainForHwnd(m_pDevice, _hwnd, &swapChainDesc1, NULL, NULL, &m_oSwapChainData.m_pSwapChain1);
	assert(SUCCEEDED(hr));
	_log_action_success("Created Swap Chain.");

	// Get swap chain's back buffer, create its render target view and set that view as render target
	hr = m_oSwapChainData.m_pSwapChain1->GetBuffer(0, __uuidof(*m_oSwapChainData.m_pBackBuffer), (void**)&m_oSwapChainData.m_pBackBuffer);
	assert(SUCCEEDED(hr));

	hr = m_pDevice->CreateRenderTargetView(m_oSwapChainData.m_pBackBuffer, nullptr, &m_oSwapChainData.m_pRenderTargetView);
	assert(SUCCEEDED(hr));

	m_oSwapChainData.m_pBackBuffer->Release();
	m_pDeviceContext->OMSetRenderTargets(1, &m_oSwapChainData.m_pRenderTargetView, NULL);

	_log_action_success("Created Render Targte.");

	return hr;
}

void RenderScene::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().x() == 0)
	{
		// TODO: Update your camera position based on the delta value.
	}
	else if (event->angleDelta().x() !=
		0) // horizontal scrolling - mice with another side scroller.
	{
		// m_pCamera->MouseWheelH += (float)(event->angleDelta().y() / WHEEL_DELTA);
	}
	else if (event->angleDelta().y() != 0)
	{
		// m_pCamera->MouseWheel += (float)(event->angleDelta().y() / WHEEL_DELTA);
	}

	QWidget::wheelEvent(event);
}

QPaintEngine* RenderScene::paintEngine() const
{
	return Q_NULLPTR;
}

void RenderScene::paintEvent(QPaintEvent* event) {}

void RenderScene::resizeEvent(QResizeEvent* event)
{
	if (m_bDeviceInitialized)
	{
		if (m_bExtendedeSwapChain)
			onReset1();
		else
			onReset();

		emit widgetResized();
	}

	QWidget::resizeEvent(event);
}

bool RenderScene::event(QEvent* event)
{
	switch (event->type())
	{
		// Workaround for https://bugreports.qt.io/browse/QTBUG-42183 to get key strokes.
		// To make sure that we always have focus on the widget when we enter the rect area.
	case QEvent::Enter:
	case QEvent::FocusIn:
	case QEvent::FocusAboutToChange:
		if (::GetFocus() != m_hWnd)
		{
			QWidget* nativeParent = this;
			while (true)
			{
				if (nativeParent->isWindow()) break;

				QWidget* parent = nativeParent->nativeParentWidget();
				if (!parent) break;

				nativeParent = parent;
			}

			if (nativeParent && nativeParent != this &&
				::GetFocus() == reinterpret_cast<HWND>(nativeParent->winId()))
				::SetFocus(m_hWnd);
		}
		break;
	case QEvent::KeyPress:
		emit keyPressed((QKeyEvent*)event);
		break;
	case QEvent::MouseMove:
		emit mouseMoved((QMouseEvent*)event);
		break;
	case QEvent::MouseButtonPress:
		emit mouseClicked((QMouseEvent*)event);
		break;
	case QEvent::MouseButtonRelease:
		emit mouseReleased((QMouseEvent*)event);
		break;
	}

	return QWidget::event(event);
}

LRESULT RenderScene::WndProc(MSG* pMsg)
{
	// Process wheel events using Qt's event-system.
	if (pMsg->message == WM_MOUSEWHEEL || pMsg->message == WM_MOUSEHWHEEL) return false;

	return false;
}

#if QT_VERSION >= 0x050000
bool RenderScene::nativeEvent(const QByteArray& eventType,
	void* message,
	long* result)
{
	Q_UNUSED(eventType);
	Q_UNUSED(result);

#    ifdef Q_OS_WIN
	MSG* pMsg = reinterpret_cast<MSG*>(message);
	return WndProc(pMsg);
#    endif

	return QWidget::nativeEvent(eventType, message, result);
}

#else // QT_VERSION < 0x050000
bool RenderScene::winEvent(MSG* message, long* result)
{
	Q_UNUSED(result);

#    ifdef Q_OS_WIN
	MSG* pMsg = reinterpret_cast<MSG*>(message);
	return WndProc(pMsg);
#    endif

	return QWidget::winEvent(message, result);
}
#endif // QT_VERSION >= 0x050000

///*
// *
// */
//#pragma comment(lib, "d3d11.lib")
//
//#include "RenderScene.h"
//
//#include <QDebug>
//#include <QEvent>
//#include <QWheelEvent>
//
//constexpr int FPS_LIMIT    = 60.0f;
//constexpr int MS_PER_FRAME = (int)((1.0f / FPS_LIMIT) * 1000.0f);
//
//RenderScene::RenderScene(QWidget* parent, bool _bExtendedSwapChain)
//    : QWidget(parent)
//    , m_pDevice(Q_NULLPTR)
//    , m_pDeviceContext(Q_NULLPTR)
//    , m_pSwapChain(Q_NULLPTR)
//    , m_pRTView(Q_NULLPTR)
//    , m_hWnd(reinterpret_cast<HWND>(winId()))
//    , m_bDeviceInitialized(false)
//    , m_bRenderActive(false)
//    , m_bStarted(false)
//    , m_BackColor{0.0f, 0.135f, 0.481f, 1.0f}
//    , m_bSExtendedeSwapChain(false)
//{
//    qDebug() << "[RenderScene::RenderScene] - Widget Handle: " << m_hWnd;
//
//    QPalette pal = palette();
//    pal.setColor(QPalette::Window, Qt::black);
//    setAutoFillBackground(true);
//    setPalette(pal);
//
//    setFocusPolicy(Qt::StrongFocus);
//    setAttribute(Qt::WA_NativeWindow);
//
//    // Setting these attributes to our widget and returning null on paintEngine event
//    // tells Qt that we'll handle all drawing and updating the widget ourselves.
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_NoSystemBackground);
//
//    connect(this, &RenderScene::MouseButtonDblClick, this, &RenderScene::onMouseDoubleClick);
//
// //   init();
//}
//
//RenderScene::~RenderScene() {}
//
//void RenderScene::release()
//{
//    m_bDeviceInitialized = false;
//    disconnect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);
//    m_qTimer.stop();
//
//    ReleaseObject(m_pRTView);
//    ReleaseObject(m_pSwapChain);
//    ReleaseObject(m_pDeviceContext);
//    ReleaseObject(m_pDevice);
//}
//
//void RenderScene::run()
//{
//    m_qTimer.start(MS_PER_FRAME);
//    m_bRenderActive = m_bStarted = true;
//}
//
//void RenderScene::pauseFrames()
//{
//    if (!m_qTimer.isActive() || !m_bStarted) return;
//
//    disconnect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);
//    m_qTimer.stop();
//    m_bRenderActive = false;
//}
//
//void RenderScene::continueFrames()
//{
//    if (m_qTimer.isActive() || !m_bStarted) return;
//
//    connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);
//    m_qTimer.start(MS_PER_FRAME);
//    m_bRenderActive = true;
//}
//
//void RenderScene::showEvent(QShowEvent * event)
//{
//    if (!m_bDeviceInitialized)
//    {
//        m_bDeviceInitialized = init();
//        emit deviceInitialized(m_bDeviceInitialized);
//    }
//
//    QWidget::showEvent(event);
//}
//
//bool RenderScene::createDeptBufferAndhStencil()
//{
//
//
//
//
//    // release interface it allocated
//    /*if (m_poDepthStencil != nullptr)
//        m_poDepthStencil->Release();*/
//
//        /*if (m_poDepthStencilView != nullptr)
//            m_poDepthStencilView->Release();*/
//
//
//
//
//
//
//
//    HRESULT hr = S_OK;
//    D3D11_TEXTURE2D_DESC depthTextureDesc;
//    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
//    depthTextureDesc.Width = width();
//    depthTextureDesc.Height = height();
//    depthTextureDesc.MipLevels = 1;
//    depthTextureDesc.ArraySize = 1;
//
//    //depthTextureDesc.SampleDesc = m_oSampleDesc;
//    depthTextureDesc.SampleDesc.Count = 8;
//    depthTextureDesc.SampleDesc.Quality = 0;
//    depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//
//    ID3D11Texture2D* DepthStencilTexture;
//    hr = m_pDevice->CreateTexture2D(&depthTextureDesc, NULL, &m_poDepthStencil);
//
//    assert(SUCCEEDED(hr));
//
//    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
//    ::ZeroMemory(&dsvDesc, sizeof(dsvDesc));
//    dsvDesc.Format = depthTextureDesc.Format;
//    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
//
//    hr = m_pDevice->CreateDepthStencilView(m_poDepthStencil, &dsvDesc, &m_poDepthStencilView);
//    assert(SUCCEEDED(hr));
//
//
//    D3D11_DEPTH_STENCIL_DESC dsDesc;
//
//    // Depth test parameters
//    dsDesc.DepthEnable = true;
//    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//    // Stencil test parameters
//    dsDesc.StencilEnable = false;
//    dsDesc.StencilReadMask = 0x00;
//    dsDesc.StencilWriteMask = 0x00;
//
//    // Stencil operations if pixel is front-facing
//    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//    // Stencil operations if pixel is back-facing
//    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//    // Create depth stencil state
//    ID3D11DepthStencilState* pDSState;
//    hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_On);
//
//    dsDesc.DepthEnable = false;
//    hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_poDepthStencilState_Off);
//
//
//    assert(SUCCEEDED(hr));
//
//
//
//
//
//    m_pDeviceContext->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
//
//
//
//
//
//
//
//    return hr;
//}
//
//bool RenderScene::init()
//{
//    if (m_bSExtendedeSwapChain)
//        return init1();
//
//    DXGI_SWAP_CHAIN_DESC sd               = {};
//    sd.BufferCount                        = 2;
//    sd.BufferDesc.Width                   = width();
//    sd.BufferDesc.Height                  = height();
//    sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//    sd.BufferDesc.RefreshRate.Numerator   = 60;
//    sd.BufferDesc.RefreshRate.Denominator = 1;
//    sd.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//    sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    sd.OutputWindow                       = m_hWnd;
//    sd.SampleDesc.Count                   = 8;
//    sd.SampleDesc.Quality                 = 0;
//    sd.Windowed                           = TRUE;
//    sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
//
//    UINT iCreateFlags = 0;
//#if defined(DEBUG) || defined(_DEBUG)
//    iCreateFlags = D3D11_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_SINGLETHREADED;
//#endif
//    D3D_FEATURE_LEVEL featureLevel;
//    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
//                                         D3D_FEATURE_LEVEL_10_0};
//
//    HRESULT hr = D3D11CreateDeviceAndSwapChain(
//        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, iCreateFlags, featureLevels,
//        _countof(featureLevels), D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
//        NULL, &m_pDeviceContext);
//    if (hr != S_OK)
//    {
//        DXCall(D3D11CreateDeviceAndSwapChain(
//            NULL, D3D_DRIVER_TYPE_SOFTWARE, NULL, iCreateFlags, featureLevels,
//            _countof(featureLevels), D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
//            &featureLevel, &m_pDeviceContext));
//    }
//
//    resetEnvironment();
//    createDeptBufferAndhStencil();
//
//
//
//    connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame);
//
//
//
//    return true;
//}
//
bool RenderScene::init1()
{
	// create device
	D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};
#ifdef _DEBUG
	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		NULL,
		&m_pDeviceContext);
	assert(SUCCEEDED(hr));
#else
	HRESULT hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&m_pDevice,
		NULL,
		&m_pDeviceContext);
	assert(SUCCEEDED(hr));;
#endif

	DXCall(

		createSwapChain1(m_hWnd, NULL/*&oSwapChainData.m_pSwapChain1*/) // , oSwapChainData.m_pBackBuffer, oSwapChainData.m_pRenderTargetView)

	);

	//// Set viewport
	m_oViewPort.TopLeftX = 0;
	m_oViewPort.TopLeftY = 0;
	m_oViewPort.Width = (FLOAT)width();
	m_oViewPort.Height = (FLOAT)height();
	m_oViewPort.MinDepth = 0.f;
	m_oViewPort.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &m_oViewPort);
	_log_action_success("Created Viewport.");

	m_bExtendedeSwapChain = true;

	createRasterizers();
	resetEnvironment();
	createDeptBufferAndhStencil(, );
	createSamplerStates();
	loadIBLMapsFromResource(IDR_DDS_IRR, IDR_DDS_RAD, IDR_DDS_LUT);

	_log_action_success("Created Rasterizers");

	//emit deviceInitialized_Once(true);

	// QT: Connect a timer to the frame updater
	connect(&m_qTimer, &QTimer::timeout, this, &RenderScene::onFrame1);
	_log_action_success("Created QT timer for rendering.");

	m_font = std::make_unique<SpriteFont>(device(), L"myfile.spritefont");
	m_spriteBatch = std::make_unique<SpriteBatch>(deviceContext());

	return true;
}

bool RenderScene::createRasterizers()
{
	D3D11_RASTERIZER_DESC wfdesc;
	std::ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	wfdesc.FrontCounterClockwise = FALSE;

	wfdesc.AntialiasedLineEnable = TRUE;
	wfdesc.MultisampleEnable = TRUE;
	HRESULT hr = m_pDevice->CreateRasterizerState(&wfdesc, &m_poRasterizerState_Solid);
	assert(SUCCEEDED(hr));

	// write frame
	std::ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	wfdesc.FrontCounterClockwise = FALSE;

	wfdesc.AntialiasedLineEnable = TRUE;
	wfdesc.MultisampleEnable = TRUE;
	hr = m_pDevice->CreateRasterizerState(&wfdesc, &m_poRasterizerState_WireFrame);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

bool RenderScene::getFrameBuffer()
{
	////RECT rc;
	////GetClientRect(hwndHandle, &rc);
	////m_dpi = GetDpiForWindow(hwndHandle);

	//ID3D11Texture2D* pBackBuffer = nullptr;
	//ID3D11Texture2D* pStagingBackBuffer = nullptr;
	//HRESULT hr = m_poSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

	//D3D11_TEXTURE2D_DESC desc_ret;
	//HRESULT hrCapture = _CaptureTexture(
	//	m_poContext,
	//	pBackBuffer,
	//	desc_ret,
	//	pStagingBackBuffer);

	////
	////if (SUCCEEDED(hr))
	////{
	////	hr = SaveDDSTextureToFile(m_poContext, pSurface,
	////		L"SCREENSHOT.DDS");
	////

	////	hr = SaveWICTextureToFile(m_poContext, pSurface,
	////		GUID_ContainerFormatPng, L"c:\\temp\\SCREENSHOT.PNG");
	////}

	////return true;

	//if (pStagingBackBuffer)
	//{
	//	D3D11_TEXTURE2D_DESC description;
	//	pStagingBackBuffer->GetDesc(&description);
	//	const int width = description.Width;
	//	const int height = description.Height;
	//	unsigned int size = width * height * 4;

	//	//	ID3D11Texture2D* pNewTexture = NULL;

	//	//	D3D11_TEXTURE2D_DESC description;
	//	//	pBackBuffer->GetDesc(&description);
	//	//	const int width = description.Width;
	//	//	const int height = description.Height;
	//	//	description.BindFlags = 0;
	//	//	description.SampleDesc.Count = 1;
	//	//	description.SampleDesc.Quality = 0;
	//	//	description.CPUAccessFlags = D3D11_CPU_ACCESS_READ| D3D11_CPU_ACCESS_WRITE;
	//	//	description.Usage = D3D11_USAGE_STAGING;
	//	//	unsigned int size = width * height * 4;

	//	//	HRESULT hrTex = m_poDevice->CreateTexture2D(&description, NULL, &pNewTexture);
	//	//	HRESULT hrTexTemp = m_poDevice->CreateTexture2D(&description, NULL, &pTempTexture);
	//	//	if (pNewTexture && hrTexTemp)
	//	//	{
	//	//		//m_poContext->CopyResource(pTempTexture, pBackBuffer);

	//			//unsigned int subresource_resolve = D3D11CalcSubresource(0, 0, description.MipLevels);
	//			//m_poContext->ResolveSubresource(pNewTexture, subresource_resolve, pBackBuffer, 0, description.Format);
	//			////m_poContext->ResolveSubresource(pNewTexture,0, pSurface, 0, description.Format);
	//			//
	//	D3D11_MAPPED_SUBRESOURCE resource;
	//	unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
	//	//HRESULT hr = m_poContext->Map(pNewTexture, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);
	//	HRESULT hr = m_poContext->Map(pStagingBackBuffer, subresource, D3D11_MAP::D3D11_MAP_READ, 0, &resource);

	//	//resource.pData; // TEXTURE DATA IS HERE

	//	const int pitch = width << 2;

	//	unsigned char* source = static_cast<unsigned char*>(resource.pData);
	//	vector<unsigned char> vecCaptured(height * (width * 4));

	//	int index = 0;
	//	for (int i = 0; i < height; ++i)
	//	{
	//		memcpy(&vecCaptured[index], source, width * 4);
	//		source += resource.RowPitch;
	//		index += width * 4;
	//	}

	//	m_poContext->Unmap(pStagingBackBuffer, 0);

	//	m_captureSize = size;
	//	m_captureWidth = width;
	//	m_captureHeight = height;

	//	time_t rawtime;

	//	struct tm timeinfo;
	//	char buffer[80];

	//	time(&rawtime);
	//	localtime_s(&timeinfo, &rawtime);

	//	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", &timeinfo);
	//	std::string strTime(buffer);

	//	string strFíleName = "RMEditor_Screenshot" + strTime + ".PNG";

	//	int enccode_result = lodepng::encode(strFíleName,
	//		vecCaptured, width, height, LodePNGColorType::LCT_RGBA, 8U);

	//	string error = lodepng_error_text(enccode_result);

	//	return true;
	//}

	return false;
}

//void RenderScene::onFrame()
//{
//    if (m_bRenderActive) tick();
//
//    beginScene();
//    render();
//
//    (m_bSExtendedeSwapChain) ? (endScene1()) : (endScene());
//
//    //endScene(); // Original
//}
//
//void RenderScene::beginScene()
//{
//    m_pDeviceContext->OMSetRenderTargets(1, &m_pRTView, NULL);
//    m_pDeviceContext->ClearRenderTargetView(m_pRTView,
//                                            reinterpret_cast<const float *>(&m_BackColor));
//}
//
//void RenderScene::endScene()
//{
//    if (FAILED(m_pSwapChain->Present(1, 0))) { onReset(); }
//}
//
void RenderScene::endScene1()
{
	DXGI_PRESENT_PARAMETERS present = { 0, NULL, NULL, NULL };
	if (FAILED(m_oSwapChainData.m_pSwapChain1->Present1(1, 0, &present))) { onReset(); }
}