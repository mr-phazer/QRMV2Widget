#pragma once

#include "IDrawableNode.h"

namespace RenderLib
{
	// How to draw a node: Shaders, rasterizer, (materials?), render target(s), etc
	class INodeRenderer
	{
	public:
		virtual void drawNode(Scene* _poScene, BaseNode* _poNode) = 0;
		virtual bool init(Scene* _poScene) = 0;
	};

	class StandardNodeRender : public INodeRenderer
	{
	public:
		virtual void drawNode(Scene* _poScene, BaseNode* _poNode) override;
		virtual bool init(Scene* _poScene) { return true; };
	};

	class SSAO_PrePass_Renderer : public INodeRenderer
	{
	public:
		virtual void drawNode(Scene* _poScene, BaseNode* _poNode) override;
		virtual bool init(Scene* _poScene) { return true; };

		RenderTarget2D* poNormalAndZBuffer;
		Shader::Shader* poSSAOPrePassShader;
	};

	class RendererQueue
	{
	public:
		explicit RendererQueue() {};
		explicit RendererQueue(const RendererQueue&) {};

		StandardNodeRender oNodeRenderer;
		//void drawAll(Scene* _poScene); // draws queue "normally", using the assigned technique IDs -> Render Technique Objects
		void drawAll(Scene* _poScene, INodeRenderer* _poNodeRenderer); // draws queue using '_poTech

		// used from within "BaseNode" when "BaseNode::process(Renderer*)" is called during recursive tree traversing
		void clear();
		void addNode(BaseNode* _po);

	protected:
		vector<BaseNode*> m_vecDrawableNodeQueue; // contains nodes to be drawn
	};
	/*
	 how to use the RenderQueue, for example,
	 for example:
	 - do SSAO pre pass: RenderQueue::drawAll(poSSAOPrePassNodeRenderer);
	 - do light scene pass: RenderQueue::drawAll(poStandardLightingNodeRenderer);
	 */
	class IRenderTechnique
	{
	public:
		virtual void renderScene(Scene* _poScene, RendererQueue* _poRenderQueue) = 0;
		virtual void reset(UINT _width, UINT _height, Scene* _poScene) = 0;
		virtual void init(UINT _width, UINT _height, Scene* _poScene) = 0;
	};

	class SSAO_RenderTechnique : public IRenderTechnique
	{
	public:
		virtual void renderScene(Scene* _poScene, RendererQueue* _poRenderQueue) override;

		virtual void reset(UINT _width, UINT _height, Scene* _poScene) override;
		virtual void init(UINT _width, UINT _height, Scene* _poScene) override;

		RendererQueue oRenderQueue;

		SSAO_PrePass_Renderer oSSAO_Node_Renderer;

		RenderTarget2D oNormalAndZBuffer;
		std::shared_ptr<Shader::Shader> spoSSAOPrePassShader;
	};
};
