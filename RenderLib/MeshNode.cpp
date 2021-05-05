#include "MeshNode.h"
#include "Scene.h"

bool RenderLib::MeshNode::loadIBLMapsFromResource(const RenderLib::Scene* _poScene, UINT _irr, UINT _rad, UINT _lut)
{
	bool b1 = res_tools::loadCubeMapTextureFromResource(_poScene->getDevice(), _irr, NULL, &m_poSRV_CubeMap_Diffuse);
	bool b2 = res_tools::loadCubeMapTextureFromResource(_poScene->getDevice(), _rad, NULL, &m_poSRV_CubeMap_Specular);
	bool b3 = res_tools::loadCubeMapTextureFromResource(_poScene->getDevice(), _lut, NULL, &m_poSRV_LookUpTexture);

	return b1 && b2 && b3;
}

void RenderLib::MeshNode::setEnvMaps(ID3D11DeviceContext* _pDeviceContext)
{
	ID3D11ShaderResourceView* ppShaderResourceViews[3];

	ppShaderResourceViews[0] = m_poSRV_CubeMap_Diffuse;
	ppShaderResourceViews[1] = m_poSRV_CubeMap_Specular;
	ppShaderResourceViews[2] = m_poSRV_LookUpTexture;

	_pDeviceContext->PSSetShaderResources(
		0,
		3,
		ppShaderResourceViews
	);
}

RenderLib::SubMeshData::~SubMeshData()
{
	/*if (m_cpVertexBuffer)
	{
		m_cpVertexBuffer->Release();
	}*/

	//if (m_poIndexBuffer)
	//{
	//	m_poIndexBuffer->Release();
	//}

	//// shaders
	//if (m_poVertexShader)
	//{
	//	m_poVertexShader->Release();
	//}

	//if (m_poPixelShader)
	//{
	//	m_poPixelShader->Release();
	//}

	/*if (m_poInputLayout)
	{
		m_poInputLayout->Release();
	}*/
}

bool RenderLib::SubMeshData::loadTextures(const MaterialInfo* _poMaterial)
{
	// TODO: fill oMaterial, load textures from the paths in _poMaterial
	return false;
}

bool RenderLib::MeshNode::allocateBoundingBoxes(Scene* _poScene)
{
	if (!m_spoMeshLodData->vecLODs.size())
		return false;

	m_vecBoundingBoxMeshes.clear();
	m_vecBoundingBoxMeshes.resize(m_spoMeshLodData->vecLODs[0].vecspoMeshData.size());

	for (size_t group = 0; group < m_spoMeshLodData->vecLODs[0].vecspoMeshData.size(); group++)
	{
		// allocate one mesh block
		auto spoBoundingBoxMesh = shared_ptr<RenderLib::SubMeshData>(new RenderLib::SubMeshData);

		m_vecBoundingBoxMeshes[group] = spoBoundingBoxMesh;

		spoBoundingBoxMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		spoBoundingBoxMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

		spoBoundingBoxMesh->m_poPixelShader =
			_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);

		spoBoundingBoxMesh->m_poVertexShader =
			_poScene->getResources()->getVertexShader(Resources::EVertexShader::eWeighted4);

		spoBoundingBoxMesh->m_poInputLayout =
			_poScene->getResources()->getInputLayout(Resources::EVertexShader::eWeighted4);

		auto& A = m_spoMeshLodData->vecLODs[0].vecspoMeshData[group].vBBMin;
		auto& B = m_spoMeshLodData->vecLODs[0].vecspoMeshData[group].vBBMax;

		std::vector<DirectX::XMFLOAT3>  vecCubeVertices = {
		{A.x, A.y, A.z}, {B.x, A.y, A.z }, {B.x, B.y, A.z}, {A.x, B.y, A.z},
		{A.x, A.y, B.z},  {B.x, A.y, B.z },   {B.x, B.y, B.z},  {A.x, B.y, B.z}
		};

		ModelVertex* pv = new ModelVertex[vecCubeVertices.size()];

		for (size_t i = 0; i < vecCubeVertices.size(); i++)
		{
			memset(&pv[i], 0, sizeof(ModelVertex));
			pv[i].position.x = vecCubeVertices[i].x;
			pv[i].position.y = vecCubeVertices[i].y;
			pv[i].position.z = vecCubeVertices[i].z;
			//pv[i].position.w = 0.f;

			pv[i].color = { 1.0f, 0.1f, 0.1f, 1.0f };
		}

		spoBoundingBoxMesh->createVertexBuffer(_poScene->getDevice(), vecCubeVertices.size() * sizeof(ModelVertex), pv);
		delete[] pv;

		spoBoundingBoxMesh->createIndexBuffer(_poScene->getDevice(), sm_vecCubeIndices.size() * sizeof(uint16_t), &sm_vecCubeIndices[0]);

		spoBoundingBoxMesh->m_index_count = sm_vecCubeIndices.size();
	}

	return true;
}

RenderLib::MeshNode* RenderLib::MeshNode::create(RenderLib::Scene* _poScene, const std::string& _strName)
{
	auto po = new MeshNode(_strName);
	_poScene->addObject(po); // at to "abstract pointer list" for later clean up

	po->m_spoMeshLodData = shared_ptr<MeshLodData>(new MeshLodData);

	po->init(_poScene);

	return po;
}

