#include "FBX_Reader.h"
#include "..\DirectXMesh\DirectXMesh.h"
#include "..\ImporterLib\IAnimExporter.h";

using namespace fbxsdk;

bool FBX_Reader::initFBX(const std::string& strFileName)
{	
	m_pSdkManager = fbxsdk::FbxManager::Create();
	
	_log_action("Iinitializing AutoDesk FBX SDK importer: " + string(m_pSdkManager->GetVersion()));
	
	// create an IOSettings object
	fbxsdk::FbxIOSettings* ios = fbxsdk::FbxIOSettings::Create(m_pSdkManager, IOSROOT);

	// set some IOSettings options
	
	ios->SetBoolProp(IMP_FBX_PIVOT , true);
	ios->SetBoolProp(IMP_FBX_MATERIAL, true);
	//ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	ios->SetBoolProp(IMP_FBX_SHAPE, true);
	ios->SetBoolProp(IMP_FBX_GOBO, true);
	ios->SetBoolProp(IMP_FBX_ANIMATION, true);
	ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	// create an empty scene
	m_pScene = fbxsdk::FbxScene::Create(m_pSdkManager, "");

	// Create an importer.
	FbxImporter* m_lImporter = fbxsdk::FbxImporter::Create(m_pSdkManager, "");

	// Initialize the importer by providing a filename and the IOSettings to use
	
	m_lImporter->Initialize(strFileName.c_str(), -1, ios);
	
	int x, y, z;
	m_lImporter->GetFileVersion(x, y, z);
	
	string str = string("File: ") + string(m_lImporter->GetFileName()) + " is using FBX version " +
		to_string(x) + "." +
		to_string(y) + "." +
		to_string(z);

	_log_action(str);

	// Import the s	cene.
	bool bResult = m_lImporter->Import(m_pScene);
	if (!bResult)
	{
		//// import failed ////

		//window::_getOKErrorBox("Error loading FBX file: " + strFileName);

		return false;

	}
	SystemLib::system_clock clock;
	clock.resetLocalTime();


	fbxsdk::FbxGeometryConverter oGeometryConverter(m_pSdkManager);
	_log_action("Triangulating....");
	bool bTri = oGeometryConverter.Triangulate(m_pScene, true, true);
	_log_action_success("Triangulating. Done.");


	/////////////////////////////// AXES //////////////////////////////
	/*fbxsdk::FbxAxisSystem m_oAxisSystem = fbxsdk::FbxAxisSystem::DirectX;
	m_oAxisSystem.DeepConvertScene(m_pScene);*/

	//m_oAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	//int lUpVectorSign = 1;
	//int lFrontVectorSign = 1;
	//fbxsdk::FbxAxisSystem::EUpVector lUpVector = m_oAxisSystem.GetUpVector(lUpVectorSign);
	////get FrontVector and its sign.
	//fbxsdk::FbxAxisSystem::EFrontVector lFrontVector = m_oAxisSystem.GetFrontVector(lFrontVectorSign);
	////get uCoorSystem. 
	//fbxsdk::FbxAxisSystem::ECoordSystem lCoorSystem = m_oAxisSystem.GetCoorSystem();
	////The FbxAxisSystem object to reconstruct back by saved parameter

	///*if (lCoorSystem == fbxsdk::FbxAxisSystem::ECoordSystem::eLeftHanded)
	//	lCoorSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;
	//else
	//	lCoorSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eLeftHanded;*/

	//fbxsdk::FbxAxisSystem oNewAxis(lUpVector, lFrontVector, lCoorSystem);
	//oNewAxis.DeepConvertScene(m_pScene);
	//
	//
	////////////////////////////// CONVERT to different Coord Sys ////////////////////








	//fbxsdk::FbxGeometryConverter oGeometryConverter(m_pSdkManager);

	//string strMesh = "Triangulation took: " + to_string(clock.getLocalTime()) + " seconds.";
	//MessageBox(NULL, strMesh.c_str(), "Done!", MB_OK);

	///////////////////////////// AXES //////////////////////////////
	/*fbxsdk::FbxAxisSystem oAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	fbxsdk::FbxAxisSystem::ECoordSystem enumCordSystem = oAxisSystem.GetCoorSystem();
	int dir;
	fbxsdk::FbxAxisSystem::EUpVector enumUpVector = oAxisSystem.GetUpVector(dir);*/

	//////////////////////////// CONVERT to different Coord Sys ////////////////////
	
	
	
	//Debug code
	_log_action_success("Deep converting scene to 'DirectX' coord system...");
	//----------------------------------------------------------------------------
	fbxsdk::FbxAxisSystem oAxis(fbxsdk::FbxAxisSystem::DirectX);
	oAxis.DeepConvertScene(m_pScene);
	//----------------------------------------------------------------------------

	 // The file is imported; so get rid of the importer.
	
	m_lImporter->Destroy();
	_log_action_success("Iinitializing FBX SDK importer...");
	return true;
}

bool FBX_Reader::destroyFBX()
{

	m_pSdkManager->Destroy();
	return true;
}

bool FBX_Reader::readMesh()
{
	fbxsdk::FbxNode* poRootNode = m_pScene->GetRootNode();
	
	if (m_bRecursiveMeshSearch)
		findSubMeshesRecursive(poRootNode);
	else 
		findSubMeshes(poRootNode);





	for (m_mesh_index = 0; m_mesh_index < m_vecMeshNodePtrs.size(); m_mesh_index++)
	{
		
		rollOutSubMesh(m_vecMeshNodePtrs[m_mesh_index]);

		readItems(m_vecMeshNodePtrs[m_mesh_index]);
		
	}

	if (m_vecSkeletonPtrs.size())
		readSKeleton(m_vecSkeletonPtrs[0]->GetNode());

	processSkeleteton();

	if (m_bHasOwnLods)
		sortLods();

	return true;
}

bool FBX_Reader::load(const std::string& _strFBXFileName, const std::string& _strSkeletonID, const std::string& _strSkeletonPath)
{	
	/*
		-- Algorithm: --
		Load .ANIM skeleton and make a map that associates bone NAMES to bone IDs
		so one can look up the numerical ID of a certain bone name.

		Load FBX Model.
		process each control point (vertex)
		check the SKIN (FbxSkin)
		for each FbxCluster (1 cluster = 1 bone)
		{
			check what "bone node" it is a controlled by and store this information as "bone id", get the ID "from the bone name -> bone ID table",
			check what "vertices" the cluster is affecting and with what "weight"
			store in the vertex: the "incluence pair" {bone_id, weight}
		}
		Do the normal remaining model stuff where convenient

		Done.

	*/
	
	// load skeleton / animation from disk to get the bone_name to bone_id map
	// TODO: this should be a param
	//string strSkeleton = _strSkeletonID + ".anim";	
	//strSkeleton = R"(E:\modding\WH2\vanilla\animations\skeletons\)" + strSkeleton;
	if (_strSkeletonPath != "") 
	m_oBoneTable.getBonesFrom_ANIM_FILE(_strSkeletonPath);
	else
	{
		if (_strSkeletonID == "")
			m_bNotRigged = true;

	}
	// reset the sysetm clock counter
	m_clock.resetLocalTime();

	// initalize FBX
	initFBX(_strFBXFileName);
	
	// read mesh
	readMesh();

	// kill FBX
	destroyFBX();

	
	return true;
}

std::vector<FBX_Mesh>& FBX_Reader::meshes()
{
	return m_vecMeshes;
}

std::vector<FBX_Mesh*>& FBX_Reader::meshesPtr()
{
	m_vecpoMeshesPtrTemp.resize(m_vecMeshes.size());
	for (size_t i = 0; i < m_vecMeshes.size(); i++)
	{
		m_vecpoMeshesPtrTemp[i] = &m_vecMeshes[i];

	}

	return m_vecpoMeshesPtrTemp;
}


bool FBX_Reader::rollOutSubMesh(fbxsdk::FbxMesh* _po)
{
	
	// get the new mesh node
	//m_vecMeshNodePtrs[m_mesh_index] = poMeshAttribute->GetNode()->GetMesh();
	 
	
	/*int vertices_before_rollout = m_vecMeshNodePtrs[m_mesh_index]->GetControlPointsCount();

	bool bConvertResult = oGeometryConverter.EmulateNormalsByPolygonVertex(_po);
	int vertices_after_rollout = m_vecMeshNodePtrs[m_mesh_index]->GetControlPointsCount();


	if (!bConvertResult)
		return _log_action_error("Converter vertex storage to 'by vertex' ('unindexing')...");

	_log_action_success("Converter vertex storage to 'by vertex' ('unindexing')...");
	*/
	
	return true;
}

