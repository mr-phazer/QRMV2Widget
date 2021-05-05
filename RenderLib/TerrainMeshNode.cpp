#include "TerrainMeshNode.h"

bool RenderLib::TerrainMeshNode::generate(RenderLib::Scene* _poScene)
{
	vector <ModelVertex> vecVertex;
	vector<unsigned short> vecIndex;

	float width = 1.0;

	SimpleMath::Vector3 p1 = { 0.f, -width, -width };
	SimpleMath::Vector3 p4 = { 0.f, width, -width };
	SimpleMath::Vector3 p5 = { 0.f, width, width };
	SimpleMath::Vector3 p6 = { 0.f, -width, width };

	for (size_t y = 0; y < 100; y++)
	{
		for (size_t x = 0; x < 100; x++)
		{
			ModelVertex p;

			p.position.x = width / 2 * x - width;
			p.position.y = 0;
			p.position.z = width / 2 * y - width;;

			vecVertex.push_back(p);
		}
	}

	//UINT k = 0;
	//for (UINT i = 0; i < m - 1; ++i)
	//{
	//	for (UINT j = 0; j < n - 1; ++j)
	//	{
	//		m_Mesh.m_Indices[k] = i * n + j;
	//		m_Mesh.m_Indices[k + 1] = i * n + j + 1;
	//		m_Mesh.m_Indices[k + 2] = (i + 1) * n + j;

	//		m_Mesh.m_Indices[k + 3] = (i + 1) * n + j;
	//		m_Mesh.m_Indices[k + 4] = i * n + j + 1;
	//		m_Mesh.m_Indices[k + 5] = (i + 1) * n + j + 1;

	//		k += 6; // next quad
	//	}
	//}

	m_spoGridMeshData->createVertexBuffer(_poScene->getDevice(), vecVertex.size() * sizeof(ModelVertex), &vecVertex[0]);
	m_spoGridMeshData->createIndexBuffer(_poScene->getDevice(), vecIndex.size() * 2, &vecIndex[0]);

	m_spoGridMeshData->setIndexCount(vecIndex.size());

	m_spoGridMeshData->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	m_spoGridMeshData->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	m_spoGridMeshData->m_poPixelShader =
		_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eGrid);

	m_spoGridMeshData->m_poVertexShader =
		_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid);

	m_spoGridMeshData->m_poInputLayout =
		_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid);

	return true;

	return true;
}