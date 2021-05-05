#pragma once
#include "MeshNode.h"

namespace RenderLib
{
	class TerrainNode :
		public MeshNode
	{
		virtual bool init(Scene*) override;
	};
}