bool FBX_Reader::readItems(fbxsdk::FbxMesh* _poMeshNode)
{
	if (!_poMeshNode->GetNode()->GetVisibility());
	// set submesh name
	m_vecMeshes[m_mesh_index].m_strName = _poMeshNode->GetNode()->GetName();
	_log_write("---------------------------------------");
	_log_write("Processing Sub Mesh: '" + m_vecMeshes[m_mesh_index].m_strName + "'");
	_log_write("---------------------------------------");


	// stuff to do before vertex stuff
	readNodeTransformations(_poMeshNode);
	readBoundingBox(_poMeshNode);

	readUVs(_poMeshNode);	

	// actual mesh
	
#pragma region code for convert to byControlPoint	
	//readVertices(_poMeshNode);
	readIndices(_poMeshNode);
#pragma  endregion

#pragma region TEST_CODE
	readControlPointWeights(_poMeshNode);	
	readVerticesTriangles(_poMeshNode);		
#pragma TEST_CODE endregion
	
	// TODO: fix this so it makes proper tangent data for verticex that are 
	// vertex1.x = -vertex2.x;
	// vertex1.UV = vertex2.UV;
	
	//calculateTagentData(); 

	// rigging
	//readWeights(_poMeshNode);
		
	return true;
}

bool FBX_Reader::readBoundingBox(fbxsdk::FbxMesh* _poMeshNode)
{
	//////////////////////////////// Bounding Bbox //////////////////////////////////////
	_poMeshNode->ComputeBBox();

	fbxsdk::FbxDouble3 BoundBoxMax = _poMeshNode->BBoxMax.Get();
	fbxsdk::FbxDouble3 BoundBoxMin = _poMeshNode->BBoxMin.Get();

	double factor_to_meters =
		m_pScene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(fbxsdk::FbxSystemUnit::m);
		
	//  bounding box data to Group Header
	m_vecMeshes[m_mesh_index].vMax.x = BoundBoxMax[0] * factor_to_meters;
	m_vecMeshes[m_mesh_index].vMax.y = BoundBoxMax[1] * factor_to_meters;
	m_vecMeshes[m_mesh_index].vMax.z = BoundBoxMax[2] * factor_to_meters;

	m_vecMeshes[m_mesh_index].vMin.x = BoundBoxMin[0] * factor_to_meters;
	m_vecMeshes[m_mesh_index].vMin.y = BoundBoxMin[1] * factor_to_meters;
	m_vecMeshes[m_mesh_index].vMin.z = BoundBoxMin[2] * factor_to_meters;

	/////////////////////////////////////////////////////////////////////////////////////

	return true;

}

bool FBX_Reader::readUVs(fbxsdk::FbxMesh* _po)
{
	m_mapvecUVMaps = LoadUVInformation(_po);

	return  m_mapvecUVMaps.size() != 0 ? true : false;
}

bool FBX_Reader::readVectors(fbxsdk::FbxMesh* _po)
{
	



	/*fbxsdk::FbxNode* poNode = _po->GetNode();
	if (poNode == NULL)
		return _log_action("GetVectors(): poNode == nullptr");

	m_vecNormals = GetNormals(poNode);
	if (m_vecNormals.size() == 0) 	return _log_action("Normals not found");
	
	m_vecBiNormals = GetBinormals(poNode);
	if (m_vecBiNormals.size() == 0) return _log_action("Binormals not found");
	
	m_vecBitangents = GetBitangents(poNode);
	if (m_vecBitangents.size() == 0) return _log_action("Tagentss not found");
		*/


	return true;
}

bool FBX_Reader::readVertices(fbxsdk::FbxMesh* poMeshNode)
{
	/*poMeshNode->GenerateNormals(true);
	FbxGeometryConverter oGeometryConverter(m_pSdkManager);
	oGeometryConverter.ComputeEdgeSmoothingFromNormals(poMeshNode);
	oGeometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(poMeshNode);
	oGeometryConverter.ComputePolygonSmoothingFromEdgeSmoothing(poMeshNode);
	poMeshNode->SetMeshSmoothness(fbxsdk::FbxMesh::ESmoothness::eFine);*/
	// get the first UV map
	vector<FbxVector2> vec = m_mapvecUVMaps.begin()->second;

	// Control Points (Vertices) ///////////////////////////////////////////////////
	m_vecMeshes[m_mesh_index].polygon_vertex_count = poMeshNode->GetControlPointsCount();
	FbxVector4* PControlPoints = poMeshNode->GetControlPoints();

	m_vecMeshes[m_mesh_index].vecPolygonVertices.resize(m_vecMeshes[m_mesh_index].polygon_vertex_count);
	// END: Control Points (Vertices) //////////////////////////////////////////////
	
	// Unit conversion
	double factor_to_meters =
		m_pScene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(fbxsdk::FbxSystemUnit::m);
	double multi = m_pScene->GetGlobalSettings().GetSystemUnit().GetMultiplier(); 
	double fact = m_pScene->GetGlobalSettings().GetSystemUnit().GetScaleFactor();
	
	


	string m_strName = poMeshNode->GetNode()->GetName();

	// get vectors
	vector<FbxVector4> m_vecNormals = GetNormals(poMeshNode->GetNode());
	vector<FbxVector4> m_vecBitangents = GetBinormals(poMeshNode->GetNode());
	vector<FbxVector4> m_vecTangents = GetTangents(poMeshNode->GetNode());

	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* normals = nullptr;
	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* binormals = nullptr;
	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* Bitangents = nullptr;
	
	

	bool bNormalsResult = poMeshNode->GetNormals(&normals);
	bool bBinormalsResult = poMeshNode->GetBinormals(&binormals);
	bool bTangentResult = poMeshNode->GetBinormals(&Bitangents);
	
	double handedness_sign = 1;
	fbxsdk::FbxAxisSystem m_oAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();
	if (m_oAxisSystem.GetCoorSystem() == fbxsdk::FbxAxisSystem::ECoordSystem::eLeftHanded)
		handedness_sign = 1;
		
	handedness_sign = -1.0;
	
		
	_log_action("Processing " + to_string(m_vecMeshes[m_mesh_index].polygon_vertex_count) + " 'unnpacked', _potentially_ unique vertices...");

	// DEBUGGING CODE
	/*fbxsdk::FbxAMatrix m;
	m_oAxisSystem.GetMatrix(m);
	vecMeshes[m_mesh_index].transform_matrix = vecMeshes[m_mesh_index].transform_matrix * m;*/

	fbxsdk::FbxAMatrix matrix_rotation_only;
	matrix_rotation_only.SetIdentity();
	matrix_rotation_only.SetR(m_vecMeshes[m_mesh_index].transform_matrix.GetR());

	double correction_factor = 1.0;

	bool bTangentInverted = false;
	for (size_t i = 0; i < m_vecMeshes[m_mesh_index].polygon_vertex_count; i++)		
	{
		// get position and multiply it by the node transformation matrix
		
		FbxVector4 pos = m_vecMeshes[m_mesh_index].transform_matrix.MultT(PControlPoints[i]);				

		//FbxVector4 pos = PControlPoints[i]; // DEBUG code
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].position.x = (pos.mData[0]) * factor_to_meters * correction_factor;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].position.y = (pos.mData[1]) * factor_to_meters * correction_factor;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].position.z = (pos.mData[2]) * factor_to_meters * correction_factor;

		// normal vector		
		m_vecNormals[i] = matrix_rotation_only.MultT(m_vecNormals[i]);
		m_vecNormals[i].Normalize();		
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].normal.x = m_vecNormals[i].mData[0];
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].normal.y = m_vecNormals[i].mData[1];
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].normal.z = m_vecNormals[i].mData[2];
		
		//// tangent vector
		m_vecTangents[i] = matrix_rotation_only.MultT(m_vecTangents[i]);
		m_vecTangents[i].Normalize();		
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.x = m_vecTangents[i].mData[0]*0;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.y = m_vecTangents[i].mData[1]*0;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.z = m_vecTangents[i].mData[2]*0;

		// binormal/bitangent
		
		m_vecBitangents[i] = matrix_rotation_only.MultT(m_vecBitangents[i]);
		m_vecBitangents[i].Normalize();
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent.x = m_vecBitangents[i].mData[0]*0;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent.y = m_vecBitangents[i].mData[1]*0;
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent.z = m_vecBitangents[i].mData[2]*0;


		/// WORK FOR SOME ROME MODELS ???
		//m_vecBitangents[i] = matrix_rotation_only.MultT(m_vecBitangents[i]);
		//m_vecBitangents[i].Normalize();
		//vecMeshes[m_mesh_index].vecControlPoints[i].bitangent.x = -m_vecBitangents[i].mData[0];
		//vecMeshes[m_mesh_index].vecControlPoints[i].bitangent.y = -m_vecBitangents[i].mData[1];
		//vecMeshes[m_mesh_index].vecControlPoints[i].bitangent.z = -m_vecBitangents[i].mData[2];
		//
		/*----------------------------------------------------------
		correct tagent data
		----------------------------------------------------------*/

		//// load vectors into DirextX math structures
		//XMVECTOR vNormal = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i].normal);
		//XMVECTOR vTangent = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i].tangent);
		//XMVECTOR vBitangent = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i].bitangent);
		//
		//// perform the calculation: dot(cross(normal, tangent), bitangent)
		//XMVECTOR scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);	

		//// get dod product scalar out of directX math structure
		//float _scalar = 0;
		//XMStoreFloat(&_scalar, scalar);

		//if (false)
		//if (_scalar < 0.0f) // if dot prodict < 0.0, then invert tangent
		//{
		//	vecMeshes[m_mesh_index].vecControlPoints[i].tangent.x *= -1;
		//	vecMeshes[m_mesh_index].vecControlPoints[i].tangent.y *= -1;
		//	vecMeshes[m_mesh_index].vecControlPoints[i].tangent.z *= -1;

		//	bTangentInverted = true;


		//	vTangent = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i].tangent);
		//	scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

		//	// get dod product scalar out of directX math structure			
		//	XMStoreFloat(&_scalar, scalar);


		//	int debug_a = 1;
		//}

		///*if (glm::dot(glm::cross(n, t), b) < 0.0f) {
		//	t = t * -1.0f;
		//}*/


		// UV
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.x = vec[i].mData[0];
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.y = 1 - vec[i].mData[1];
	}
	
	
