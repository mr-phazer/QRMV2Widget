#pragma once

#pragma once
//#include <Definitions.h>
//#include "amim_structs.h"
#include "mesh_structs.h"
#include "..\QtRMV2Widget\QConsole.h"0
#include <fstream>
//#include "..\FBX_EXPORTER\fbx_exporter.h"
//#include "..\D3D11_RENDER_LIB\OBJ_Loader.h"
//#include "anim_file.h"
#include "..\SystemLib\system_clock.h"
#include "..\meshopt\meshoptimizer.h" 

#include "fbx_geometry.h"
#include "RigidModelFileStructs.h"


struct FBX_Mesh
{
	friend class FBX_Reader;

public:
	string m_strName = "empty mesh";
	vector<pair <TextureType::EValues, string>> vecpairTextures;



	vector<RMV2_Vertex_Common> vecPolygonVertices;	
	vector<RMV2_Vertex_Common_RAW> vecVertices_RAW;	
	vector<uint32_t> vecIndices32; // FBX slang for indices, more or less
	
	vector<vector<pair<int, float>>> vecvecControlPointInfluences;

	bool doCleanUp();
	bool calculateBoundingBox_RAW();
	bool simplify(float _threshold, float _target_error = 1e-3f, bool _bUseSloppyIfStuck = false);
	bool simplify_slopppy(float _threshold, float _target_error = 1e-3f);

	
	operator const vector<uint16_t>& ()
	{
		m_vecIndices16.clear();
		m_vecIndices16.resize(vecIndices32.size());

		for (size_t i = 0; i < vecIndices32.size(); i++)
		{
			m_vecIndices16[i] = vecIndices32[i];
		}
			
		return m_vecIndices16;
	}

	// [vertices][influences]
	vector<vector<pair<int, float>>> vecvecInfluences;

	XMFLOAT3 vMin = { 0,0,0 };
	XMFLOAT3 vMax = { 0,0,0 };
	
	XMFLOAT3 vPivotPoint = { 0,0,0 };

	


//protected:
	size_t polygon_vertex_count = 0;
	//size_t comn_count = 0;

	size_t vertex_count = 0;
	size_t index_count = 0;

	
	uint32_t vertex_format_value = ImporterLib::RigidModel::RigidMaterial::default_material;
		//vector<pair<int, float>> vecpairInflucnes;
	bool m_bNegativeScaling = false; 

private:
	fbxsdk::FbxAMatrix transform_matrix;
	fbxsdk::FbxAMatrix normal_transformation_matrix;

	vector<uint16_t> m_vecIndices16; // FBX slang for indices, more or less

	
};


struct FBX_Meshes
{
	vector<FBX_Mesh> m_vecMeshes;

	operator vector<FBX_Mesh>& () { return m_vecMeshes; };
	operator vector<FBX_Mesh> () { return m_vecMeshes; };

};