bool RenderLib::MeshNode::init(RenderLib::Scene* _poScene)
{
	return BaseNode::init(_poScene);
}

void RenderLib::MeshNode::setShowBoundBox(bool _bState)
{
	m_bShowBoundingBox = _bState;
}

void RenderLib::MeshNode::draw(RenderLib::Scene* _poScene, Shader::Shader* _poShader)
{
	auto _deviceContext = _poScene->getDeviceContext();
	BaseNode::draw(_poScene);

	if (m_bShowBoundingBox)
		for (auto& it : m_vecBoundingBoxMeshes)
			it->drawMesh(_poScene);

	UINT stride = sizeof(ModelVertex);
	UINT offset = 0;

	setEnvMaps(_deviceContext);

	if (m_spoMeshLodData->vecLODs.size() > 0)
		for (size_t group = 0; group < m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData.size(); group++)
		{
			if (!_poShader)
			{
				_deviceContext->IASetInputLayout(m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].m_poInputLayout);
				_deviceContext->VSSetShader(m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].m_poVertexShader, nullptr, 0);
				_deviceContext->PSSetShader(m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].m_poPixelShader, nullptr, 0);
			}
			_deviceContext->IASetVertexBuffers(0, 1,
				m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].vertexBufferAddressReadOnly(),
				&stride, &offset);

			_deviceContext->IASetIndexBuffer(
				m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].indexBuffer(),
				m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].EIndexFormat, 0);

			//deviceContext->RSSetViewports(1, poD3DWidget->viewPort());
			//_deviceContext->RSSetState(m_poRasterizerStateWireFrame);
			//_deviceContext->RSSetState(NULL);

			//ID3D11ShaderResourceView* p[5];
			//p[0] = m_PPpShaderResourceView[group][TEXTURE_DIFFUSE];
			//p[1] = m_PPpShaderResourceView[group][TEXTURE_NORMAL];
			//p[2] = m_PPpShaderResourceView[group][TEXTURE_SPECULAR];
			//p[3] = m_PPpShaderResourceView[group][TEXTURE_MASK];
			//p[4] = m_PPpShaderResourceView[group][TEXTURE_GLOSS];*/
			//*p[5] = m_poShaderViewReflectionMapBlurLevel1;
			//p[6] = m_poShaderViewReflectionMapBlurLevel2;*/

			if (m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size())
			{
				_deviceContext->PSSetShaderResources(
					3,
					m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size(),
					&m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[0]
				);
			}

			//deviceContext->PSSetShaderResources(0, 1, &textureView);
			//deviceContext->PSSetSamplers(0, 1, &samplerState);

			//_deviceContext->OMSetRenderTargets(1, &frameBufferView, nullptr);
			//deviceContext->OMSetDepthStencilState(depthStencilState, 0);
			//deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)

			///////////////////////////////////////////////////////////////////////////////////////////
			_deviceContext->IASetPrimitiveTopology(m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].ETopology);
			//deviceContext->DrawIndexed(oTestNode.uiIndexCount, 0, 0);

			_deviceContext->DrawIndexed(m_spoMeshLodData->vecLODs[m_lod].vecspoMeshData[group].indexCount(), 0, 0);

			///////////////////////////////////////////////
			// draw lod mesh contents
			//m_oMeshLodData.vecLODs[m_lod].vecspm_oMeshLodData.group]->drawSubMesh(_deviceContext);
			///////////////////////////////////////////////

			///////////////////////////////////////////////
			// draw bounding boxed
			//m_vecBoundingBoxMeshes[group]->drawSubMesh(_deviceContext);
			///////////////////////////////////////////////////////////////////////////////////////////

			//m_spoCubeMesh->drawSubMesh(_deviceContext);
		}
}

void RenderLib::MeshNode::setLOD(size_t _lod)
{
	if (m_spoMeshLodData->vecLODs.size() == 0)
		return;

	m_lod = (_lod >= m_spoMeshLodData->vecLODs.size()) ? m_lod = m_spoMeshLodData->vecLODs.size() - 1 : _lod;
}

SystemLib::return_value RenderLib::SubMeshData::createVertexBuffer(
	ID3D11Device* _poDevice,
	UINT _data_size,
	const void* _pvData,
	D3D11_USAGE _Usage,
	UINT _CPUAccessFlags)

{
	// Setup the buffer descript for "vertex buffer"
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = _Usage;
	vertexBufferDesc.ByteWidth = _data_size;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = _CPUAccessFlags;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Setup init data
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = _pvData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, m_cpVertexBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

SystemLib::return_value RenderLib::SubMeshData::createIndexBuffer(
	ID3D11Device* _poDevice, UINT _data_size,

	const void* _pvData,

	D3D11_USAGE _Usage,
	UINT _CPUAccessFlags)
{
	// Setup the buffer descript for "index buffer"
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = _Usage;
	vertexBufferDesc.ByteWidth = _data_size;
	vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = _CPUAccessFlags;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Setup init data
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = _pvData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (indexData.pSysMem) ? &indexData : NULL, &m_poIndexBuffer);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}