//	calcTagentData();


	if (bTangentInverted)
		_log_write("FBX_Reader::getVertices: Vertex tangent data was correcteed.");

	return true;
}

bool FBX_Reader::readVerticesTriangles(fbxsdk::FbxMesh* poMeshNode)
{




	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* normals = nullptr;
	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* binormals = nullptr;
	fbxsdk::FbxLayerElementArrayTemplate<FbxVector4>* Bitangents = nullptr;


	vector<FbxVector2> vec = m_mapvecUVMaps.begin()->second;

	// position info
	FbxVector4* pControlPoints = poMeshNode->GetControlPoints();
	// polygon info, count + indexes to each "corner"
	int polygon_vertex_count = poMeshNode->GetPolygonVertexCount();
	int* pPolyggonVertices = poMeshNode->GetPolygonVertices();

	// Control Points (Vertices) ///////////////////////////////////////////////////
	string strMeshName = poMeshNode->GetNode()->GetName();
	m_vecMeshes[m_mesh_index].m_strName = strMeshName;
	m_vecMeshes[m_mesh_index].polygon_vertex_count = polygon_vertex_count;
	m_vecMeshes[m_mesh_index].index_count = polygon_vertex_count;

	m_vecMeshes[m_mesh_index].vecPolygonVertices.resize(polygon_vertex_count);
	m_vecMeshes[m_mesh_index].vecIndices32.resize(polygon_vertex_count);
	// END: Control Points (Vertices) //////////////////////////////////////////////

	fbxsdk::FbxArray<FbxVector4> PV_Normals;
	poMeshNode->GetPolygonVertexNormals(PV_Normals);

	// get first UV map
	vector<FbxVector2> UVs = m_mapvecUVMaps.begin()->second;

	fbxsdk::FbxArray<FbxVector2> PV_UVs;
	poMeshNode->GetPolygonVertexUVs(m_mapvecUVMaps.begin()->first.c_str(), PV_UVs);

	// Unit conversion
	double factor_to_meters =
		m_pScene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(fbxsdk::FbxSystemUnit::m);
	double multi = m_pScene->GetGlobalSettings().GetSystemUnit().GetMultiplier();
	double fact = m_pScene->GetGlobalSettings().GetSystemUnit().GetScaleFactor();
	

	const fbxsdk::FbxGeometryElementUV* lUVElement = poMeshNode->GetElementUV(0);
	std::string m_strName = lUVElement->GetName();
	int uv_count = lUVElement->GetDirectArray().GetCount();

	// *************************************
	// TESTING CODE: so far this makes this best tagent data
	// *************************************
	//	 set the texcorrd.v = 1 - v
	for (size_t i = 0; i < uv_count; i++)
	{
		fbxsdk::FbxVector2 oUV =
			lUVElement->GetDirectArray().GetAt(i);

		oUV.mData[0] = oUV.mData[0];
		oUV.mData[1] = 1.0 - oUV.mData[1];
				
		lUVElement->GetDirectArray().SetAt(i, oUV);
	}

	// TODO: use the transformed normal to calculate better tangent data

	// --------------------------------------------------------------------------------
	// setup matrix for uses only for normal, tangent and bitangent vectors
	fbxsdk::FbxAMatrix normal_tranformation_matrix;
	normal_tranformation_matrix.SetIdentity();
	normal_tranformation_matrix.SetR(m_vecMeshes[m_mesh_index].transform_matrix.GetR());
	//normal_tranformation_matrix.SetS(vecMeshes[m_mesh_index].transform_matrix.GetS());
	// --------------------------------------------------------------------------------
		
	//FbxGeometryElementNormal* pNormalElement = poMeshNode->GetElementNormal();
	//int normal_count = pNormalElement->GetDirectArray().GetCount();


	// --- TEST CODE ---
	//for (size_t i = 0; i < normal_count; i++)
	//{
	//	//TODO: transform the normals before calculting tangent data
	//	// N = M_normal * N
	//	// N = Identity, so the normals are not further transformed, (if it works, simply delete transforms)
	//	pNormalElement->GetDirectArray()[i] = normal_tranformation_matrix.MultT(pNormalElement->GetDirectArray()[i]);
	//}
	//normal_tranformation_matrix.SetIdentity();
	////bool bResult = poMeshNode->GenerateTangentsDataForAllUVSets(true);
	

	// ----------------------------------------------------------------------------
	// Generate tangent data
	bool bGenerateTangentDataResult = poMeshNode->GenerateTangentsData(0, true, true);
	// ----------------------------------------------------------------------------
	

	// get vectors
	vector<FbxVector4> m_vecNormals = GetNormals(poMeshNode->GetNode());
	vector<FbxVector4> m_vecBitangents = GetBinormals(poMeshNode->GetNode());
	vector<FbxVector4> m_vecTangents = GetTangents(poMeshNode->GetNode());

	
	fbxsdk::FbxAMatrix normal_scale_matrix;
	normal_scale_matrix.SetIdentity();
	FbxVector4 v = m_vecMeshes[m_mesh_index].transform_matrix.GetS();
		v[0] /= abs(v[0]);

		v[1] /= abs(v[1]);

		v[2] /= abs(v[2]);

		v[3] /= abs(v[3]);


	//normal_scale_matrix.SetS(vecMeshes[m_mesh_index].transform_matrix.GetS());
	normal_scale_matrix.SetQOnly(m_vecMeshes[m_mesh_index].transform_matrix.GetQ());



	
	//normal_tranformation_matrix.SetS(vecMeshes[m_mesh_index].transform_matrix.GetS());

	FbxVector4 vGlobalScale = m_vecMeshes[m_mesh_index].transform_matrix.GetS();

	vGlobalScale[0] /= abs(vGlobalScale[0]);
	vGlobalScale[1] /= abs(vGlobalScale[1]);
	vGlobalScale[2] /= abs(vGlobalScale[2]);

	
	//vGlobalScale.Normalize();
	// Make reflect matrix
	FbxAMatrix mReflect;
	mReflect.SetIdentity();
	mReflect[0][0] = vGlobalScale[0];
	mReflect[1][1] = vGlobalScale[1];
	mReflect[2][2] = vGlobalScale[2];


	//  ------------------- DEBUGING CODE -------------------
	//TODO: TRY and test what signs the vector (1,1,1) get after being multiplied by the scale matrix
	// and see if those signs
	FbxAMatrix mFlip;
	FbxVector4 mScaleXYZ = m_vecMeshes[m_mesh_index].transform_matrix.GetS(); // look at the signs
	//mScaleXYZ.Normalize();
	mFlip.SetROnly(mScaleXYZ);

	FbxVector4 vTest = { 1,0,0,0 };

	vTest = mFlip.MultT(vTest);

	double x = mScaleXYZ[0]; // =  -1; flip over YZ plane, x= -x; 
	double y = mScaleXYZ[1]; //=  -1; flip over XZ pane, y= -y; 
	double z = mScaleXYZ[2]; //=  -1; flip over XY pane, z= -z; 
	// TODO: check that plane above are correct, if that is important, at all.
	//  END: ------------------- DEBUGING CODE -------------------
	
	x =  x / abs(x);
	y =  y / abs(y);
	z =  z / abs(z);
	//matrix_rotation_only = matrix_rotation_only;

	
	int triangle_count = poMeshNode->GetPolygonCount();

	int vertex_index = 0;

	for (size_t triangle_index = 0; triangle_index < triangle_count; triangle_index++)
	{



		for (size_t edge = 0; edge < 3;  edge++)
		{
			int control_point_index = poMeshNode->GetPolygonVertex(triangle_index, edge);
			
			FbxVector4 vControlPoint = pControlPoints[control_point_index];
			
			// get influences for this control point, and add to polygon vertex, "edge"
			vector<pair<int, float>> vecWeightInfluences = m_vecMeshes[m_mesh_index].vecvecControlPointInfluences[control_point_index];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].vecInfluences = vecWeightInfluences;

			// apply transform
			vControlPoint = (m_vecMeshes[m_mesh_index].transform_matrix).MultT(vControlPoint);			
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].position.x = vControlPoint.mData[0] * factor_to_meters;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].position.y = vControlPoint.mData[1] * factor_to_meters;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].position.z = vControlPoint.mData[2] * factor_to_meters;



			// --------------------------------------------------------- NORMALS --------------------------------------------------- 
			// transformation  + assign normals			
			FbxVector4 vNormalVector;
			poMeshNode->GetPolygonVertexNormal(triangle_index, edge, vNormalVector);							
			//vNormalVector =(vecMeshes[m_mesh_index].normal_transformation_matrix).MultT(vNormalVector);
			vNormalVector =(normal_tranformation_matrix).MultT(vNormalVector);
			
			vNormalVector.Normalize();
			
			double sign = 1.0;

			//if (vecMeshes[m_mesh_index].m_bNegativeScaling)
			//sign = -1.0;

			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].normal.x = vNormalVector.mData[0] * sign;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].normal.y = vNormalVector.mData[1]  * sign;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].normal.z = vNormalVector.mData[2]  * sign;
			


			// --------------------------------------------------------- Tangents --------------------------------------------------- 
			//			 transformation  + assign normals			
			FbxVector4 vTangentVector;
			vTangentVector = m_vecTangents[vertex_index];
			
			vTangentVector =(normal_tranformation_matrix).MultT(vTangentVector);
			vTangentVector.Normalize();

			sign = 1.0f;

			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.x = vTangentVector.mData[0];// * x;// *vGlobalScale[0];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.y = vTangentVector.mData[1];// * y;// *vGlobalScale[1];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.z = vTangentVector.mData[2];// * z;// *vGlobalScale[2];

			// check w
			if (vTangentVector.mData[3] < 0)
			{
				int debug_int1 = 1;
			}  

			// --------------------------------------------------------- Bitangents --------------------------------------------------- 
			//			 transformation  + assign normals			
			FbxVector4 vBitangentVector;
			vBitangentVector = m_vecBitangents[vertex_index];
			
			vBitangentVector =normal_tranformation_matrix.MultT(vBitangentVector);			
			vBitangentVector.Normalize();
						
			sign = 1.0f;

			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent.x = vBitangentVector.mData[0];// * x;// *vGlobalScale[0];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent.y = vBitangentVector.mData[1];// * y;// *vGlobalScale[1];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent.z = vBitangentVector.mData[2];// * z;// *vGlobalScale[2];


			if (vBitangentVector.mData[3] < 0)
			{
				int debug_int1 = 1;
			}
			//






			bool mapped = false;
			FbxVector2 vUV;
			poMeshNode->GetPolygonVertexUV(triangle_index, edge, m_mapvecUVMaps.begin()->first.c_str(), vUV, mapped);


			// ---- TEST CODE: -----,			
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].textcords.x = vUV.mData[0];
			m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].textcords.y = vUV.mData[1];

			//// -- TEST CODE FOR WHEN V = 1 - V is not already set ---
			//vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].textcords.x = vUV.mData[0];
			//vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].textcords.y = 1.0 - vUV.mData[1];

			m_vecMeshes[m_mesh_index].vecIndices32[vertex_index] = vertex_index;

			

			//,if (false)
			{
				XMVECTOR vNormal = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].normal);
				XMVECTOR vBitangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent);
				XMVECTOR vTangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent);

				// perform the calculation: dot(cross(normal, tangent), bitangent)
				XMVECTOR scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

				// get dot product scalar out of directX math structure
				float _scalar = 0;
				XMStoreFloat(&_scalar, scalar);

				// for logging / console outputs, to avoid doing console write in the loop
				// and only do ONE "1 or more tagents were corrected" output
				bool bTangentInverted;

				if (false) // DEBUGGIN CODE, "to correct or not to correct, that is the question."
				if (_scalar < 0.0f) // if dot prodict < 0.0, then invert tangent
				{
					m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.x *= -1;
					m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.y *= -1;
					m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent.z *= -1;

					bTangentInverted = true;

					// DEBUGGING CODE: recheck TBN orthogonality
					vTangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent);
					scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

					// get dot product scalar out of directX math structure			
					float _scalar_corrected = 0;
					XMStoreFloat(&_scalar_corrected, scalar);


					int debug_a = 10;
					// END: DEBUGGING CODE: recheck TBN orthogonality			
				}
				/*
				// swap tangent and bitangent around
				XMFLOAT3 temp = vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent;

				vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent =
					vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent;

				vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent = temp;*/
				
			}		

			// swap tangent and bitangent around
			/*XMFLOAT3 temp = vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent;

			vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent =
				vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent;

			vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent = temp;*/

			vertex_index++;
		}

		
		

	}

	return true;

	//for (size_t pv = 0; pv < polygon_vertex_count; pv++)
	//{
	//
	//	// get polygon vertex index;
	//	int index = pPolyggonVertices[pv];
	//	
	//	// get contron poiont from buffer
	//	FbxVector4 vControlPoint = pControlPoints[index];
	//	
	//	
	//	// transform
	//	vControlPoint =
	//		vecMeshes[m_mesh_index].transform_matrix.MultT(vControlPoint);

	//	// assign to XMLFLOAT3;
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].position.x = vControlPoint.mData[0] * factor_to_meters;
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].position.y = vControlPoint.mData[1] * factor_to_meters;
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].position.z = vControlPoint.mData[2] * factor_to_meters;
	//	
	//	// transformation  + assign normals
	//	FbxVector4 vNormalVector = matrix_rotation_only.MultT(PV_Normals[index]);
	//	vNormalVector.Normalize();
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].normal.x = vNormalVector.mData[0];
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].normal.y = vNormalVector.mData[1];
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].normal.z = vNormalVector.mData[2];
	//	
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].textcords.x = PV_UVs[index].mData[0];
	//	vecMeshes[m_mesh_index].vecPolygonVertices[pv].textcords.y = 1.0 - PV_UVs[index].mData[1];







	//}




	//return true;
}

