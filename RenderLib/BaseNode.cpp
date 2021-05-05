#include "pch.h"
#include "BaseNode.h"
#include "lib3d.h"
#include "..\QtRME_GUI\render_resources.h"
#include "IAbstractNode.h"
#include "..\ImporterLib\FBX_Reader.h"
#include "IRenderQueue.h"

RenderLib::BaseNode::BaseNode(RenderLib::BaseNode* _poRoot)
{
	this->poRoot = _poRoot;
}

RenderLib::BaseNode::BaseNode()
{
}

void RenderLib::BaseNode::setVisible(bool _bState)
{
	m_bVisible = _bState;
}

bool RenderLib::BaseNode::visible()
{
	return m_bVisible;
}

//RenderLib::BaseNode* RenderLib::BaseNode::create(RenderLib::Scene* _poScene, const std::string& _strName)
//{
//	auto po = new BaseNode();
//
//	//_poScene->vecObjects.push_back(shared_ptr<IObjecet>(po));
//
//	return po;
//}

RenderLib::BaseNode* RenderLib::BaseNode::create(RenderLib::Scene* _poScene, const std::string& _strName)
{
	auto po = new BaseNode(_strName);
	_poScene->addObject(po);
	po->init(_poScene);
	po->m_strName = _strName;

	return po;
}

bool RenderLib::BaseNode::init(Scene* _poScene) {
	return allocateCube(_poScene);
}

const string& RenderLib::BaseNode::getName()
{
	return m_strName;
}

RenderLib::BaseNode* RenderLib::BaseNode::getParent()
{
	return m_poParent;
}

void RenderLib::BaseNode::render(Scene* _poScene, Shader::Shader* _poShader)
{
	updateGlobalMatrix();
	//XMStoreFloat4x4(&_poScene->m_VS_PerModel_ConstantBuffer.world, XMMatrixTranspose(m_xmGlobalTransform));
	_poScene->m_VS_PerModel_ConstantBuffer.mWorld = (m_xmGlobalTransform).Transpose();
	_poScene->m_VS_PerModel_ConstantBuffer.mProjection =
		_poScene->getCamera().ProjectMatrixPerspective;

	_poScene->updateVSPerModelConstBuffer();

	draw(_poScene);

	for (auto& it : m_vecChildren)
	{
		if (it->visible())
		{
			it->render(_poScene, _poShader);
		}
	}
}

void RenderLib::BaseNode::process(RenderLib::RendererQueue* _poRenderer)
{
	_poRenderer->addNode(this);

	for (auto& it : m_vecChildren)
	{
		if (it->visible())
		{
			it->process(_poRenderer);
		}
	}
}

void RenderLib::BaseNode::draw(RenderLib::Scene* _poScene, Shader::Shader* _poShader)
{
	if (m_bIsCubeVisible)
	{
		drawBoundingBox(_poScene);
	}
}

bool RenderLib::BaseNode::setCubeVisible(bool _bState)
{
	bool bOldState = m_bIsCubeVisible;
	m_bIsCubeVisible = _bState;
	return bOldState;
}

RenderLib::BaseNode::operator bool()
{
	return m_bIValid;
}

void RenderLib::BaseNode::transform(const DirectX::XMFLOAT4X4& _mMatrix)
{
	//XMMATRIX m = XMLoadFloat4x4(&_mMatrix);
	SimpleMath::Matrix m = _mMatrix;

	m_xmRotation = m * m_xmRotation;
}

void RenderLib::BaseNode::mulTranslate(const DirectX::XMFLOAT3& _vTranslation)
{
	m_xmTranslation = m_xmTranslation * XMMatrixTranslation(_vTranslation.x, _vTranslation.y, _vTranslation.z);
}

void RenderLib::BaseNode::setTranslate(const DirectX::XMFLOAT3& _t)
{
	m_xmTranslation = XMMatrixTranslation(_t.x, _t.y, _t.z);
}

void RenderLib::BaseNode::setScale(const DirectX::XMFLOAT3& _s)
{
	m_xmScaling = XMMatrixScaling(_s.x, _s.y, _s.z);
}

void RenderLib::BaseNode::setTranslateX(float x)
{
}

void RenderLib::BaseNode::setRotateQuaternion(const DirectX::XMFLOAT4& _qOrienation)
{
	m_qLocalOrientation = _qOrienation;
}

//void RenderLib::BaseNode::setRotateMatrix(const DirectX::XMFLOAT4X4& _mOrienation)
//{
//	m_xmRotation =	_mOrienation;
//}

