#pragma once
#include "MeshNode.h"

namespace RenderLib
{
	class QuadNode :
		public MeshNode
	{
	public:
		static QuadNode* create(Scene* _poScene, const std::string& _strName);
		bool reGenerate(XMFLOAT3 center, float _fSideLength);
	};
}