bool FBX_Reader::readIndices(fbxsdk::FbxMesh* poMeshNode)
{
	
	



	// Polygon vertices (indices) ///////////////////////////////////////////////////
	int poly_v_count = poMeshNode->GetPolygonVertexCount();
	int* pPolyVIndices = poMeshNode->GetPolygonVertices();
	m_vecMeshes[m_mesh_index].polygon_vertex_count = poly_v_count;
	m_vecMeshes[m_mesh_index].vecIndices32.resize(poly_v_count);

	

	// END: polygon verts (indices) ///////////////////////////////////////////////

	// Switch index -2 and 3
	_log_action("FBX_Reader::getIndices(): Processing " + to_string(poly_v_count) + " 'unnpacked', _poentially_ unique indices...");
	for (size_t i = 0; i < poly_v_count / 3; i++)
	{		
		//if (vecMeshes[m_mesh_index].m_bNegativeScaling)
		//{
		//	vecMeshes[m_mesh_index].vecIndices32[3 * i] = pPolyVIndices[3 * i];
		//	vecMeshes[m_mesh_index].vecIndices32[3 * i + 2] = pPolyVIndices[3 * i + 1];
		//	vecMeshes[m_mesh_index].vecIndices32[3 * i + 1] = pPolyVIndices[3 * i + 2];

		//}
		//else
		{
			m_vecMeshes[m_mesh_index].vecIndices32[3 * i] = pPolyVIndices[3 * i];
			m_vecMeshes[m_mesh_index].vecIndices32[3 * i + 1] = pPolyVIndices[3 * i + 1];
			m_vecMeshes[m_mesh_index].vecIndices32[3 * i + 2] = pPolyVIndices[3 * i + 2];
		}
		//m_vecRMV2_Indices[g][3 * i] = pPolyVIndices[3 * i];
		//m_vecRMV2_Indices[g][3 * i + 1] = pPolyVIndices[3 * i + 2];
		//m_vecRMV2_Indices[g][3 * i + 2] = pPolyVIndices[3 * i + 1];
	}

	return true;
}

