#include "ResourceManager.h"
#include "MeshData.h"
#include "Scene.h"
#include "..\ImporterLib\pack_file.h"
#include "..\ImporterLib\IRigidModelmporter.h"
#include "..\ImporterLib\RigidModelV2_Reader.h"

using namespace RenderLib;
bool RenderLib::ResourceManager::destroy()
{
	m_mapStringToMeshLodData.clear(); // clears map and thus destroys all pointers
	return true;
}

std::shared_ptr<MeshLodData> ResourceManager::getMeshLodDataFromInternalPath(
	Scene* _poScene,
	const VariantMeshInfo* _poMeshInfo,
	int32_t bone_index)
{
	// is mesh resource already in memory? Then return it.

	auto it = m_mapStringToMeshLodData.find(_poMeshInfo->strModelPath);
	if (it != m_mapStringToMeshLodData.end())
	{
		return it->second;
	}

	// if not, then load it:

	// get pack file node
	pack_file_node* ppfn = pack_file::sm_poPathTree->getNodeFromInternalPath(_poMeshInfo->strModelPath);
	if (!ppfn)
		return nullptr;

	// extract model data and read by data stream

	auto pBuffer = (uint8_t*)ppfn->extractFileToMemory();

	auto RMV2_Importer = RigidModelV2::File_Importer_Common::create(pBuffer, ppfn->file_size);

	auto pRMV2_File = RMV2_Importer->getFile();

	/*****************************
	SystemLib::DataStream oDataStream(pBuffer, ppfn->file_size);

	if (!oDataStream)
		return nullptr;

	if (oDataStream.m_vecBuffer.size() == 268206)
		int i = 1;

	// make a RigidModel file in memory from the data
	auto spoRigidModelFile = ImporterLib::RigidModel::createRidigModFile(oDataStream);

	if (!spoRigidModelFile)
		return nullptr;
		***********************************************************/

		// create a "MeshLodDat" object
	std::shared_ptr<MeshLodData> spoMesh = std::shared_ptr<MeshLodData>(new MeshLodData);

	// use object to parse RMV2 data
	/*if (!spoMesh->loadFromRigidModel(_poScene, spoRigidModelFile.get(), _poMeshInfo, bone_index))
		return nullptr;
	*/
	if (!spoMesh->loadFromRigidModel_new(_poScene, pRMV2_File.get(), _poMeshInfo, bone_index))
		return nullptr;

	spoMesh->strPath = _poMeshInfo->strModelPath;

	// store MeshLodData in resource map
	m_mapStringToMeshLodData[_poMeshInfo->strModelPath] = spoMesh;

	return spoMesh;
}