/// <summary>
/// Sets EULER rotation to ABSOLUTE values (radians)
/// </summary>
/// <param name="_v">: vector containing rotation for each axis</param>
void RenderLib::BaseNode::setRotationEuler(const DirectX::XMFLOAT3& _v)
{
	m_xmRotation =

		(
		XMMatrixRotationZ(_v.z) *
		XMMatrixRotationY(_v.y) *
		XMMatrixRotationX(_v.x)
		);
}

/// <summary>
/// "Adds" rotation to the node's existing rotation (radians)
/// </summary>
/// <param name="_v">: vector containing rotation for each axis</param>
void RenderLib::BaseNode::rotateEuler(const DirectX::XMFLOAT3& _v)
{
	m_xmRotation =
		m_xmRotation *
		(
		XMMatrixRotationZ(_v.z) *
		XMMatrixRotationY(_v.y) *
		XMMatrixRotationX(_v.x)
		);
}

void RenderLib::BaseNode::rotateRoll(const DirectX::XMFLOAT3& _vRoll)
{
	m_xmRotation = XMMatrixRotationRollPitchYaw(_vRoll.x, _vRoll.x, _vRoll.x);
}

const DirectX::XMFLOAT4X4& RenderLib::BaseNode::getGlobalTransform()
{
	//updateGlobalMatrix();
	DirectX::XMStoreFloat4x4(&m_matrixTemp, DirectX::XMMatrixTranspose(m_xmGlobalTransform));

	return m_matrixTemp;
}

bool RenderLib::BaseNode::allocateCube(Scene* _poScene)
{
	m_spoCubeMesh = _poScene->getResources()->getSubMeshData(Resources::ESubMeshData::eCubeMesh);

	if (!m_spoCubeMesh) // not a valid pointer = not created = create!
	{
		m_spoCubeMesh = std::shared_ptr<SubMeshData>(new SubMeshData);
		m_spoCubeMesh->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		m_spoCubeMesh->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		m_spoCubeMesh->m_poPixelShader =
			_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);

		m_spoCubeMesh->m_poVertexShader =
			_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);

		m_spoCubeMesh->m_poInputLayout =
			_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);

		m_spoCubeMesh->m_poRasterizerState =
			_poScene->getResources()->getRasterizerState(Resources::ERasterizers::eSolid);

		ModelVertex* pv = new ModelVertex[sm_vecCubeVertices.size()];

		for (size_t i = 0; i < sm_vecCubeVertices.size(); i++)
		{
			memset(&pv[i], 0, sizeof(ModelVertex));
			pv[i].position.x = sm_vecCubeVertices[i].x * 0.0125f;
			pv[i].position.y = sm_vecCubeVertices[i].y * 0.0125f;
			pv[i].position.z = sm_vecCubeVertices[i].z * 0.0125f;
			//pv[i].position.w = 0.f;

			pv[i].color = { 0.1f, 1.f, 0.1f, 1.0f };
		}

		m_spoCubeMesh->createVertexBuffer(_poScene->getDevice(), sm_vecCubeVertices.size() * (size_t)sizeof(ModelVertex), pv);
		delete[] pv;

		m_spoCubeMesh->createIndexBuffer(_poScene->getDevice(), sm_vecCubeIndices.size() * (size_t)sizeof(uint16_t), &sm_vecCubeIndices[0]);

		m_spoCubeMesh->m_index_count = sm_vecCubeIndices.size();

		// store
		_poScene->getResources()->setSubMeshData(Resources::ESubMeshData::eCubeMesh, m_spoCubeMesh);
	}

	return true;
}

void RenderLib::BaseNode::makeLineRecursvice(Scene* _poScene, vector<DirectX::XMFLOAT4>& vertices, vector<uint16_t>& indices)
{
	XMFLOAT4X4 mGlobal;
	XMStoreFloat4x4(&mGlobal, m_xmGlobalTransform);

	XMFLOAT4 po;
	po.x = mGlobal._14;
	po.y = mGlobal._24;
	po.z = mGlobal._34;
	po.z = 1;

	vertices.push_back(po);

	auto offset = indices.size();
	for (size_t i = 0; i < m_vecChildren.size(); i++)
	{
		XMStoreFloat4x4(&mGlobal, m_vecChildren[i]->m_xmGlobalTransform);

		po.x = mGlobal._14;
		po.y = mGlobal._24;
		po.z = mGlobal._34;
		po.z = 1;

		vertices.push_back(po);

		// make line from parent to each child
		indices.push_back(offset + 0);
		indices.push_back(offset + i);
	}

	for (auto& it : m_vecChildren)
	{
		it->makeLineRecursvice(_poScene, vertices, indices);
	}
}