bool FBX_Reader::readIndicesUnindexed(fbxsdk::FbxMesh* poMeshNode)
{





	// Polygon vertices (indices) ///////////////////////////////////////////////////
	int poly_v_count = poMeshNode->GetPolygonVertexCount();
	//int* pPolyVIndices = poMeshNode->GetPolygonVertices();
	m_vecMeshes[m_mesh_index].polygon_vertex_count = poly_v_count;
	m_vecMeshes[m_mesh_index].vecIndices32.resize(poly_v_count);



	// END: polygon verts (indices) ///////////////////////////////////////////////

	// Switch index -2 and 3
	_log_action("FBX_Reader::getIndices(): Processing " + to_string(poly_v_count) + " 'unnpacked', _poentially_ unique indices...");
	for (size_t i = 0; i < poly_v_count; i++)
	{
		m_vecMeshes[m_mesh_index].vecIndices32[i] = i;
		/*vecMeshes[m_mesh_index].vecIndices32[3 * i + 1] = pPolyVIndices[3 * i + 1];
		vecMeshes[m_mesh_index].vecIndices32[3 * i + 2] = pPolyVIndices[3 * i + 2];*/

		//m_vecRMV2_Indices[g][3 * i] = pPolyVIndices[3 * i];
		//m_vecRMV2_Indices[g][3 * i + 1] = pPolyVIndices[3 * i + 2];
		//m_vecRMV2_Indices[g][3 * i + 2] = pPolyVIndices[3 * i + 1];
	}

	return true;
}

bool FBX_Reader::readWeights(fbxsdk::FbxMesh* pGroup)
{
	

	


	int deformer_count = pGroup->GetDeformerCount();

	// Error checking,
	// TODO: check if skin count > 1 is actually "wrong" for RMV2s
	if (deformer_count < 1)
	{
		return _log_action_warning(string(pGroup->GetName()) + ": no deformer/skin modifier found. No Rigging Will Be Added.");
		//continue;
	}
	else if (deformer_count > 1)
	{
		return _log_action_warning(string(pGroup->GetName()) + ": deformer/skin counter > 1. No Rigging Will Be Added.");
		//continue;
	}

	// get skin 0
	fbxsdk::FbxSkin* pSkin = (FbxSkin*)pGroup->GetDeformer(0);
	if (!pSkin)
	{
		return _log_action_warning(string(pGroup->GetName()) + ": pSkin == NULL.");
		
	}

	int cluster_count = pSkin->GetClusterCount();

	// check if there is no rigging data for this skin
	if (cluster_count < 1)
	{
		return _log_action_warning(string(pGroup->GetName()) + ": no weighting data found for skin: " + string(pSkin->GetName()) + " !");
		//continue;
	}
	// rough through all clusters (1 cluster = 1 bone)

	for (size_t cluster = 0; cluster < cluster_count; cluster++)
	{
		// get the collection of {vertex, weight}
		fbxsdk::FbxCluster* pCluster = pSkin->GetCluster(cluster);

		// get bone
		fbxsdk::FbxNode* pBoneNode = pCluster->GetLink();

		// get the bone ID for this bone name
		string strBoneName = tools::lower(pBoneNode->GetName());

		// get bone id from name->id table
		int BONE_ID = -1;
		// check if bone name exists in the bone name to ID table
		BONE_ID = m_oBoneTable.getIdFromName(strBoneName);
		if (BONE_ID == -1)
			return _log_action_error("Bone in FBX File: '" + strBoneName +
				"' is not found in skeleton/animtion file: " + m_strAnimSkeletonFile);


		_log_action("Processing weighting for bone: '" + strBoneName +
			", ID: " + to_string(BONE_ID));

		int vertex_index_count = pCluster->GetControlPointIndicesCount();

		// influences
		//vecMeshes[m_mesh_index].vecvecInfluences.resize(vecMeshes[m_mesh_index].control_point_count);

		int* pControlPointIndices = pCluster->GetControlPointIndices();
		double* pVertexWeights = pCluster->GetControlPointWeights();
		 

		int ctrl_point_index = 0;
		float weight = 0;	// initiallize geometry converter
		size_t vertex_index = 0;
		
		for (vertex_index = 0; vertex_index < vertex_index_count; vertex_index++)
		{
			ctrl_point_index = pControlPointIndices[vertex_index];

			// get weight associated with vertex
			weight = pVertexWeights[vertex_index];

			// add the influence {BONE, WEIGHT} to the proper vertex
			m_vecMeshes[m_mesh_index].vecPolygonVertices[ctrl_point_index].addInfluence(BONE_ID, weight);
			//vecMeshes[m_mesh_index].vecControlPoints[ctrl_point_index].addInfluence(0, 1.0f); // DEBUG CODE
		};
	}

	return true;
}

bool FBX_Reader::readControlPointWeights(fbxsdk::FbxMesh* poMeshNode)
{
	int ctrl_point_count = poMeshNode->GetControlPointsCount();
	m_vecMeshes[m_mesh_index].vecvecControlPointInfluences.resize(ctrl_point_count);

	int deformer_count = poMeshNode->GetDeformerCount();

	// Error checking,
	// TODO: check if skin count > 1 is actually "wrong" for RMV2s
	if (deformer_count < 1)
	{
		return _log_action_warning(string(poMeshNode->GetName()) + ": no deformer/skin modifier found. No Rigging Will Be Added.");
		//continue;
	}
	else if (deformer_count > 1)
	{
		return _log_action_warning(string(poMeshNode->GetName()) + ": deformer/skin counter > 1. No Rigging Will Be Added.");
		//continue;
	}

	// get skin 0
	fbxsdk::FbxSkin* pSkin = (FbxSkin*)poMeshNode->GetDeformer(0);
	if (!pSkin)
	{
		return _log_action_warning(string(poMeshNode->GetName()) + ": pSkin == NULL.");

	}

	int cluster_count = pSkin->GetClusterCount();

	// check if there is no rigging data for this skin
	if (cluster_count < 1)
	{
		return _log_action_warning(string(poMeshNode->GetName()) + ": no weighting data found for skin: " + string(pSkin->GetName()) + " !");
		//continue;
	}
	// rough through all clusters (1 cluster = 1 bone)

	for (size_t cluster = 0; cluster < cluster_count; cluster++)
	{
		// get the collection of {vertex, weight}
		fbxsdk::FbxCluster* pCluster = pSkin->GetCluster(cluster);

		// get bone
		fbxsdk::FbxNode* pBoneNode = pCluster->GetLink();

		// get the bone ID for this bone name
		string strBoneName = tools::lower(pBoneNode->GetName());

		// get bone id from name->id table
		int BONE_ID = -1;
		// check if bone name exists in the bone name to ID table
		BONE_ID = m_oBoneTable.getIdFromName(strBoneName);
		if (BONE_ID == -1)
			return _log_action_error("Bone in FBX File: '" + strBoneName +
				"' is not found in skeleton/animtion file: " + m_strAnimSkeletonFile);


		_log_action("Processing weighting for bone: '" + strBoneName +
			", ID: " + to_string(BONE_ID));

		int vertex_index_count = pCluster->GetControlPointIndicesCount();

		// influences
		//vecMeshes[m_mesh_index].vecvecInfluences.resize(vecMeshes[m_mesh_index].control_point_count);

		int* pControlPointIndices = pCluster->GetControlPointIndices();
		double* pVertexWeights = pCluster->GetControlPointWeights();


		int ctrl_point_index = 0;
		float weight = 0;	// initiallize geometry converter
		size_t vertex_index = 0;

		for (vertex_index = 0; vertex_index < vertex_index_count; vertex_index++)
		{
			ctrl_point_index = pControlPointIndices[vertex_index];

			// get weight associated with vertex
			weight = pVertexWeights[vertex_index];

			// add the influence {BONE, WEIGHT} to the proper vertex
			m_vecMeshes[m_mesh_index].vecvecControlPointInfluences[ctrl_point_index].push_back({ BONE_ID, weight });
			//vecMeshes[m_mesh_index].vecControlPoints[ctrl_point_index].addInfluence(0, 1.0f); // DEBUG CODE
		};
	}


	return true;
}

