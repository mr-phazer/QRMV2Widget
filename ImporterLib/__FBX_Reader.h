#pragma once

#pragma once
//#include <Definitions.h>

//#include "..\EASY_GUI\log.h"
#include <fstream>
#include <list>

#include <fbxsdk.h>
#include <sstream> 

//#include "..\FBX_EXPORTER\fbx_exporter.h"

//#include "..\D3D11_RENDER_LIB\OBJ_Loader.h"
//#include "anim_file.h"
//#include "IAnimExporter.h"
#include "..\SystemLib\system_clock.h"
#include "..\meshopt\meshoptimizer.h"
#include "fbx_geometry.h"
#include "FBX_Mesh.h"
#include "..\QtTestRMV2\QConsole.h"
#include "..\ImporterLib\bone_table.h"


class FBX_Reader
{
public:
	bool load(const std::string& strFileName, const std::string& strSkletonID = "", const std::string& strSkeletonPath = "");
	
	std::vector<FBX_Mesh>& meshes();
	std::vector<FBX_Mesh*>& meshesPtr();

	
protected:
	bool initFBX(const std::string& strFileName);
	bool destroyFBX();
	bool readMesh();

	bool findSubMeshes(fbxsdk::FbxNode* _poNode);
	bool findSubMeshesRecursive(fbxsdk::FbxNode* _poNode);	
	
	bool sortLods();
	
	bool readNodeTransformations(fbxsdk::FbxMesh* _poMeshNode);

protected:
	bool rollOutSubMesh(fbxsdk::FbxMesh* _po);	
	bool readItems(fbxsdk::FbxMesh* _po);	
	bool readBoundingBox(fbxsdk::FbxMesh* _po);
	bool readUVs(fbxsdk::FbxMesh* _po);
	bool readVectors(fbxsdk::FbxMesh* _po);


	bool readVertices(fbxsdk::FbxMesh* _po);
	bool readVerticesTriangles(fbxsdk::FbxMesh* _po);
	bool readIndices(fbxsdk::FbxMesh* _po);	
	bool readIndicesUnindexed(fbxsdk::FbxMesh* _po);	
	bool readWeights(fbxsdk::FbxMesh* _po);	
	bool readControlPointWeights(fbxsdk::FbxMesh* _po);

	void readSKeleton(fbxsdk::FbxNode * _poBone);
	void processSkeleteton();

	bool cleanUpMesh(FBX_Mesh& mesh);
	

	string strSkeletonID = "";
	string m_strSource_FBX_File = "";
	string m_strDest_RMV2_File = "";

	map<string, FBX_Mesh> mapMeshes;


	std::vector<FBX_Mesh> m_vecMeshes;
		
	vector<vector<FBX_Mesh>> vecMeshesSorted;
	
	vector<FbxNode*> vecBones;
	map<string, string> mapBoneNameToParentBoneName;
	map<string, fbxsdk::FbxNode*> mapBoneNameToFbxNode;
	
	map<int, string> mapBonidToName;
	map<string, int> mapBoneNameToId;
	map<string, pair<int, int>> mapBoneNameToIdAndParent;
	
	vector<list<string>> veclistParentIdSearchOrder;
	
	
	vector<pair<string, int>> vecNameParentId;



	size_t current_bone_id = 0;
private:
	bool m_bRecursiveMeshSearch = false;
	bool m_bNotRigged = false;
	bool m_bHasOwnLods = false;


	string m_strAnimSkeletonFile;
	FBX_Mesh m_oTemp;
	SystemLib::system_clock m_clock;

	//vector<FbxQuaternion> m_vecNormals;
	//vector<FbxQuaternion> m_vecBiNormals;
	//vector<FbxQuaternion> m_vecTangents;	

	map<string, vector<fbxsdk::FbxVector2>> m_mapvecUVMaps;


	std::vector<FBX_Mesh*> m_vecpoMeshesPtrTemp;
public:
	ImporterLib::bone_table m_oBoneTable;



private:
	vector< fbxsdk::FbxMesh*> m_vecMeshNodePtrs; // submeshes
	vector< fbxsdk::FbxSkeleton*> m_vecSkeletonPtrs; // skeleton
	int m_mesh_index = -1;

	fbxsdk::FbxManager* m_pSdkManager = nullptr;
	fbxsdk::FbxScene* m_pScene = nullptr;
	fbxsdk::FbxImporter* m_pImporter = nullptr;
	//fbxsdk::FbxMesh* m_poCurrentMesh = nullptr;


	bool calculateTagentData();
};




