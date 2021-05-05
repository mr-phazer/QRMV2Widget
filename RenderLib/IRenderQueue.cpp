#include "IRenderQueue.h"
#include "vertex_layouts.h"

void RenderLib::RendererQueue::drawAll(Scene* _poScene, INodeRenderer* _poNodeRenderer)
{
	for (auto& itNode : m_vecDrawableNodeQueue)
	{
		_poNodeRenderer->drawNode(_poScene, itNode);
	}

	clear();
}

void RenderLib::RendererQueue::clear()
{
	m_vecDrawableNodeQueue.clear();
}

void RenderLib::RendererQueue::addNode(BaseNode* _po)
{
	m_vecDrawableNodeQueue.push_back(_po);
}

void RenderLib::StandardNodeRender::drawNode(Scene* _poScene, BaseNode* _poNode)
{
	_poNode->updateGlobalMatrix();
	//XMStoreFloat4x4(&_poScene->m_VS_PerModel_ConstantBuffer.world, XMMatrixTranspose(m_xmGlobalTransform));
	_poScene->m_VS_PerModel_ConstantBuffer.mWorld = (_poNode->m_xmGlobalTransform).Transpose();
	_poScene->m_VS_PerModel_ConstantBuffer.mProjection =
		_poScene->getCamera().ProjectMatrixPerspective;

	_poScene->updateVSPerModelConstBuffer();

	_poNode->draw(_poScene);
}

void RenderLib::SSAO_PrePass_Renderer::drawNode(Scene* _poScene, BaseNode* _poNode)
{
	auto& o = _poScene->m_VS_PerModel_ConstantBuffer;

	_poNode->updateGlobalMatrix();
	//XMStoreFloat4x4(&_poScene->m_VS_PerModel_ConstantBuffer.world, XMMatrixTranspose(m_xmGlobalTransform));
	_poScene->m_VS_PerModel_ConstantBuffer.mWorld = (_poNode->m_xmGlobalTransform).Transpose();
	_poScene->m_VS_PerModel_ConstantBuffer.mProjection = _poScene->getCamera().ProjectMatrixPerspective;

	_poScene->getCamera().calculateMatrices(1024, 1024);
	_poScene->m_VS_PerModel_ConstantBuffer.mView = _poScene->getCamera().ViewMatrix;;

	o.mWorldViewProj = o.mWorld * o.mView * o.mProjection;
	o.mWorldInvTransposeView = o.mWorld.Invert() * o.mView.Transpose();

	_poScene->updateVSPerModelConstBuffer();

	_poScene->getDeviceContext()->VSSetShader(poSSAOPrePassShader->getVertexShader(), NULL, 0);
	_poScene->getDeviceContext()->IASetInputLayout(poSSAOPrePassShader->getInputLayout());
	_poScene->getDeviceContext()->PSSetShader(poSSAOPrePassShader->getPixelShader(), NULL, 0);

	auto const poTarget = poNormalAndZBuffer->getRenderTargetView();
	_poScene->getDeviceContext()->OMSetRenderTargets(1, &poTarget, NULL);

	_poNode->draw(_poScene, (Shader::Shader*)(1));
}

void RenderLib::SSAO_RenderTechnique::renderScene(Scene* _poScene, RendererQueue* _poRenderQueue)
{
	oSSAO_Node_Renderer.poNormalAndZBuffer = &this->oNormalAndZBuffer;
	oSSAO_Node_Renderer.poSSAOPrePassShader = spoSSAOPrePassShader.get();

	_poRenderQueue->drawAll(_poScene, &this->oSSAO_Node_Renderer);
}

void RenderLib::SSAO_RenderTechnique::reset(UINT _width, UINT _height, Scene* _poScene)
{
	oNormalAndZBuffer.init(_poScene, _width, _height);
}

void RenderLib::SSAO_RenderTechnique::init(UINT _width, UINT _height, Scene* _poScene)
{
	oNormalAndZBuffer.init(_poScene, _width, _height);

	spoSSAOPrePassShader = RenderLib::Shader::Shader::create(
		_poScene->getDevice(),
		layout_Weighted4,
		_countof(layout_Weighted4),
		LR"(..\RenderLib\shaders\VS_SSAO.cso)",
		LR"(..\RenderLib\shaders\PS_SSAO.cso)"
	);

	int debug_1 = 1;
}