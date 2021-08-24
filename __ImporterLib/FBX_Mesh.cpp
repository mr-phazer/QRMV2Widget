#include "FBX_Mesh.h"
#include "..\DirectXMesh\DirectXMesh.h"

bool FBX_Mesh::doCleanUp()
{

	
	_log_action("Peforming Remapping...");
	
	// copy high precision version to ´low precision for disk storage
	for (size_t i = 0; i < vecPolygonVertices.size(); i++)	
	{	
			vecVertices_RAW.push_back(
				vecPolygonVertices[i].getCommonRaw() );
	}	
	
	//return true;
	

	struct v
	{
		half x;
		half y;
		half z;

		uint8_t normal_x;
		uint8_t normal_y;
		uint8_t normal_z;

		half u, v;
	};

	vector<v> vecVerticesMin(vecVertices_RAW.size());
	for (size_t i = 0; i < vecVertices_RAW.size(); i++)
	{
		vecVerticesMin[i].x = vecVertices_RAW[i].pos_x;
		vecVerticesMin[i].y = vecVertices_RAW[i].pos_y;
		vecVerticesMin[i].z = vecVertices_RAW[i].pos_z;
				
		vecVerticesMin[i].normal_x = vecVertices_RAW[i].normal_x;
		vecVerticesMin[i].normal_y = vecVertices_RAW[i].normal_y;
		vecVerticesMin[i].normal_z = vecVertices_RAW[i].normal_z;

		vecVerticesMin[i].u = vecVertices_RAW[i].u;
		vecVerticesMin[i].v = vecVertices_RAW[i].v;
	}
	
	size_t start_total_indices = vecIndices32.size();
	size_t start_total_vertices = vecPolygonVertices.size();
	std::vector<unsigned int> remap(start_total_indices);

	// generate an index remap + get the nummber of UNIQUE vertices 	
	size_t unique_vertices = meshopt_generateVertexRemap<uint32_t>(
		&remap[0], 
		&vecIndices32[0], start_total_indices, &vecVerticesMin[0], start_total_vertices, sizeof(v));

	vector<uint32_t> mesh_indices(start_total_indices);
	meshopt_remapIndexBuffer(&mesh_indices[0], &vecIndices32[0], start_total_indices, &remap[0]);

	vector<RMV2_Vertex_Common_RAW> newVertices(unique_vertices);
	meshopt_remapVertexBuffer(&newVertices[0], &vecVertices_RAW[0],

		start_total_vertices, sizeof(RMV2_Vertex_Common_RAW), &remap[0]);

	_log_action("Remappped to " + to_string(unique_vertices) + " _acutually_ uinque vertice"
		+ "\n\r" + to_string(start_total_vertices - unique_vertices) + " redundants.");


	// optimized mesh
	vecVertices_RAW = newVertices;
	vecIndices32 = mesh_indices;


	return true;
	
	
	// ------------------------------------------------------------------------------

	vector<XMFLOAT3> pos(vecVertices_RAW.size());
	for (size_t i = 0; i < vecVertices_RAW.size(); i++)
	{
		pos[i].x = vecVertices_RAW[i].pos_x;
		pos[i].y = vecVertices_RAW[i].pos_y;
		pos[i].z = vecVertices_RAW[i].pos_z;		
	}

	// for uv
	vector<XMFLOAT2> uv(vecVertices_RAW.size());
	for (size_t i = 0; i < vecVertices_RAW.size(); i++)
	{
		uv[i].x = vecVertices_RAW[i].u;
		uv[i].y = vecVertices_RAW[i].v;

	}

	// for normal vectors
	vector<XMFLOAT3> normal(vecVertices_RAW.size());
	for (size_t i = 0; i < vecVertices_RAW.size(); i++)
	{
		normal[i].x = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_x);
		normal[i].y = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_y);
		normal[i].z = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_z);

	};



	vector<XMFLOAT4> tangent(vecVertices_RAW.size());
	vector<XMFLOAT3> bitangent(vecVertices_RAW.size());

	/*/////////////////////////////////////////////////////////////////////////////
			Calculate Tangents and Bitangents using DirectXMesh
	/////////////////////////////////////////////////////////////////////////////*/
	HRESULT hrTangents = DirectX::ComputeTangentFrame(
		&vecIndices32[0], vecIndices32.size() / 3,
		&pos[0],
		&normal[0],
		&uv[0], vecVertices_RAW.size(),
		&tangent[0],
		&bitangent[0]);

	if (!SUCCEEDED(hrTangents))
		return _log_action_error("FBX_Reader::calTangentData() tangen/bitangent calculation failed");

	for (size_t i = 0; i < bitangent.size(); i++)
	{

		/*tangent[i].x *= tangent[i].w;
		tangent[i].y *= tangent[i].w;
		tangent[i].z *= tangent[i].w;;*/

		//DirectX::XMFLOAT3 T;
		//T.x = tangent[i].x;
		//T.y = tangent[i].y;
		//T.z = tangent[i].z;

		//T = lib3d::normalize3(T);
		//tangent[i].x = T.x;
		//tangent[i].y = T.y;
		//tangent[i].z = T.z;
		normal[i] = lib3d::normalize3(normal[i]);
		vecVertices_RAW[i].normal_x = lib3d::float_to_unorm8(normal[i].x);
		vecVertices_RAW[i].normal_y = lib3d::float_to_unorm8(normal[i].y);
		vecVertices_RAW[i].normal_z = lib3d::float_to_unorm8(normal[i].z);
		vecVertices_RAW[i].normal_w = 0;

		
		//vecVertices_RAW[i].bitangent_x = lib3d::float_to_unorm8(bitangent[i].x * (-1.0f));
		//vecVertices_RAW[i].bitangent_y = lib3d::float_to_unorm8(bitangent[i].y * (-1.0f));
		//vecVertices_RAW[i].bitangent_z = lib3d::float_to_unorm8(bitangent[i].z * (-1.0f));


		bitangent[i].x *= tangent[i].w;
		//bitangent[i].y *= tangent[i].w;
		//bitangent[i].z *= tangent[i].w;
		bitangent[i] = lib3d::normalize3(bitangent[i]);
		vecVertices_RAW[i].bitangent_x = lib3d::float_to_unorm8(bitangent[i].x);
		vecVertices_RAW[i].bitangent_y = lib3d::float_to_unorm8(bitangent[i].y);
		vecVertices_RAW[i].bitangent_z = lib3d::float_to_unorm8(bitangent[i].z);
		vecVertices_RAW[i].bitangent_w = 0;

		/*if (tangent[i].w < 0)
		{
			vecVertices_RAW[i].bitangent_x = lib3d::float_to_unorm8(0.0f);
			vecVertices_RAW[i].bitangent_y = lib3d::float_to_unorm8(0.0f);
			vecVertices_RAW[i].bitangent_z = lib3d::float_to_unorm8(0.0f);
		}*/
		

		////float3 binormal = cross(normal, tangent.xyz) * tangent.w;
		//XMFLOAT3 T;
		//T.x = tangent[i].x;
		//T.y = tangent[i].y;
		//T.z = tangent[i].z;
		//XMVECTOR _tangent = XMLoadFloat3(&T);
		//XMVECTOR _normal = XMLoadFloat3(&normal[i]);

		//XMVECTOR _bitagent = XMVector3Cross(_normal, _tangent) * tangent[i].w;		
		//XMStoreFloat3(&bitangent[i], _bitagent);		

		
		//if (tangent[i].w < 0)
		//{
		//	vecVertices_RAW[i].tangent_x = lib3d::float_to_unorm8(tangent[i].x * (-1.0f));
		//	vecVertices_RAW[i].tangent_y = lib3d::float_to_unorm8(tangent[i].y * (-1.0f));
		//	vecVertices_RAW[i].tangent_z = lib3d::float_to_unorm8(tangent[i].z * (-1.0f));
		//	//vecVertices_RAW[i].tangent_w = lib3d::float_to_unorm8(0);
		//}
		//else

		tangent[i] = lib3d::normalize4_AsVec3(tangent[i]);
		//tangent[i] = lib3d::normalize3(tangent[i]);
		{
			vecVertices_RAW[i].tangent_x = lib3d::float_to_unorm8(tangent[i].x);
			vecVertices_RAW[i].tangent_y = lib3d::float_to_unorm8(tangent[i].y);
			vecVertices_RAW[i].tangent_z = lib3d::float_to_unorm8(tangent[i].z);
			vecVertices_RAW[i].tangent_w = 0;

		}
	}
	
	//if (false)
	for (size_t i = 0; i < bitangent.size(); i++)
	{
		XMVECTOR vNormal = XMLoadFloat3(&normal[i]);
		XMVECTOR vBitangent = XMLoadFloat3(&bitangent[i]);
		XMVECTOR vTangent = XMLoadFloat4(&tangent[i]);

		// perform the calculation: dot(cross(normal, tangent), bitangent)
		XMVECTOR scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

		// get dot product scalar out of directX math structure
		float _scalar = 0;
		XMStoreFloat(&_scalar, scalar);

		// for logging / console outputs, to avoid doing console write in the loop
		// and only do ONE "1 or more tagents were corrected" output
		bool bTangentInverted;

	//	if (false) // DEBUGGIN CODE, "to correct or not to correct, that is the question."
			if (_scalar < 0.0f) // if dot prodict < 0.0, then invert tangent
			{
				tangent[i].x *= -1;
				tangent[i].y *= -1;
				tangent[i].z *= -1;

				bTangentInverted = true;
 
				// DEBUGGING CODE: recheck TBN orthogonality
				vTangent = XMLoadFloat4(&tangent[i]);
				scalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(vNormal, vTangent), vBitangent);

				// get dot product scalar out of directX math structure			
				float _scalar_corrected = 0;
				XMStoreFloat(&_scalar_corrected, scalar);


				int debug_a = 10;
				// END: DEBUGGING CODE: recheck TBN orthogonality			
			}

		//XMFLOAT3 temp = vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent;

		//vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].tangent =
		//	vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent;

		//vecMeshes[m_mesh_index].vecPolygonVertices[vertex_index].bitangent = temp;
	}
	
	return true;

	//for (size_t i = 0; i < bitangent.size(); i++)
	//{


	//	DirectX::XMFLOAT3 T;
	//	T.x = tangent[i].x;
	//	T.y = tangent[i].y;
	//	T.z = tangent[i].z;

	//	T = lib3d::normalize3(T);
	//	tangent[i].x = T.x;
	//	tangent[i].y = T.y;
	//	tangent[i].z = T.z;


	//	vecVertices_RAW[i].tangent_x = vecVertices_RAW[i].bitangent_x;
	//	vecVertices_RAW[i].tangent_y = vecVertices_RAW[i].bitangent_y;
	//	vecVertices_RAW[i].tangent_z = vecVertices_RAW[i].bitangent_z;

	//	vecVertices_RAW[i].bitangent_x = lib3d::norm_float_to_byte(T.x);
	//	vecVertices_RAW[i].bitangent_y = lib3d::norm_float_to_byte(T.y);
	//	vecVertices_RAW[i].bitangent_z = lib3d::norm_float_to_byte(T.z);
	//}

	return true;
}

