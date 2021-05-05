#pragma once
#include "MeshNode.h"
#include "MeshData.h"

namespace RenderLib
{
	class TerrainMeshNode :
		public MeshNode
	{
		static TerrainMeshNode* create(RenderLib::Scene* _poScene, const std::string& _strName);
		bool generate(RenderLib::Scene* _poScene);

		std::shared_ptr<SubMeshData> m_spoGridMeshData;
	};
}
