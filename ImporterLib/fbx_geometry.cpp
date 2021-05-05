#include "fbx_geometry.h"



map<string, vector<fbxsdk::FbxVector2>> LoadUVInformation(FbxMesh* pMesh)
{

    map<string, std::vector<FbxVector2>> mapvecRet;
    
    

    //get all UV set names
    FbxStringList lUVSetNameList;
    pMesh->GetUVSetNames(lUVSetNameList);

    //iterating over all uv sets
    

    for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
    {
        //get lUVSetIndex-th uv set
        const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

        // init map vector, string -> UV list
        mapvecRet[lUVSetName] = std::vector<FbxVector2>();

        if (!lUVElement)
            continue;

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
        {
            
            for (auto& it : mapvecRet)
            {
                it.second.clear();
            }
            mapvecRet.clear();

            return mapvecRet;
        
        }

        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

        //iterating through the data by polygon
        const int lPolyCount = pMesh->GetPolygonCount();

        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
        {
            for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
                {
                    FbxVector2 lUVValue;

                    //get the index of the current vertex in control points array
                    int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

                    //the UV index depends on the reference mode
                    int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

                    // store
                    mapvecRet[lUVSetName].push_back(lUVValue);

                }
            }
        }
        else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            int lPolyIndexCounter = 0;
            for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
                {
                    //if (lPolyIndexCounter < lIndexCount)
                    {
                        FbxVector2 lUVValue;

                        //the UV index depends on the reference mode
                        int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

                        // store
                        mapvecRet[lUVSetName].push_back(lUVValue);


                        lPolyIndexCounter++;
                    }
                }
            }
        }
    }

    return mapvecRet;
}



FbxVector4 multT(FbxNode* node, FbxVector4 vector) {

    FbxAMatrix matrixGeo;
    matrixGeo.SetIdentity();
    if (node->GetNodeAttribute())
    {
        const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
        const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
        const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
        matrixGeo.SetT(lT);
        matrixGeo.SetR(lR);
        matrixGeo.SetS(lS);
    }
    FbxAMatrix globalMatrix = node->EvaluateGlobalTransform();

    FbxAMatrix matrix = globalMatrix * matrixGeo;
    FbxVector4 result = matrix.MultT(vector);
    return result;
}

FbxVector4 multT_normal(FbxNode* node, FbxVector4 vector) {

    FbxAMatrix matrixGeo;
    matrixGeo.SetIdentity();
    if (node->GetNodeAttribute())
    {

        //const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
        const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
        //	const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
            //matrixGeo.SetT(lT);
        matrixGeo.SetROnly(lR);
        //matrixGeo.SetS(lS);
    }
    FbxAMatrix globalMatrix;
    globalMatrix.SetROnly(node->EvaluateGlobalTransform().GetROnly());


    FbxAMatrix matrix = matrixGeo;
    FbxVector4 result = matrix.MultT(vector);
    return result;
}
//get mesh normals info
vector<FbxVector4> GetNormals(fbxsdk::FbxNode* pNode)
{
	vector<FbxVector4> vecNormals;
	vecNormals.clear();

	if (!pNode)
		return vecNormals;

	//get mesh
	FbxMesh* lMesh = pNode->GetMesh();
	if (lMesh)
	{
		//get the normal element
		fbxsdk::FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();

		if (lNormalElement)
		{
			return GetVectors(pNode, lNormalElement);
		}

	}
}

vector<FbxVector4> GetTangents(FbxNode* pNode)
{
	vector<FbxVector4> vecTangents;
	vecTangents.clear();

	if (!pNode)
		return vecTangents;

	//get mesh
	FbxMesh* lMesh = pNode->GetMesh();
	if (lMesh)
	{
		//get the normal element
		fbxsdk::FbxGeometryElementTangent* pVectorElement = lMesh->GetElementTangent();

		if (pVectorElement)
		{
			return GetVectors(pNode, pVectorElement);
		}
	}

	return vecTangents;
}
vector<FbxVector4> GetBinormals(FbxNode* pNode)
{
	vector<FbxVector4> vecBinormals;
	vecBinormals.clear();

	if (!pNode)
		return vecBinormals;

	//get mesh
	FbxMesh* lMesh = pNode->GetMesh();
	if (lMesh)
	{

		//get the normal element
		fbxsdk::FbxGeometryElementBinormal* pVectorElement = lMesh->GetElementBinormal();

		//pVectorElement->SetMappingMode
		if (pVectorElement)
		{
			return GetVectors(pNode, pVectorElement);
		}
	}

	return vecBinormals;
}

