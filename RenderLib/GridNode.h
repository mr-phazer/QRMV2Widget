#pragma once
#include "MeshData.h"
#include "IDrawableNode.h"

namespace RenderLib
{
	class GridNode :
		public IDrawableNode
	{
	public:
		GridNode(const string& _strName) : IDrawableNode(_strName)
		{
		};

		static GridNode* create(RenderLib::Scene* _poScene, const std::string& _strName);
		bool generate(RenderLib::Scene* _poScene);

		virtual void draw(RenderLib::Scene* _poScene, Shader::Shader* _poShader = nullptr) override
		{
			m_spoGridMeshData->drawMesh(_poScene);
		}
	protected:
		virtual bool init(Scene* _poScene);

		int  grid_lines = 100;
		float spacing = 0.19;

		std::shared_ptr<SubMeshData> m_spoGridMeshData;
	};
};
