#include "Scene.h"
#include "BaseNode.h"
#include "IDrawableNode.h"
#include "IRenderQueue.h"

RenderLib::Scene::Scene()
{
}

void RenderLib::Scene::updateVSPerModelConstBuffer()
{
	// world
	//m_VS_PerModel_ConstantBuffer.world

	// camera + projection

	m_VS_PerModel_ConstantBuffer.mView = getCamera().ViewMatrix;
	m_VS_PerModel_ConstantBuffer.mViewInverse = getCamera().InvViewMatrix;
	m_VS_PerModel_ConstantBuffer.mProjection = getCamera().ProjectMatrixPerspective;
	m_poDeviceContext->UpdateSubresource(m_poVertexShaderPerModelConstBuffer, 0, nullptr, &m_VS_PerModel_ConstantBuffer, 0, 0);
	//  poD3DWidget->deviceContext()->UpdateSubresource(m_poPerFrameConstBuffer, 0, nullptr, &m_PS_PerFrame_ConstantBuffer,0, 0);

	m_poDeviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_poVertexShaderPerModelConstBuffer);
}

void RenderLib::Scene::update_PS_PerFrameConstBuffer()
{
	m_poDeviceContext->UpdateSubresource(m_poPixelShaderPerFrameConstBuffer, 0, nullptr, &m_PS_PerFrame_ConstantBuffer, 0, 0);
	//  poD3DWidget->deviceContext()->UpdateSubresource(m_poPerFrameConstBuffer, 0, nullptr, &m_PS_PerFrame_ConstantBuffer,0, 0);

	m_poDeviceContext->PSSetConstantBuffers(
		0,
		1,
		&m_poPixelShaderPerFrameConstBuffer);
}

void RenderLib::Scene::update_PS_PerMeshConstBuffer()
{
	m_poDeviceContext->UpdateSubresource(m_poPixelShaderPerMeshConstBuffer, 0, nullptr, &m_PS_PerMesh_ConstantBuffer, 0, 0);
	m_poDeviceContext->PSSetConstantBuffers(
		1,
		1,
		&m_poPixelShaderPerMeshConstBuffer);
}

void RenderLib::Scene::update_PS_PerMeshConstBuffer(const MaterialData& _oMat)
{
	m_PS_PerMesh_ConstantBuffer.alpha_used = _oMat.alpha;

	m_poDeviceContext->UpdateSubresource(m_poPixelShaderPerMeshConstBuffer, 0, nullptr, &m_PS_PerMesh_ConstantBuffer, 0, 0);
	m_poDeviceContext->PSSetConstantBuffers(
		1,
		1,
		&m_poPixelShaderPerMeshConstBuffer);
}

void RenderLib::Scene::updatePSLightConstBuffer()
{
	m_poDeviceContext->UpdateSubresource(m_poLightConstBufferr, 0, nullptr, &m_oPS_LightBuffer, 0, 0);
	m_poDeviceContext->PSSetConstantBuffers(
		0,
		1,
		&m_poLightConstBufferr);
}

RenderLib::Scene* RenderLib::Scene::create(ID3D11Device* _poDevice, ID3D11DeviceContext* _poDC, const std::string& _strName, Resources::IResourceContainer* _pRes)
{
	auto m_poScene = new Scene;
	g_spoScene = shared_ptr<Scene>(m_poScene);
	m_poScene->m_poDeviceContext = _poDC;
	m_poScene->m_strName = _strName;
	m_poScene->m_poDevice = _poDevice;
	m_poScene->m_spoResources = shared_ptr<Resources::IResourceContainer>(_pRes);
	m_poScene->m_spoResources->loadAll(_poDevice);

	m_poScene->m_spoRootNode = std::shared_ptr<RenderLib::IDrawableNode>(new IDrawableNode("root_node"));
	m_poScene->m_spoRootNode->allocateCube(m_poScene);

	m_poScene->createConstBuffers();
	//m_poScene->m_poSSAORenderTarget = RenderTarget2D::create()

	m_poScene->spoRendererTechnique = std::shared_ptr < RenderLib::SSAO_RenderTechnique>(new RenderLib::SSAO_RenderTechnique);
	m_poScene->spoRendererTechnique->init(1024, 1024, m_poScene);

	return m_poScene;
}