vector<FbxVector4> GetVectors(FbxNode* pNode, FbxLayerElementTemplate<FbxVector4>* lNormalElement)
{
	vector<FbxVector4> vecNormals;
	vecNormals.clear();

	if (!pNode)
		return vecNormals;



	//get mesh
	FbxMesh* lMesh = pNode->GetMesh();
	if (lMesh)
	{
		//print mesh node name
		//FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

		//get the normal element
		//fbxsdk::FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();

		if (lNormalElement)
		{
			//mapping mode is by control points. The mesh should be smooth and soft.
			//we can get normals by retrieving each control point
			if (lNormalElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
			{
				int control_point_count = lMesh->GetControlPointsCount();
				vecNormals.resize(control_point_count);

				//Let's get normals of each vertex, since the mapping mode of normal element is by control point
				for (int lVertexIndex = 0; lVertexIndex < lMesh->GetControlPointsCount(); lVertexIndex++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as vertex index.
					//get normals by the index of control vertex
					if (lNormalElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
						lNormalIndex = lVertexIndex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == fbxsdk::FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

					//Got normals of each vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					//vecNormals.resize
					vecNormals[lVertexIndex] = lNormal;
					//	if (gVerbose) FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
						//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
						//. . .
				}//end for lVertexIndex
			}//end eByControlPoint
			//mapping mode is by polygon-vertex.
			//we can get normals by retrieving polygon-vertex.
			else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				int lIndexByPolygonVertex = 0;
				int polygon_count = lMesh->GetPolygonCount();
				//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
				for (int lPolygonIndex = 0; lPolygonIndex < polygon_count; lPolygonIndex++)
				{
					//get polygon size, you know how many vertices in current polygon.
					int lPolygonSize = lMesh->GetPolygonSize(lPolygonIndex);
					//retrieve each vertex of current polygon.
					for (int i = 0; i < lPolygonSize; i++)
					{
						int lNormalIndex = 0;
						//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
							lNormalIndex = lIndexByPolygonVertex;

						//reference mode is index-to-direct, get normals by the index-to-direct
						if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
							lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

						//Got normals of each polygon-vertex.
						FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);


						//	if (gVerbose) FBXSDK_printf("normals for polygon[%d]vertex[%d]: %f %f %f %f \n",
								//lPolygonIndex, i, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
							//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
							//. . .

						lIndexByPolygonVertex++;

						if (vecNormals.size() < lIndexByPolygonVertex + 1)
							vecNormals.resize(lIndexByPolygonVertex + 1);

						vecNormals.insert(vecNormals.begin() + lIndexByPolygonVertex, lNormal);
					}//end for i //lPolygonSize
				}//end for lPolygonIndex //PolygonCount

			}//end eByPolygonVertex
		}//end if lNormalElement

	}//end if lMesh

	return vecNormals;
}

//if (load_obj)
//{
//
//	loader.LoadFile(R"(c:\temp\cube.obj)");
//
//	vertex_count = loader.LoadedVertices.size();
//	//vertex_count = 6; // debug code
//	m_vecGroupHeader[group_counter].uiVertextCount = vertex_count;
//
//	m_vecVertices.resize(vertex_count);
//
//	for (size_t i = 0; i < vertex_count; i++)
//	{
//		m_vecVertices[i].position.x = loader.LoadedVertices[i].Position.X;
//		m_vecVertices[i].position.y = loader.LoadedVertices[i].Position.Y;
//		m_vecVertices[i].position.z = loader.LoadedVertices[i].Position.Z;
//
//		m_vecVertices[i].normal.x = loader.LoadedVertices[i].Normal.X;
//		m_vecVertices[i].normal.y = loader.LoadedVertices[i].Normal.Y;
//		m_vecVertices[i].normal.z = loader.LoadedVertices[i].Normal.Z;
//
//		m_vecVertices[i].tangent.x = loader.LoadedVertices[i].Normal.X;
//		m_vecVertices[i].tangent.y = loader.LoadedVertices[i].Normal.Y;
//		m_vecVertices[i].tangent.z = loader.LoadedVertices[i].Normal.Z;
//
//		m_vecVertices[i].bitangent.x = loader.LoadedVertices[i].Normal.X;
//		m_vecVertices[i].bitangent.y = loader.LoadedVertices[i].Normal.Y;
//		m_vecVertices[i].bitangent.z = loader.LoadedVertices[i].Normal.Z;
//
//		m_vecVertices[i].textcords.x = loader.LoadedVertices[i].TextureCoordinate.X;
//		m_vecVertices[i].textcords.y = loader.LoadedVertices[i].TextureCoordinate.Y;
//		//double test				    = (*poUVs)[i].mData[2];
//
//
//
//		int a = 1;
//	}
//}
/*if (load_obj)
			{
				index_count = loader.LoadedIndices.size();
				//index_count = 6; // debug code
				m_vecGroupHeader[group_counter].uiIndexCount = index_count;
				_vecIndices.resize(index_count);
				for (size_t i = 0; i < index_count; i++)
				{
					_vecIndices[i] = loader.LoadedIndices[i];
					/*_vecIndices[i*3] = loader.LoadedIndices[i*3];
					_vecIndices[i*3+2] = loader.LoadedIndices[i*3+1];
					_vecIndices[i*3+1] = loader.LoadedIndices[i*3+2];
				}
			}*/