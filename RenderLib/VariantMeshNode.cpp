#include "VariantMeshNode.h"

#include "..\ImporterLib\pack_file.h"
#include "..\SystemLib\system_clock.h"

using namespace RenderLib;

void VariantMeshNode_1::render(Scene* _poScene, Shader::Shader* _poShader)
{
	updateGlobalMatrix();

	m_oSkeleton.render(_poScene);

	for (shared_ptr<MeshLodData>& spo : vecspoMeshes)
	{
		spo->drawLOD(_poScene, _poShader);
	}
}

void RenderLib::VariantMeshNode_1::draw(Scene* _poScene, Shader::Shader* _poShader)
{
	m_oSkeleton.render(_poScene);

	for (shared_ptr<MeshLodData>& spo : vecspoMeshes)
	{
		spo->drawLOD(_poScene, _poShader);
	}
}

VariantMeshNode_1* RenderLib::VariantMeshNode_1::create(RenderLib::Scene* _poScene, const std::string& _strName)
{
	auto po = new VariantMeshNode_1();
	po->m_strName = _strName;
	_poScene->addObject(po); // add to "abstract pointer list" for later clean up

	return po;
}

bool RenderLib::VariantMeshNode_1::updateMeshes(Scene* _poScene, vector<const RenderLib::VariantMeshInfo*>& vecMeshInfo)
{
	// TODO: create resource management for textures and meshes, so neither are loaded twice, ever
	vecspoMeshes.clear();
	for (auto& it : vecMeshInfo)
	{
		// set all vertices rigged to 1 bone (for weapons)
		int32_t bone_index = -1;
		//if (m_poSkeleton)
		{
			bone_index = m_oSkeleton.getIdFromName(it->strAttachBoneName);
		}

		//pack_file_node* ppfn = pack_file::sm_poPathTree->getNodeFromInternalPath(it->strModelPath);
		//if (!ppfn)
		//	return false;
		//
		//SystemLib::DataStream oDataStream((void*)ppfn->extractFileToMemory(), ppfn->file_size);

		//if (!oDataStream)
		//	return false;

		//auto spoRigidModelFile = ImporterLib::RigidModel::createRidigModFile(oDataStream);

		//if (!spoRigidModelFile)
		//	return false;

		////clock.resetLocalTime();
		//std::shared_ptr<MeshLodData> spoMesh = std::shared_ptr<MeshLodData>(new MeshLodData);

		//
		//if (!spoMesh->loadFromRigidModel(_poScene, spoRigidModelFile.get(), bone_index))
		//	return false;

		auto spoMesh = _poScene->oResourceManger.getMeshLodDataFromInternalPath(_poScene, it, bone_index);

		if (!spoMesh)
		{
			continue;
		}

		// update bound box the encloses all submeshes in LOD, using min/max
		vBBMax.x = max(spoMesh->vBBMax.x, vBBMax.x);
		vBBMax.y = max(spoMesh->vBBMax.y, vBBMax.y);
		vBBMax.z = max(spoMesh->vBBMax.z, vBBMax.z);

		vBBMin.x = min(spoMesh->vBBMin.x, vBBMin.x);
		vBBMin.y = min(spoMesh->vBBMin.y, vBBMin.y);
		vBBMin.z = min(spoMesh->vBBMin.z, vBBMin.z);

		spoMesh->strPath = it->strModelPath;

		vecspoMeshes.push_back(spoMesh);
	}

	_poScene->getCamera().setToBB(vBBMin, vBBMax);

	return true;
}

//void VariantMeshNode::render(Scene* _poScene)
//{
//	updateGlobalMatrix();
//
//	m_oSkeleton.render(_poScene);
//
//	for (shared_ptr<MeshNode>& spo : vecspoMeshes)
//	{
//		spo->render(_poScene);
//	}
//}
//
//VariantMeshNode* RenderLib::VariantMeshNode::create(RenderLib::Scene* _poScene, const std::string& _strName)
//{
//	auto po = new VariantMeshNode();
//	po->m_strName = _strName;
//	_poScene->addObject(po); // add to "abstract pointer list" for later clean up
//
//	return po;
//}
//
//bool RenderLib::VariantMeshNode::updateMeshes(Scene* _poScene, vector<const RenderLib::VariantMeshInfo*>& vecMeshInfo)
//{
//	// TODO: create resource management for textures and meshes, so neither are loaded twice, ever
//	vecspoMeshes.clear();
//	for (auto& it : vecMeshInfo)
//	{
//		// set all vertices rigged to 1 bone (for weapons)
//		int32_t bone_index = -1;
//		//if (m_poSkeleton)
//		{
//			bone_index = m_oSkeleton.getIdFromName(it->strAttachBoneName);
//		}
//
//		//pack_file_node* ppfn = pack_file::sm_poPathTree->getNodeFromInternalPath(it->strModelPath);
//		//if (!ppfn)
//		//	return false;
//		//
//		//SystemLib::DataStream oDataStream((void*)ppfn->extractFileToMemory(), ppfn->file_size);
//
//		//if (!oDataStream)
//		//	return false;
//
//		//auto spoRigidModelFile = ImporterLib::RigidModel::createRidigModFile(oDataStream);
//
//		//if (!spoRigidModelFile)
//		//	return false;
//
//		////clock.resetLocalTime();
//		//std::shared_ptr<MeshLodData> spoMesh = std::shared_ptr<MeshLodData>(new MeshLodData);
//
//		//
//		//if (!spoMesh->loadFromRigidModel(_poScene, spoRigidModelFile.get(), bone_index))
//		//	return false;
//
//		auto spoMesh = _poScene->oResourceManger.getMeshLodDataFromInternalPath(_poScene, it, bone_index);
//
//		spoMesh->strPath = it->strModelPath;
//
//		MeshNode* po = MeshNode::create(_poScene, "");
//
//		po->setLodMeshData(spoMesh);
//
//		vecspoMeshes.push_back(shared_ptr <MeshNode>(po));
//	}
//
//	return true;
//}