RenderLib::BaseNode* RenderLib::Scene::getRootNode()
{
	return m_spoRootNode.get();
}

Camera& RenderLib::Scene::getCamera()
{
	return m_oCamera;
}

void RenderLib::Scene::render(ID3D11DeviceContext* _poDC)
{
	updatePSLightConstBuffer();

	if (m_poRenderTargetView)
	{
		m_poDeviceContext->OMSetRenderTargets(1, &m_poRenderTargetView, NULL);
		float color[4] = { 0, 0, 1, 1 };
		m_poDeviceContext->ClearRenderTargetView(m_poRenderTargetView, color);

		/*deviceContext()->ClearDepthStencilView(
			depthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);*/
	}

	if (m_bRenderingRunning)
	{
		//		m_spoRootNode->m_xmOrientation = SimpleMath::Matrix::CreateRotationY(XM_PI);
		//		m_spoRootNode->render(this, poSSAOSHader.get());

		//spoRenderer->clear();
		m_spoRootNode->render(this);
		//m_spoRootNode->process(&spoRendererTechnique->oRenderQueue);


		//spoRendererTechnique->

		//spoRendererTechnique->renderScene(this, &spoRendererTechnique->oRenderQueue);
	}

	int debug_1 = 1;
}

ID3D11Device* RenderLib::Scene::getDevice() const
{
	return m_poDevice;
}

ID3D11DeviceContext* RenderLib::Scene::getDeviceContext() const
{
	return m_poDeviceContext;
}

void RenderLib::Scene::addObject(RenderLib::BaseNode* _po)
{
	vecObjects.push_back(std::shared_ptr<RenderLib::BaseNode>(_po));
}

Resources::IResourceContainer* RenderLib::Scene::getResources()
{
	return m_spoResources.get();
}

void RenderLib::Scene::_setPixelShaderResource(size_t _index, ID3D11PixelShader* _poResource)
{
	m_mapPixelShaders[_index] = _poResource;
}

ID3D11PixelShader* RenderLib::Scene::getPixelShaderResource(size_t _index)
{
	auto  it = m_mapPixelShaders.find(_index);
	if (it == m_mapPixelShaders.end())
		throw std::out_of_range("Pixel Shader Resource Not Found!");

	return it->second;
}
bool RenderLib::Scene::createConstBuffers()
{
	// Pixel shader, PER FRAME
	{
		CD3D11_BUFFER_DESC cbDesc(
			sizeof(m_PS_PerFrame_ConstantBuffer),
			D3D11_BIND_CONSTANT_BUFFER
		);

		//	Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_PS_PerFrame_ConstantBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr = m_poDevice->CreateBuffer(
			&cbDesc,
			&InitData,
			&m_poPixelShaderPerFrameConstBuffer
		);
		assert(SUCCEEDED(hr));
	}

	// Pixel shader, PER MESH const buffer

	size_t legnth = sizeof(m_PS_PerMesh_ConstantBuffer);
	{
		CD3D11_BUFFER_DESC cbDesc(
			sizeof(m_PS_PerMesh_ConstantBuffer),
			D3D11_BIND_CONSTANT_BUFFER
		);

		//	Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_PS_PerMesh_ConstantBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr = m_poDevice->CreateBuffer(
			&cbDesc,
			&InitData,
			&m_poPixelShaderPerMeshConstBuffer

		);
		assert(SUCCEEDED(hr));
	}

	// Vertex shader, PER MODEL
	{
		CD3D11_BUFFER_DESC cbDesc(
			sizeof(m_VS_PerModel_ConstantBuffer),
			D3D11_BIND_CONSTANT_BUFFER
		);

		//	Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_VS_PerModel_ConstantBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr = m_poDevice->CreateBuffer(
			&cbDesc,
			&InitData,
			&m_poVertexShaderPerModelConstBuffer
		);
		assert(SUCCEEDED(hr));
	}

	// Vertex shader, PER FRAME
	{
		CD3D11_BUFFER_DESC cbDesc(
			sizeof(m_PS_PerFrame_ConstantBuffer),
			D3D11_BIND_CONSTANT_BUFFER
		);

		//	Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &m_PS_PerFrame_ConstantBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr = m_poDevice->CreateBuffer(
			&cbDesc,
			&InitData,
			&m_poPixelShaderPerFrameConstBuffer
		);
		assert(SUCCEEDED(hr));
	}

	if (!createLightBuffer())
		return false;

	return true;
}

