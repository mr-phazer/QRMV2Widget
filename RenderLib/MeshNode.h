#pragma once

#include "MeshData.h"
#include "IDrawableNode.h"

#include "..\QtRME_GUI\render_resources.h"
#include "Shader.h"

using namespace std;

namespace RenderLib
{
	class MeshNode :
		public IDrawableNode
	{
	protected:
		MeshNode() {};
		MeshNode(const string& _strName) : IDrawableNode(_strName) {};
	public:
		virtual ~MeshNode() {
			int debug_1 = 1;
		}

		static MeshNode* create(Scene* _poScene, const std::string& _strName);
		virtual bool init(Scene* _poScene) override;

		MeshLodData* meshes()
		{
			return m_spoMeshLodData.get();
		}

		void setShowBoundBox(bool _bState);

		virtual void draw(Scene* _poScene, Shader::Shader* _poShader = nullptr) override;

		bool loadIBLMapsFromResource(const RenderLib::Scene* _poScene, UINT _irr, UINT _rad, UINT _lut);
		void setEnvMaps(ID3D11DeviceContext* m_pDeviceContext);

		bool allocateBoundingBoxes(Scene* _poScene);

		void setLOD(size_t);
		size_t getLOD();

		vector<shared_ptr<RenderLib::SubMeshData>> m_vecBoundingBoxMeshes;

		void setLodMeshData(shared_ptr < MeshLodData > _ptr)
		{
			m_spoMeshLodData = _ptr;
		}

	protected:
		shared_ptr < MeshLodData > m_spoMeshLodData;

		//vector<shared_ptr<MeshLodData>> m_spoMeshLodData;

		size_t m_lod = 0;
		bool m_bShowBoundingBox = true;

	protected:
		ID3D11ShaderResourceView* m_poSRV_CubeMap_Diffuse = nullptr;
		ID3D11ShaderResourceView* m_poSRV_CubeMap_Specular = nullptr;
		ID3D11ShaderResourceView* m_poSRV_LookUpTexture = nullptr;
	};
}