bool FBX_Mesh::calculateBoundingBox_RAW()
{
	for (size_t i = 0; i < vecVertices_RAW.size(); i++)
	{

		if (i == 0)
		{
			vMin.x = vecVertices_RAW[i].pos_x;
			vMin.y = vecVertices_RAW[i].pos_y;
			vMin.z = vecVertices_RAW[i].pos_z;

			vMax.x = vecVertices_RAW[i].pos_x;
			vMax.y = vecVertices_RAW[i].pos_y;
			vMax.z = vecVertices_RAW[i].pos_z;

		}

		vMin.x = std::min<float>(vecVertices_RAW[i].pos_x, vMin.x);
		vMin.y = std::min<float>(vecVertices_RAW[i].pos_y, vMin.y);
		vMin.z = std::min<float>(vecVertices_RAW[i].pos_z, vMin.z);

		vMax.x = std::max<float>(vecVertices_RAW[i].pos_x, vMax.x);
		vMax.y = std::max<float>(vecVertices_RAW[i].pos_y, vMax.y);
		vMax.z = std::max<float>(vecVertices_RAW[i].pos_z, vMax.z);
	}

	return true;
}


bool FBX_Mesh::simplify(float _threshold, float _target_error, bool _bUseSloppyIfStuck)
{

	if (vecIndices32.size() < 4)
		return false;

	_log_action("Performing Mesh Simplification (LOD generation");
	float threshold = _threshold;

	//if (m_vecRMV2_Vertices_Cinematic_Raw[_group].size() > 65536)
	//if (false)

	struct Vertex
	{
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};

	vector<Vertex> mesh_vertices(vecVertices_RAW.size());

	for (size_t i = 0; i < mesh_vertices.size(); i++)
	{
		mesh_vertices[i].px = vecVertices_RAW[i].pos_x;
		mesh_vertices[i].py = vecVertices_RAW[i].pos_y;
		mesh_vertices[i].pz = vecVertices_RAW[i].pos_z;

	
		mesh_vertices[i].nx = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_x);
		mesh_vertices[i].ny = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_y);
		mesh_vertices[i].nz = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_z);

		mesh_vertices[i].tx = vecVertices_RAW[i].u;
		mesh_vertices[i].ty = vecVertices_RAW[i].v;
	}

	vector<uint32_t> lod_indices;
	//vector<vertex> lod_vertices;

	vector<RMV2_Vertex_Common_RAW> lod_vertices;

	size_t target_index_count = size_t(vecIndices32.size() * threshold);
	float target_error = _target_error;

	//////////////////////////////////////////////////
	lod_indices.resize(vecIndices32.size()); // note: simplify needs space for index_count elements in the destination array, not target_index_count

	lod_indices.resize(
		meshopt_simplify<uint32_t>(&
			lod_indices[0], 
			&vecIndices32[0], 
			vecIndices32.size(),
			&mesh_vertices[0].px, 
			mesh_vertices.size(), 
			sizeof(Vertex), 
			target_index_count, 
			target_error
		));


	//lod_indices.resize(
	//	meshopt_simplifySloppy<uint32_t>(
	//		&lod_indices[0],
	//		&m_vecRMV2_Indices[_group][0],
	//		m_vecRMV2_Indices[_group].size(),
	//		&mesh_vertices[0].px,
	//		mesh_vertices.size(),
	//		sizeof(Vertex),
	//		target_index_count
	//		));

	lod_vertices.resize(lod_indices.size() < mesh_vertices.size() ? lod_indices.size() : mesh_vertices.size()); // note: this is just to reduce the cost of resize()

	lod_vertices.resize(meshopt_optimizeVertexFetch(
		&lod_vertices[0],
		&lod_indices[0],
		lod_indices.size(),
		&vecVertices_RAW[0],
		vecVertices_RAW.size(),
		sizeof(RMV2_Vertex_Common_RAW)));

	float vertex_reduction = 1.0f - ((float)lod_vertices.size() / (float)vecVertices_RAW.size());
	float index_reduction = 1.0f - ((float)lod_indices.size() / (float)vecIndices32.size());


	_log_action("Mesh simplifier, vertices: " + to_string(vertex_reduction * 100.0f) + "% reduction");
	_log_action("Mesh simplifier, indicies: " + to_string(index_reduction * 100.0f) + "% reduction");
	_log_action("From " + to_string(vecIndices32.size()) + " polygons (triangles) to " +
		to_string(lod_indices.size() / 3) + " polygons (triangles).");

	//m_vecRMV2_Vertices_Cinematic_Raw[_group].resize(lod_vertices.size());
	vecVertices_RAW = lod_vertices;

	//m_vecRMV2_Indices[_group].resize(lod_indices.size());
	vecIndices32 = lod_indices;


	if (_bUseSloppyIfStuck)
	if (target_index_count < vecIndices32.size())


	{
		float ratio = ((float)target_index_count) / ((float)vecIndices32.size());
	
		simplify_slopppy(ratio);
	}

	return true;
}

