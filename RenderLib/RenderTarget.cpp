#include "RenderTarget.h"

#include "Scene.h"

bool RenderLib::RenderTarget2D::init(RenderLib::Scene* _poScene, UINT _width, UINT _height)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource

	// get client dimension = render target dimensions

	/*RECT rc = getClientRect();
	UINT _width = rc.right;
	UINT _height = rc.bottom;*/

	//_poScene->getRT

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	textureDesc.SampleDesc.Count = 8;
	textureDesc.SampleDesc.Quality = 0;

	//textureDesc.SampleDesc = m_oSampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
	assert(SUCCEEDED(hr));

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;;// D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	assert(SUCCEEDED(hr));

	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.

	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
		&shaderResourceViewDesc, &m_cpoShaderResourceView);
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool RenderLib::RenderTarget2D::init(RenderLib::Scene* _poScene, UINT Width, UINT Height, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags)
{
	return init_impl(
		_poScene,
		Width,
		Height,
		1,
		1,
		Format,
		SampleDesc,
		Usage,
		BindFlags,
		0,
		0
	);
}

HRESULT RenderLib::RenderTarget2D::init_impl(RenderLib::Scene* _poScene, UINT Width, UINT Height, UINT MipLevels, UINT ArraySize, DXGI_FORMAT Format, DXGI_SAMPLE_DESC SampleDesc, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

	// Setup the texture description.

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	textureDesc.Width = Width;
	textureDesc.Height = Height;
	textureDesc.MipLevels = MipLevels;
	textureDesc.ArraySize = ArraySize;
	textureDesc.Format = Format;

	textureDesc.SampleDesc.Count = SampleDesc.Count;
	textureDesc.SampleDesc.Quality = SampleDesc.Quality;

	//textureDesc.SampleDesc = m_oSampleDesc;
	textureDesc.Usage = Usage;
	textureDesc.BindFlags = BindFlags;
	textureDesc.CPUAccessFlags = CPUAccessFlags;
	textureDesc.MiscFlags = MiscFlags;

	// Create the texture
	hr = _poScene->getDevice()->CreateTexture2D(&textureDesc, NULL, &m_cpoTexture);
	assert(SUCCEEDED(hr));

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;;// D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = _poScene->getDevice()->CreateRenderTargetView(m_cpoTexture.Get(), &renderTargetViewDesc, &m_cpoRenderTargetView);
	assert(SUCCEEDED(hr));

	CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = MipLevels;

	// Create the shader resource view.

	hr = _poScene->getDevice()->CreateShaderResourceView(m_cpoTexture.Get(),
		&shaderResourceViewDesc, &m_cpoShaderResourceView);
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return hr;
}

bool RenderLib::RenderTarget2D::generateQuad(Scene* _poScene)
{
	// **********************************************************************************
		// quad half size
		// **********************************************************************************

	ModelVertex vertices[6];

	float left = -1.f / 2.0f;
	float right = 1.f / 2.0f;
	float top = 1.f / 2.0f;
	float bottom = -1.f / 2.0f;

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = { left, top, 0.0f };  // Top left.
	vertices[0].uv = { 0.0f, 0.0f };

	vertices[1].position = { right, bottom, 0.0f };  // Bottom right.
	vertices[1].uv = { 1.0f, 1.0f };

	vertices[2].position = { left, bottom, 0.0f };  // Bottom left.
	vertices[2].uv = { 0.0f, 1.0f };

	// Second triangle.
	vertices[3].position = { left, top, 0.0f };  // Top left.
	vertices[3].uv = { 0.0f, 0.0f };

	vertices[4].position = { right, top, 0.0f };  // Top right.
	vertices[4].uv = { 1.0f, 0.0f };

	vertices[5].position = { right, bottom, 0.0f };  // Bottom right.
	vertices[5].uv = { 1.0f, 1.0f };

	oQuad.createVertexBuffer(_poScene->getDevice(), sizeof(ModelVertex) * 6, vertices);

	uint32_t indices[6];
	for (unsigned short i = 0; i < 6; i++)
		indices[i] = i;

	oQuad.createIndexBuffer(_poScene->getDevice(), 6 * 4, indices);

	return true;
}