void FBX_Reader::readSKeleton(fbxsdk::FbxNode * _poBone)
{
	// map bone name to parent bone name
	mapBoneNameToParentBoneName[_poBone->GetName()] = _poBone->GetParent()->GetName();
	mapBoneNameToFbxNode[_poBone->GetName()] = _poBone;

	mapBonidToName[current_bone_id] = _poBone->GetName();
	mapBoneNameToId[_poBone->GetName()] = current_bone_id;
	
	
	current_bone_id++;

	int child_count = _poBone->GetChildCount();
	

	for (size_t bone = 0; bone < child_count; bone++)
	{
		readSKeleton(_poBone->GetChild(bone));
	}

}

void FBX_Reader::processSkeleteton()
{


	return;

	m_oBoneTable.read(L"humanoid01.bone_table");

	mapBoneNameToId.clear();

	for (size_t i = 0; i < m_oBoneTable.getBoneTable().size(); i++)
	{
		mapBoneNameToId[m_oBoneTable.getBoneTable()[i].strBoneName] = i;
	}

	vecNameParentId.resize(mapBoneNameToId.size());
	for (auto& it : mapBoneNameToId)
	{
		string strParent = mapBoneNameToParentBoneName[it.first];
		
		int parent_id = -2;
		
		if (strParent == "RootNode")
			parent_id = -1;
		else
			parent_id = mapBoneNameToId[strParent];
		
		int id = it.second;
				
		mapBoneNameToIdAndParent[it.first] = { id, parent_id };

		vecNameParentId[id] = { it.first, parent_id };
	};

	
	SystemLib::DataStream file("human01.anim");
	auto exporter = TWAnim::createAnimExporter(file);
	auto po = exporter->oAnimFile;

	po.oFileHeader.dwBoneCount = (uint32_t)vecNameParentId.size();
	po.oFileHeader.strSkeletonId = "humanoid01";
	// write inv bind pose matrices
	ofstream inv_file("humanoid01_RME.bone_inv_trans_mats", ios::binary);
	uint32_t num = 1;
	inv_file.write((char*)&num, 4);
	num = (uint32_t)vecNameParentId.size();
	inv_file.write((char*)&num, 4);

	auto joint_count = vecNameParentId.size();
	po.vecMaskTranslations.resize(vecNameParentId.size());
	po.vecMaskRotations.resize(vecNameParentId.size());
	
	po.vecBoneData.resize(vecNameParentId.size());

	// make 2 skeleton frames
	po.vecFrames.resize(2);
	po.vecFrames[0].vecTranslations.resize(joint_count);
	po.vecFrames[0].vecQuaternions.resize(joint_count);

	double factor_to_meters =
		m_pScene->GetGlobalSettings().GetSystemUnit().GetConversionFactorTo(fbxsdk::FbxSystemUnit::m);

	for (size_t i = 0; i < vecNameParentId.size(); i++)
	{
		po.vecBoneData[i].parent_id = vecNameParentId[i].second;
		po.vecBoneData[i].strName = vecNameParentId[i].first;
		po.vecMaskTranslations[i] = i;
		po.vecMaskRotations[i] = i;

		// get fbx node
		auto fbx_node = mapBoneNameToFbxNode[vecNameParentId[i].first];

		// store local tranlations
		auto local_transform = fbx_node->EvaluateLocalTransform();
		auto debug_global_tranform_before = fbx_node->EvaluateGlobalTransform(); // TODO: DEBUGGING CODE
		auto trans = local_transform.GetT();		
		

		// ---------------------------------------------------------------
		//		nodes for .ANIM storage	
		// ---------------------------------------------------------------
		// change handedness on translations and convert to game's unit
		trans[0] = trans[0] * factor_to_meters;
		trans[1] = trans[1] * factor_to_meters;
		trans[2] = trans[2] * factor_to_meters;
		fbx_node->LclTranslation.Set(trans); // put the corrected translation back into the node

		auto debug_global_tranform_after = fbx_node->EvaluateGlobalTransform(); // TODO: DEBUGGING CODE

		trans = fbx_node->EvaluateLocalTransform().GetT();
				
		// save the translation in "meters"
		po.vecFrames[0].vecTranslations[i].x = trans[0]; 
		po.vecFrames[0].vecTranslations[i].y = trans[1];
		po.vecFrames[0].vecTranslations[i].z = trans[2];

		// store quaternion from local transform
		auto quat = fbx_node->EvaluateLocalTransform().GetQ();
		po.vecFrames[0].vecQuaternions[i].x = quat[0];
		po.vecFrames[0].vecQuaternions[i].y = quat[1];
		po.vecFrames[0].vecQuaternions[i].z = quat[2];
		po.vecFrames[0].vecQuaternions[i].w = quat[3];
		

		// ---------------------------------------------------------------
		//		Inverse Bind Pose Matrices
		// ---------------------------------------------------------------
		// get the global transformation matrix and make into an inverse bind pose matrix
		auto matrix = fbx_node->EvaluateGlobalTransform();
		matrix = matrix.Inverse();
		matrix = matrix.Transpose();

		// run though the inverse bind pose matrix
		for (size_t n = 0; n < 4; n++)
		{
			for (size_t m = 0; m < 3; m++)
			{
				float a = matrix[m][n];
				//float a = 0.1; // TODO: DEBUGGIN CODE, makes incorrect matrices on purpose
				inv_file.write((char*)&a, 4);
			}
		}		
	}

	po.vecFrames[1] = po.vecFrames[0];
	po.oFrameHeader.dwRotationCount = joint_count;
	po.oFrameHeader.dwTranslationsCount = joint_count;
	po.dwFrameCount = 2;

	inv_file.close();

	exporter->write(file); // write all to DataStream
	file.write_file(); // write to disk


	int debug_a = 1;

}

