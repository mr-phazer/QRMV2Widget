#include "RootNode.h"

void RenderLib::RootNode::setCamera(const Camera& _oCamera)
{
	m_oCamera = _oCamera;
}

Camera& RenderLib::RootNode::getCamara()
{
	 return m_oCamera; 
}