bool FBX_Mesh::simplify_slopppy(float _threshold, float _target_error)
{
	_log_action("Performing SLOPPY Mesh Simplification (LOD generation) to reach " + to_string(_threshold) + " polygon redcution ratio");
	float threshold = _threshold;

	//if (m_vecRMV2_Vertices_Cinematic_Raw[_group].size() > 65536)
	//if (false)

	struct Vertex
	{
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};

	vector<Vertex> mesh_vertices(vecVertices_RAW.size());

	for (size_t i = 0; i < mesh_vertices.size(); i++)
	{
		mesh_vertices[i].px = vecVertices_RAW[i].pos_x;
		mesh_vertices[i].py = vecVertices_RAW[i].pos_y;
		mesh_vertices[i].pz = vecVertices_RAW[i].pos_z;

		mesh_vertices[i].nx = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_x);
		mesh_vertices[i].ny = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_y);
		mesh_vertices[i].nz = lib3d::unorm8_to_float(vecVertices_RAW[i].normal_z);

		mesh_vertices[i].tx = vecVertices_RAW[i].u;
		mesh_vertices[i].ty = vecVertices_RAW[i].v;
	}

	vector<uint32_t> lod_indices;
	//vector<vertex> lod_vertices;

	vector<RMV2_Vertex_Common_RAW> lod_vertices;

	size_t target_index_count = size_t(vecIndices32.size() * threshold);
	float target_error = _target_error;

	//////////////////////////////////////////////////
	lod_indices.resize(vecIndices32.size()); // note: simplify needs space for index_count elements in the destination array, not target_index_count

	//lod_indices.resize(
	//	meshopt_simplify<uint32_t>(&
	//		lod_indices[0],
	//		&vecIndices32[0],
	//		vecIndices32.size(),
	//		&mesh_vertices[0].px,
	//		mesh_vertices.size(),
	//		sizeof(Vertex),
	//		target_index_count,
	//		target_error
	//		));

	lod_indices.resize(
		meshopt_simplifySloppy<uint32_t>(
			&lod_indices[0],
			&vecIndices32[0],
			vecIndices32.size(),
			&mesh_vertices[0].px,
			mesh_vertices.size(),
			sizeof(Vertex),
			target_index_count
			));

	lod_vertices.resize(lod_indices.size() < mesh_vertices.size() ? lod_indices.size() : mesh_vertices.size()); // note: this is just to reduce the cost of resize()
	
	lod_vertices.resize(meshopt_optimizeVertexFetch(
		&lod_vertices[0],
		&lod_indices[0],
		lod_indices.size(),
		&vecVertices_RAW[0],
		vecVertices_RAW.size(),
		sizeof(RMV2_Vertex_Common_RAW)));

	float vertex_reduction = 1.0f - ((float)lod_vertices.size() / (float)vecVertices_RAW.size());
	float index_reduction = 1.0f - ((float)lod_indices.size() / (float)vecIndices32.size());


	_log_action("Sloppy Mesh simplifier, vertices: " + to_string(vertex_reduction * 100.0f) + "% reduction");
	_log_action("Sloppy Mesh simplifier, indicies: " + to_string(index_reduction * 100.0f) + "% reduction");
	_log_action("From " + to_string(vecIndices32.size()) + " polygons (triangles) to " +
		to_string(lod_indices.size() / 3) + " polygons (triangles).");

	//m_vecRMV2_Vertices_Cinematic_Raw[_group].resize(lod_vertices.size());
	vecVertices_RAW = lod_vertices;

	//m_vecRMV2_Indices[_group].resize(lod_indices.size());
	vecIndices32 = lod_indices;

	// total ration ratio
	float ratio = ((float)vecIndices32.size()) / ((float)vecPolygonVertices.size());
	_log_action("Total reduction ration. " + to_string(ratio));
	

	return true;
}