bool FBX_Reader::cleanUpMesh(FBX_Mesh& mesh)
{
	

	/*-------log---------*/
	_log_action("Peforming 'un-unrolling', non-descrutive, mesh remapping..");
	/*-------log---------*/

	struct v
	{
		float x;
		float y;
		float z;

		float normal_x;
		float normal_y;
		float normal_z;


		float u, v;
	};

	vector<v> vecVerticesMin;
	vecVerticesMin.resize(mesh.vecPolygonVertices.size());
	for (size_t i = 0; i < mesh.vecPolygonVertices.size(); i++)
	{
		vecVerticesMin[i].x = mesh.vecPolygonVertices[i].position.x;
		vecVerticesMin[i].y = mesh.vecPolygonVertices[i].position.y;
		vecVerticesMin[i].z = mesh.vecPolygonVertices[i].position.z;


		vecVerticesMin[i].normal_x = mesh.vecPolygonVertices[i].normal.x;
		vecVerticesMin[i].normal_y = mesh.vecPolygonVertices[i].normal.y;
		vecVerticesMin[i].normal_z = mesh.vecPolygonVertices[i].normal.z;

		vecVerticesMin[i].u = mesh.vecPolygonVertices[i].textcords.x;
		vecVerticesMin[i].v = mesh.vecPolygonVertices[i].textcords.y;
	}

	size_t start_total_indices = mesh.vecIndices32.size();
	size_t start_total_vertices = mesh.vecPolygonVertices.size();
	std::vector<unsigned int> remap(start_total_indices);

	/*-------log---------*/
	_log_action("Unroll operation resulted in: " + to_string(start_total_indices) + "vertices.");
	/*-------log---------*/

	

	size_t unique_vertices = meshopt_generateVertexRemap<uint32_t>(
		&remap[0], 
		&mesh.vecIndices32[0],
		start_total_indices, 
		&vecVerticesMin[0], 
		start_total_vertices, sizeof(v));


	/*-------log---------*/
	_log_action("Indexing Cleanup resulted in: " + to_string(unique_vertices) + " unnique vertices. \n\r Cleaned up " +
		to_string(start_total_indices - unique_vertices) + " duplicate vertices.");


	/*-------log---------*/

	_log_action("Remapping gave  " + to_string(unique_vertices) + " unique vertices...");
	/*-------log---------*/

	vector<uint32_t> mesh_indices(start_total_indices);
	meshopt_remapIndexBuffer(&mesh_indices[0], &mesh.vecIndices32[0], start_total_indices, &remap[0]);

	vector<RMV2_Vertex_Common> newVertices(unique_vertices);
	meshopt_remapVertexBuffer(&newVertices[0], &mesh.vecPolygonVertices[0],
		start_total_vertices, sizeof(RMV2_Vertex_Common), &remap[0]);

	

	

	

	//_log_action("Remappped to " + to_string(unique_vertices) + " _acutually_ uinque vertice"
	//	+ "\n\r" + to_string(start_total_vertices - unique_vertices) + " redundants.");


	//// optimized mesh
	
	mesh.vecPolygonVertices.resize(newVertices.size());
	mesh.vecIndices32.resize(mesh_indices.size());

	
	mesh.vecPolygonVertices = newVertices;		
	mesh.vecIndices32 = mesh_indices;
		
	// new sizes
	mesh.polygon_vertex_count = mesh.vecPolygonVertices.size();
	mesh.polygon_vertex_count = mesh.vecIndices32.size();

	return true;
}

bool FBX_Reader::findSubMeshes(fbxsdk::FbxNode* _poNode)
{
	
	//fbxsdk::FbxNode* pRootNode = m_pScene->GetRootNode();
	fbxsdk::FbxNode* pRootNode = _poNode;

	// go through the chilren of the root node
	int child_count = pRootNode->GetChildCount();

	////////////////////////////////////////////
	// get all the LOD1 submeshes:
	////////////////////////////////////////////
	fbxsdk::FbxNode* pChildNode = NULL;
	for (size_t i = 0; i < child_count; i++)
	{		

		// get child of root node by index
		pChildNode = pRootNode->GetChild(i);
		if (pChildNode == nullptr)
			return _log_action_error("FBX_Reader::findSubMeshes(): Node " + to_string(i) + " is invalid (== nullptr)");
		string m_strName = tools::lower(pChildNode->GetName());
		
		fbxsdk::FbxNodeAttribute* poAtrr = pChildNode->GetNodeAttribute();

		// Get node attributes
		if (poAtrr == nullptr)
		{
			_log_action_warning("FBX_Reader::findSubMeshes(): node '" + m_strName + "' has no atrribue (of unknown type)");
				continue;
		}
		fbxsdk::FbxNodeAttribute::EType eArributeType = poAtrr->GetAttributeType();

		// if node attribute = mesh, get the groups in LOD1
		if (eArributeType == fbxsdk::FbxNodeAttribute::EType::eMesh)
		{
			fbxsdk::FbxMesh* poMeshNode = (fbxsdk::FbxMesh*)pChildNode->GetNodeAttribute();

			// check if mesh name has "_lod1" in the name
			string m_strName = tools::lower(pChildNode->GetName());			
			if (m_strName.find("_lod1") != string::npos) // TODO: REENABLE
			{
				bool show = pChildNode->Show.Get();
				if (show)
					m_vecMeshNodePtrs.push_back(poMeshNode);
			}
			// TODO: make sorting of pre-generated LODs work
			// if there is an "lod2" it means that the FBX file has its own LODs
			/*if (strName.find("_lod2") != string::npos)
				m_bHasOwnLods = true;*/
			
		}

		if (eArributeType == fbxsdk::FbxNodeAttribute::EType::eSkeleton)
		{
			fbxsdk::FbxSkeleton* poSkeletonNode = (fbxsdk::FbxSkeleton*)pChildNode->GetNodeAttribute();
			
			

			// check if mesh name has "_lod1" in the name
			string m_strName = tools::lower(pChildNode->GetName());			
			//if (strName.find("_lod1") != string::npos) // TODO: REENABLE
			{
				bool show = pChildNode->Show.Get();
				if (show)
					m_vecSkeletonPtrs.push_back(poSkeletonNode);
			}
			// TODO: make sorting of pre-generated LODs work
			// if there is an "lod2" it means that the FBX file has its own LODs
			/*if (strName.find("_lod2") != string::npos)
				m_bHasOwnLods = true;*/
			
		}
	}	

	

	// resize the non-FBX mesh storage
	m_vecMeshes.resize(m_vecMeshNodePtrs.size());
	
	// if not _LOD1 size = 0
	if (m_vecMeshNodePtrs.size() < 1)
		return _log_action_error("FBX_Read::findSubmeshes(): No '_lod1' found in any submesh name!");

	return true;
}

bool FBX_Reader::findSubMeshesRecursive(fbxsdk::FbxNode* _poNode)
{
	//fbxsdk::FbxNode* pRootNode = m_pScene->GetRootNode();
	fbxsdk::FbxNode* pRootNode = _poNode;

	// go through the chilren of the root node
	int child_count = pRootNode->GetChildCount();

	////////////////////////////////////////////
	// get all the LOD1 submeshes:
	////////////////////////////////////////////
	fbxsdk::FbxNode* pChildNode = NULL;
	for (size_t i = 0; i < child_count; i++)
	{
		// get child of root node by index
		pChildNode = pRootNode->GetChild(i);
		fbxsdk::FbxNodeAttribute* poAtrr = pChildNode->GetNodeAttribute();

		// Get node attributes
		fbxsdk::FbxNodeAttribute::EType eArributeType = poAtrr->GetAttributeType();

		// if node attribute = mesh, get the groups in LOD1
		if (eArributeType == fbxsdk::FbxNodeAttribute::EType::eMesh)
		{
			fbxsdk::FbxMesh* poMeshNode = (fbxsdk::FbxMesh*)pChildNode->GetNodeAttribute();

			string m_strName = tools::lower(pChildNode->GetName());
			// check if mesh name has "_lod1" in the name
			if (m_strName.find("_lod1") != string::npos)
			{
				m_vecMeshNodePtrs.push_back(poMeshNode);
			}
		}

		// call the same function on each child (recursive)
		findSubMeshesRecursive(pChildNode);
	}

	// resize the non-FBX mesh storage
	m_vecMeshes.resize(m_vecMeshNodePtrs.size());

	// if not _LOD1 size = 0
	if (m_vecMeshNodePtrs.size() < 1)
		return _log_action_error("FBX_Read::findSubmeshesRecursive(): No '_lod1' found in any submesh name!");

	return true;
}






bool FBX_Reader::sortLods()
{
	for (const auto& itMesh : m_vecMeshes)
	{
		// get next mesh name string
		string strMeshName = tools::lower(itMesh.m_strName);
		
		// place index at first "number character"
		size_t index_to_number = strMeshName.find("lod");

		if (index_to_number != string::npos)
		{
			// get number after chars "lod", convert from string to int
			string strFetchLodNumString = &strMeshName[index_to_number + 3];
			stringstream ss(strFetchLodNumString);
			int fetched_lod_number;
			ss >> fetched_lod_number;

			// resize [lod] if fetched_lod_numer > [lod][group]
			vecMeshesSorted.resize(vecMeshesSorted.size() < fetched_lod_number ? fetched_lod_number : vecMeshesSorted.size());

			vecMeshesSorted[fetched_lod_number - 1].push_back(itMesh);
		}
	}

	return true;
}


