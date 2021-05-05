#pragma once

#include <d3d11.h>
#include <vector>
#include "Shader.h"

namespace RenderLib
{
	class Scene;

	class RenderTarget;
	class RendererQueue;

	struct RenderParam
	{
		RenderParam(Scene* _poScene, Shader::Shader* _poShader = nullptr, RenderTarget* _poTarget = nullptr)
			: poScene(_poScene), poShader(_poShader), poTarget(_poTarget) {};

		RenderParam(Scene* _poScene, RenderTarget* _poTarget)
			: poScene(_poScene), poShader(nullptr), poTarget(_poTarget) {};

		Scene* poScene;
		Shader::Shader* poShader;
		RenderTarget* poTarget;
	};

	enum ENodeType : uint32_t
	{
		eAbstract,
		eBase,
		eMesh,
		eSkeleleton
	};

	class Scene;
	class BaseNode;
	class IAbstractNode
	{
	public:
		/*virtual void render(Scene* _poScene) = 0;

		virtual void draw(Scene* _poScene) = 0;*/

		//
	public:
		virtual void draw(RenderLib::Scene* _poScene, Shader::Shader* _poShader) {};

		virtual void draw(ID3D11DeviceContext* _poContext, ID3D11RenderTargetView* _poRTV = NULL) {};

		virtual void render(ID3D11DeviceContext* _poContext, ID3D11RenderTargetView* _poRTV = NULL) {};

		virtual void process(RenderLib::RendererQueue* _poRenderer) = 0;
		//		virtual void updateGlobalMatrix() = 0;
			//	virtual void updateMatrices() = 0;
		//		virtual bool init(Scene* _poScene) { return true; };

			//	virtual void addChild(BaseNode*) = 0;
			//	virtual void clearChildren() = 0;

			//private:
			//	ENodeType m_NodeType = ENodeType::eAbstract;
	};
};
