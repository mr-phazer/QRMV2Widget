#pragma once

#include "MeshNode.h"
#include "..\ImporterLib\IRigidModelmporter.h"
#include "SKELETON.H"
#include "VariantMeshStructs.h"

namespace RenderLib
{
	/// <summary>
	/// Node for endering and animations a linear group of models
	///
	/// </summary>
	class VariantMeshNode_1 :
		public IDrawableNode
	{
		VariantMeshNode_1() {};

	public:
		static VariantMeshNode_1* create(RenderLib::Scene* _poScene, const std::string& _strName);

		void setSkeleton(Skeleton& _oSkeleton) { m_oSkeleton = _oSkeleton; }
		bool updateMeshes(Scene* _poScene, vector<const RenderLib::VariantMeshInfo*>& vecMeshInfo);

		Skeleton* skeleton() { return &m_oSkeleton; }

	protected:
		virtual void render(Scene* _poScene, Shader::Shader* _poShader = nullptr) override;
		virtual void draw(Scene* _poScene, Shader::Shader* _poShader);

		vector<shared_ptr<MeshLodData>> vecspoMeshes;

		Skeleton m_oSkeleton;
	};

	///////////////////////////////////////////////

	//class VariantMeshNode :
	//	public MeshNode
	//{
	//	VariantMeshNode() {};

	//public:
	//	static VariantMeshNode* create(RenderLib::Scene* _poScene, const std::string& _strName);

	//	void setSkeleton(Skeleton& _oSkeleton) { m_oSkeleton = _oSkeleton; }
	//	bool updateMeshes(Scene* _poScene, vector<const RenderLib::VariantMeshInfo*>& vecMeshInfo);

	//	Skeleton* skeleton() { return &m_oSkeleton; }

	//protected:
	//	virtual void render(Scene* _poScene) override;

	//	vector<shared_ptr<MeshNode>> vecspoMeshes;

	//	Skeleton m_oSkeleton;
	//};
}