bool FBX_Reader::readNodeTransformations(fbxsdk::FbxMesh* _poMeshNode)
{
	_log_action("FBX_Reader::readNodeTransformations(): Getting sub mesh node trasnformations.");
	
	fbxsdk::FbxNode* poNode = _poMeshNode->GetNode();
	/*///////////////////////////////////////////////////////////////////////////////////////////
			mesh geomtric transformation
	///////////////////////////////////////////////////////////////////////////////////////////*/
	fbxsdk::FbxAMatrix matrixGeo;
	matrixGeo.SetIdentity(); // clear
	FbxVector4 vTranslationDest = poNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	FbxVector4 vTranslation = poNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	FbxVector4 vRotation = poNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	FbxVector4 lS = poNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	matrixGeo.SetT(vTranslation);
	matrixGeo.SetR(vRotation);
	matrixGeo.SetS(lS);
	/*///////////////////////////////////////////////////////////////////////////////////////////
		mesh NODE-		transformation
	///////////////////////////////////////////////////////////////////////////////////////////*/

	string strNodeName = poNode->GetName();
	
	
	FbxAMatrix MMeshPivot;
	_poMeshNode->GetPivot(MMeshPivot);
	FbxVector4 vLocalTranslate = poNode->LclTranslation.Get();
	FbxVector4 vLocalScale = poNode->LclScaling.Get();
	FbxVector4 vLocalRotate = poNode->LclRotation.Get();
	fbxsdk::FbxAMatrix globalMatrix = poNode->EvaluateGlobalTransform();

	FbxVector4 vGlobalTranslate = globalMatrix.GetT();
	FbxVector4 vGlobalScale = globalMatrix.GetS();
	FbxVector4 vGlobalRotate = globalMatrix.GetR();

	
	m_vecMeshes[m_mesh_index].normal_transformation_matrix.SetIdentity();
	m_vecMeshes[m_mesh_index].normal_transformation_matrix.SetR(globalMatrix.GetR());
	m_vecMeshes[m_mesh_index].normal_transformation_matrix.SetS(globalMatrix.GetS());

	FbxAMatrix mTempGeo;
	mTempGeo.SetIdentity();
	mTempGeo.SetR(matrixGeo.GetR());
	mTempGeo.SetS(matrixGeo.GetS());
	

	m_vecMeshes[m_mesh_index].transform_matrix = globalMatrix * matrixGeo;
	
	FbxVector4 vScaleTest = m_vecMeshes[m_mesh_index].transform_matrix.GetS();

	if (vScaleTest[0] < 0.0 || vScaleTest[1] < 0.0 || vScaleTest[2] < 0.0)
		m_vecMeshes[m_mesh_index].m_bNegativeScaling = true;

 	_log_action_success("FBX_Reader::readNodeTransformations(): Getting sub mesh node trasnformations.");
	return true;
}



bool FBX_Reader::calculateTagentData()
{
	// Copy data from the "united" vertex storage into its own sequential buffer 
	// for use the in DirectMesh functions:

	// for position 
	if (false)
	
	{

		vector<XMFLOAT3> pos(m_vecMeshes[m_mesh_index].vecPolygonVertices.size());
		for (size_t i = 0; i < m_vecMeshes[m_mesh_index].vecPolygonVertices.size(); i++)
		{
			pos[i] = m_vecMeshes[m_mesh_index].vecPolygonVertices[i].position;
		}

		// for uv
		vector<XMFLOAT2> uv(m_vecMeshes[m_mesh_index].vecPolygonVertices.size());
		for (size_t i = 0; i < m_vecMeshes[m_mesh_index].vecPolygonVertices.size(); i++)
		{
			uv[i].x = m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.x;
			uv[i].y = m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.y;

		}

		// for normal vectors
		vector<XMFLOAT3> normal(m_vecMeshes[m_mesh_index].vecPolygonVertices.size());
		for (size_t i = 0; i < m_vecMeshes[m_mesh_index].vecPolygonVertices.size(); i++)
		{
			normal[i] = m_vecMeshes[m_mesh_index].vecPolygonVertices[i].normal;
		}

		vector<XMFLOAT3> tangent(m_vecMeshes[m_mesh_index].vecPolygonVertices.size());
		vector<XMFLOAT3> bitangent(m_vecMeshes[m_mesh_index].vecPolygonVertices.size());

		HRESULT hrTangents = DirectX::ComputeTangentFrame(
			&m_vecMeshes[m_mesh_index].vecIndices32[0], m_vecMeshes[m_mesh_index].vecIndices32.size() / 3,
			&pos[0],
			&normal[0],
			&uv[0], m_vecMeshes[m_mesh_index].vecPolygonVertices.size(),
			&tangent[0],
			&bitangent[0]);

		if (!SUCCEEDED(hrTangents))
			return _log_action_error("FBX_Reader::calTangentData() tangen/bitangent calculation failed");

	}
	
	for (size_t i = 0; i < m_vecMeshes[m_mesh_index].vecPolygonVertices.size(); i++)
	{
	//	vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent = tangent[i];
//		vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent = bitangent[i];

		// -- check TBN orthogonality --
		// Load vectors into DirextX aligned "math structures"
		XMVECTOR vNormal = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[i].normal);
		XMVECTOR vTangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent);
		XMVECTOR vBitangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent);

		if (
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.x > 1.0 ||
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.x < 0.0 ||
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.y > 1.0 ||
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].textcords.y < 0.0

			)
			int debug_break_UV = 10;
		// perform the calculation: dot(cross(normal, tangent), bitangent)
		XMVECTOR scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

		// get dot product scalar out of directX math structure
		float _scalar = 0;
		XMStoreFloat(&_scalar, scalar);

		// for logging / console outputs, to avoid doing console write in the loop
		// and only do ONE "1 or more tagents were corrected" output
		bool bTangentInverted;

		//if (false) // DEBUGGIN CODE, "to correct or not to correct, that is the question."
		if (_scalar < 0.0f) // if dot prodict < 0.0, then invert tangent
		{
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.x *= -1;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.y *= -1;
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent.z *= -1;

			bTangentInverted = true;

			// DEBUGGING CODE: recheck TBN orthogonality
			vTangent = XMLoadFloat3(&m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent);
			scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

			// get dot product scalar out of directX math structure			
			XMStoreFloat(&_scalar, scalar);


			int debug_a = 10;
			// END: DEBUGGING CODE: recheck TBN orthogonality			
		}

		// swap tangent and bitangent around 
		XMFLOAT3 temp = m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent;
		
		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].tangent = 
			m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent;

		m_vecMeshes[m_mesh_index].vecPolygonVertices[i].bitangent = temp;
	}

	return true;

	//for (int i = 0; i < vecMeshes[m_mesh_index].vecControlPoints.size(); i += 3) {

	//	// Shortcuts for vertices
	//	XMVECTOR v0 = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 0].position);
	//	XMVECTOR v1 = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 1].position);
	//	XMVECTOR v2 = XMLoadFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 2].position);

	//	// Shortcuts for UVs
	//	XMVECTOR uv0 = XMLoadFloat2(&vecMeshes[m_mesh_index].vecControlPoints[i + 0].textcords);
	//	XMVECTOR uv1 = XMLoadFloat2(&vecMeshes[m_mesh_index].vecControlPoints[i + 1].textcords);
	//	XMVECTOR uv2 = XMLoadFloat2(&vecMeshes[m_mesh_index].vecControlPoints[i + 2].textcords);

	//	// Edges of the triangle : position delta
	//	XMVECTOR deltaPos1 = v1 - v0;
	//	XMVECTOR deltaPos2 = v2 - v0;

	//	// UV delta
	//	XMVECTOR deltaUV1 = uv1 - uv0;
	//	XMVECTOR deltaUV2 = uv2 - uv0;


	//	float r = 1.0f / (deltaUV1.m128_f32[0] * deltaUV2.m128_f32[1] - deltaUV1.m128_f32[1] * deltaUV2.m128_f32[0]);
	//	XMVECTOR  tangent = (deltaPos1 * deltaUV2.m128_f32[1] - deltaPos2 * deltaUV1.m128_f32[1]) * r;
	//	XMVECTOR  bitangent = (deltaPos2 * deltaUV1.m128_f32[0] - deltaPos1 * deltaUV2.m128_f32[0]) * r;

	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 0].tangent, tangent);
	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 0].bitangent, bitangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 0].tangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 0].bitangent);

	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 1].tangent, tangent);
	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 1].bitangent, bitangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 1].tangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 1].bitangent);


	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 2].tangent, tangent);
	//	XMStoreFloat3(&vecMeshes[m_mesh_index].vecControlPoints[i + 2].bitangent, bitangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 2].tangent);
	//	lib3d::normalize3(vecMeshes[m_mesh_index].vecControlPoints[i + 2].bitangent);





 //		int debug_a = 1;
	//}
}