bool RenderLib::Scene::createLightBuffer()
{
	HRESULT hr = S_OK;
	//////////// Light buffer ////////////
	D3D11_BUFFER_DESC cbDescLight;
	cbDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	cbDescLight.Usage = D3D11_USAGE_DEFAULT;
	cbDescLight.ByteWidth = sizeof(m_oPS_LightBuffer);
	cbDescLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDescLight.CPUAccessFlags = 0;
	cbDescLight.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_oPS_LightBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = m_poDevice->CreateBuffer(
		&cbDescLight,
		&InitData,
		&m_poLightConstBufferr
	);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

//void RenderLib::Scene::renderStuff()
//{
//	// TODO: Present your scene here. For aesthetics reasons, only do it here if it's an
//	// important component, otherwise do it in the MainWindow.
//	// m_pCamera->Apply();
//
//	/*if (!scene())
//		return;*/
//
//		/*m_oCamera.calculateMatrices(width(), height());/*****************************/
//		// setSamplers(); /*****************************/
//
//	auto pTarget = m_oSwapChainData.m_pRenderTargetView;
//	float color[4] = { 0, 0, 0, 1 };
//
//	m_poDeviceContext->OMSetRenderTargets(1, &pTarget, m_poDepthStencilView);
//	m_poDeviceContext->ClearRenderTargetView(pTarget, color);
//
//	// clear z buffer (and stenicl buffer)
//	m_poDeviceContext->ClearDepthStencilView(
//		m_poDepthStencilView,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
//		1.0f,
//		0);
//
//	// load cube maps for env ambient PBR lighting
//// TODO: make it so there can more than one of these
//
//	//setEnvMaps();  /*****************************/
//
//	render(m_poDeviceContext);
//
//	//m_poDeviceContext->OMSetDepthStencilState(m_poDepthStencilState_Off, 1);
//
//	//m_fontPos.x = 0; //width() / 2.f;
//	//m_fontPos.y = 0; // height() / 2.f;
//
//	//m_spriteBatch->Begin();
//
//	//const wchar_t* output = L"Hello World";
//
//	//DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_font->MeasureString(output) / 2.f;
//
//	//DirectX::SimpleMath::Vector3 vScale = { 0.6, 0.6, 0.6 };
//	//DirectX::SimpleMath::Color vColor = { 0.5, 0.5, 0.5, 1 };
//
//	//m_font->DrawString(m_spriteBatch.get(), m_strStringToRender.c_str(),
//	//	m_fontPos, vColor, 0.f, origin);
//
//	//vScale = { 0.5, 0.5, 0.5 };
//	//vColor = { 1, 1, 1, 1 };
//
//	//m_font->DrawString(m_spriteBatch.get(), m_strStringToRender.c_str(),
//	//	m_fontPos, vColor, 0.f, origin);
//
//	//m_spriteBatch->End();
//
//	//deviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
//
//	// TODO: shall this remain disabled
//	/* is there is any reason to render stuff from the main window at all? */
//
//	//emit rendered(this);
//}

shared_ptr<RenderLib::Scene> RenderLib::g_spoScene;