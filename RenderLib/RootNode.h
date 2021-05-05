#pragma once
#include "BaseNode.h"
#include "Camera.h"
#include "..\SystemLib\tree_node.h"
#include <d3d11.h>

namespace RenderLib
{
    class RootNode :
        public BaseNode
    {
        RootNode();
    public:
        //static RootNode* create(ID3D11Device, const std::string& _strName);
        /// <summary>
        /// Set the camera for the entire scene
        /// </summary>
        /// <param name="_oCamera">camera to set</param>
        void setCamera(const Camera& _oCamera);
        
        Camera& getCamara();
        


    protected:
        Camera m_oCamera;
    };

}