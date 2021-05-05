#pragma once

#include "pch.h"

// ---- d3d 11 includes -----
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

#include <string>
#include <vector>

#include "Scene.h"
#include "MeshData.h"
#include "IAbstractNode.h"

#include "..\RenderLib\render_const_bufers.h"

using namespace std;
// ---- authour includes -----
//#include "MeshData.h"
//#include "..\SystemLib\tree_node.h"

namespace RenderLib
{
	class NodeMetaData
	{
	public:
		const string& getName();
		void setName(const string& _strName);

	protected:
		string m_strName = "no name";
	};

	class BaseNode : public NodeMetaData, public IAbstractNode
	{
		friend class Scene;

	protected:
		BaseNode(BaseNode* _poRoot);
		BaseNode(const string& _strName)
		{
			m_strName = _strName;
		};
		virtual bool init(Scene* _poScene);

	public:
		string m_strName = "";
		BaseNode();

		virtual ~BaseNode()
		{
		}

		static BaseNode* create(Scene* _poScene, const std::string& _strName);

		void setVisible(bool _bState = true);
		bool visible();

		virtual void render(Scene* _poScene, Shader::Shader* _poShader = nullptr);

		virtual void draw(RenderLib::Scene* _poScene, Shader::Shader* _poShader = nullptr);

		virtual void process(RenderLib::RendererQueue* _poRenderer) override;

		/*
			Tree Management
		*/
		const std::string& getName();
		BaseNode* getParent();

		RenderLib::BaseNode* addChild(BaseNode*);
		void appendChild(BaseNode* _poBaseNode);
		void insertChild(size_t index, BaseNode*);

		size_t getChildCount();
		BaseNode* findChild(const std::string& _strName);
		BaseNode* getChild(size_t _index);

		virtual void clearChildren();

		/*
			Node debugging graphics indications
		*/
		bool setCubeVisible(bool _bState = true);

		operator bool(); // makes it possible use "BaseNode objects in boolean expressions

		/*
			Tranformation opertions
		*/
		void transform(const DirectX::XMFLOAT4X4& _mMatrix);

		// translation
		void mulTranslate(const DirectX::XMFLOAT3& _vTranslation);
		void setTranslate(const DirectX::XMFLOAT3& _vTranslation);

		void setTranslateX(float x);
		void setTranslateY(float y);
		void setTranslateZ(float z);

		// rotation
		void rotateByQuaternion(const DirectX::SimpleMath::Quaternion _Orientation);

		void setRotateQuaternion(const DirectX::XMFLOAT4& _qOrienation);
		void setRotateMatrix(const DirectX::XMFLOAT4X4& _qOrienation);
		void setRotationEuler(const DirectX::XMFLOAT3& _vEulerAngles);
		void rotateEuler(const DirectX::XMFLOAT3& _vEulerAngles);

		void rotateRoll(const DirectX::XMFLOAT3& _vRoll);

		//scale
		void setScale(const DirectX::XMFLOAT3& _vScale);

		// update matrix
		virtual void updateGlobalMatrix(); // update matrix using TRS and parent local transform
		virtual void updateLocalMatrix(); // update matrix using TRS and parent local transform

		virtual void update();

		const DirectX::XMFLOAT4X4& getGlobalTransform();
		const DirectX::XMFLOAT4X4& getLocalTransform();

	protected:
		DirectX::SimpleMath::Vector3 m_vLocalTanslation = { 0,0,0 };
		DirectX::SimpleMath::Quaternion m_qLocalOrientation = DirectX::SimpleMath::Quaternion::Identity;
		DirectX::SimpleMath::Vector3 m_vLocalScale = { 0,0,0 };

		DirectX::SimpleMath::Matrix mLocalTransformation =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

	protected:
		BaseNode* m_poParent = nullptr;;
		BaseNode* poRoot = nullptr;;
		std::vector<BaseNode*> m_vecChildren;

	protected:
		bool m_bIValid = false;
		bool m_bIsCubeVisible = false;

		bool allocateCube(Scene* _poScene);
		void makeLines(Scene* _poScene, vector<DirectX::XMFLOAT4>& vertices, vector<uint16_t>& indices);
		void makeLineRecursvice(Scene* _poScene, vector<DirectX::XMFLOAT4>& vertices, vector<uint16_t>& indices);

		vector<DirectX::XMFLOAT4> m_vertices;
		vector<uint16_t> m_indices;

	public:
		virtual void drawBoundingBox(Scene* _poScene);

		virtual void __drawCube(ID3D11DeviceContext* _poContext);

		//protected:
			//SubMeshData oLines;

			//DirectX::XMMATRIX m_xmGlobalTransform = DirectX::XMMatrixIdentity();
			//DirectX::XMMATRIX m_xmLocalTransform = DirectX::XMMatrixIdentity();

			//DirectX::XMMATRIX m_xmTranslation = DirectX::XMMatrixIdentity();
			//DirectX::XMMATRIX m_xmRotation = DirectX::XMMatrixIdentity();
			//DirectX::XMMATRIX m_xmScaling = DirectX::XMMatrixIdentity();
		DirectX::SimpleMath::Matrix m_xmOrientation = DirectX::SimpleMath::Matrix::Identity;

		DirectX::SimpleMath::Matrix m_xmGlobalTransform = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix m_xmLocalTransform = DirectX::SimpleMath::Matrix::Identity;

		DirectX::SimpleMath::Matrix m_xmTranslation = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix m_xmRotation = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix m_xmScaling = DirectX::SimpleMath::Matrix::Identity;

	private:

	private:
		DirectX::XMFLOAT4X4 m_matrixTemp =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		typedef std::shared_ptr<BaseNode> spo3dObjectBase;

		template<typename TYPE>
		TYPE* getAs()
		{
			return dynamic_cast<TYPE*>(this);
		}
	public:

	public:
		shared_ptr<RenderLib::SubMeshData> m_spoCubeMesh;

		static std::vector<DirectX::XMFLOAT3>  sm_vecCubeVertices;
		static std::vector<unsigned short int> sm_vecCubeIndices;

	private:
		bool m_bVisible = false;
	};
};