//void RenderLib::BaseNode::makeLines(Scene* _poScene,
//	vector<XMFLOAT4> v, vector<uint16_t> i)
//{
//	static int vertex_index = 0;
//	static int index = 0;
//
//	ModelVertex* pv;
//
//	XMFLOAT4X4 mGlobal;
//	XMStoreFloat4x4(&mGlobal, m_xmGlobalTransform);
//
//
//
//
//	for (auto& it : m_vecChildren)
//	{
//
//
//		it->render(_poScene);
//	}
//
//
//}

void RenderLib::BaseNode::drawBoundingBox(Scene* _poScene)
{
	//_poScene->m_VS_PerModel_ConstantBuffer.world = m_xmGlobalTransform; // *SimpleMath::Matrix::CreateScale(1.2f);
	//_poScene->updatePerOjbect(_poScene->getDeviceContext());

	m_spoCubeMesh->drawMesh(_poScene);
}

void RenderLib::BaseNode::__drawCube(ID3D11DeviceContext* _poContext)
{
	//	m_spoCubeMesh->drawSubMesh(_poContext);
}

void RenderLib::BaseNode::updateGlobalMatrix()
{
	/*m_xmLocalTransform = m_xmTranslation * m_xmRotation * m_xmScale;*/

	if (m_poParent)
		m_xmGlobalTransform = (m_xmScaling * (SimpleMath::Matrix::CreateFromQuaternion(m_qLocalOrientation) * m_xmRotation) * m_xmTranslation) * m_poParent->m_xmGlobalTransform;
	else
		m_xmGlobalTransform = (m_xmScaling * (SimpleMath::Matrix::CreateFromQuaternion(m_qLocalOrientation) * m_xmRotation) * m_xmTranslation);
}

void RenderLib::BaseNode::updateLocalMatrix()
{
	m_xmLocalTransform = m_xmTranslation * (m_xmOrientation * m_xmRotation) * m_xmScaling;
}

void RenderLib::BaseNode::update()
{
	updateGlobalMatrix();

	for (auto& it : m_vecChildren)
	{
		it->update();
	}
}

RenderLib::BaseNode* RenderLib::BaseNode::addChild(BaseNode* _poBaseNode)
{
	_poBaseNode->m_poParent = this;
	m_vecChildren.push_back(_poBaseNode);

	return _poBaseNode;
}

void RenderLib::BaseNode::appendChild(BaseNode* _poBaseNode)
{
	_poBaseNode->m_poParent = this;
	m_vecChildren.push_back(_poBaseNode);
}

void RenderLib::BaseNode::insertChild(size_t _index, BaseNode* _poNode)
{
	_poNode->m_poParent = this;
	m_vecChildren.insert(m_vecChildren.begin() + _index, _poNode);
}

size_t RenderLib::BaseNode::getChildCount()
{
	return m_vecChildren.size();
}

RenderLib::BaseNode* RenderLib::BaseNode::findChild(const std::string& _strName)
{
	for (auto& it : m_vecChildren)
	{
		if (it->m_strName == _strName)
			return it;
	}

	return nullptr;
}

RenderLib::BaseNode* RenderLib::BaseNode::getChild(size_t _index)
{
	if (_index >= m_vecChildren.size())
		return nullptr;

	return m_vecChildren[_index];
}

void RenderLib::BaseNode::clearChildren()
{
	m_vecChildren.clear();
}

std::vector<DirectX::XMFLOAT3>  RenderLib::BaseNode::sm_vecCubeVertices = {
	{-1, -1, -1}, {1, -1, -1, }, {1, 1, -1}, {-1, 1, -1},
	{-1, -1, 1}, {1, -1, 1 }, {1, 1, 1}, {-1, 1, 1}
};

std::vector<uint16_t> RenderLib::BaseNode::sm_vecCubeIndices =
{ 0, 1,  1, 2,  2, 3,  3,0,   // side 1

4, 5,  5, 6,  6, 7,  7,4,   // side 1

0, 4,   1,5,   2,6,  3, 7 };  // connecting lines

struct vert
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
};

const std::vector<vert> g_quad =
{
	{
		-1, 0, -1,
		0.f, 0.f,
		0.f, 1.f, 0.f
	},

	{
		1, 0, -1,
		1.f, 0.f,
		0.f, 1.f, 0.f
	},

	{
		1, 0, 1,
		1.f, 1.f,
		0.f, 1.f, 0.f
	},

	{
		-1, 0, 1,
		0.f, 1.f,
		0.f, 1.f, 0.f
	}
};

const string& RenderLib::NodeMetaData::getName()
{
	return m_strName;
}

void RenderLib::NodeMetaData::setName(const string& _strName)
{
	m_strName = _strName;
}