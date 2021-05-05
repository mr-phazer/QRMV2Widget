#include "GridNode.h"

RenderLib::GridNode* RenderLib::GridNode::create(Scene* _poScene, const std::string& _strName)
{
	auto po = new GridNode(_strName);

	_poScene->addObject(po);
	po->init(_poScene);

	return po;
}

bool RenderLib::GridNode::generate(RenderLib::Scene* _poScene)
{
	m_spoGridMeshData = shared_ptr<SubMeshData>(new SubMeshData);

	vector <ModelVertex> vecVertex;
	vector<unsigned short> vecIndex;

	unsigned short index = 0;
	for (int x = -grid_lines / 2; x < (grid_lines / 2) + 1; x++)
	{
		ModelVertex v1;
		//v1.position = { x * spacing, 0, -( spacing * ( grid_lines - 1 ) ) / 2   , 1 };
		v1.position = { x * spacing, 0, -(spacing * (grid_lines - 1)) / 2 };

		float c = 0.5f * (x + ((float)grid_lines / 2.f)) / (float)grid_lines;
		c = 0.2f;
		v1.color = { c, c, c, 1 };

		ModelVertex v2;
		//v2.position = { x * spacing, 0, ( spacing * ( grid_lines - 1 ) ) / 2    , 1 };
		v2.position = { x * spacing, 0, (spacing * (grid_lines - 1)) / 2 };

		c = (x + ((float)grid_lines / 2.f)) / (float)grid_lines;
		c = 0.2;
		v2.color = { c, c, c, 1 };

		vecVertex.push_back(v1);
		vecIndex.push_back(index++);
		vecVertex.push_back(v2);
		vecIndex.push_back(index++);
	}

	for (int z = -grid_lines / 2; z < (grid_lines / 2) + 1; z++)
	{
		ModelVertex v1;
		v1.position = {
			(spacing * (grid_lines - 1)) / 2,
			0,
			z * spacing
		};

		float c = 0.5f * (z + ((float)grid_lines / 2.f)) / (float)grid_lines;
		c = 0.2f;
		v1.color = { c, c, c, 1 };

		ModelVertex v2;
		v2.position = {
			-(spacing * (grid_lines - 1)) / 2,
			0,
			z * spacing
		};

		c = (z + ((float)grid_lines / 2.f)) / (float)grid_lines;
		c = 0.2f;
		v2.color = { c, c, c, 1 };

		vecVertex.push_back(v1);
		vecIndex.push_back(index++);
		vecVertex.push_back(v2);
		vecIndex.push_back(index++);
	}

	m_spoGridMeshData->createVertexBuffer(_poScene->getDevice(), vecVertex.size() * sizeof(ModelVertex), &vecVertex[0]);
	m_spoGridMeshData->createIndexBuffer(_poScene->getDevice(), vecIndex.size() * 2, &vecIndex[0]);

	m_spoGridMeshData->m_index_count = vecIndex.size();

	m_spoGridMeshData->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	m_spoGridMeshData->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	m_spoGridMeshData->m_poPixelShader =
		_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eGrid);

	m_spoGridMeshData->m_poVertexShader =
		_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);

	m_spoGridMeshData->m_poInputLayout =
		_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);

	return true;
}

bool RenderLib::GridNode::init(Scene* _poScene)
{
	return IDrawableNode::init(_poScene);
}