#include "RigidModelV2_Data_Structs.h"
#include "..\RenderLib\lib3d.h"
#include "..\meshopt\meshoptimizer.h"
#include "..\QtTestRMV2\QConsole.h"

RigidModelV2::Common::GroupBlock* RigidModelV2::Common::CommonFile::getGroup(size_t _lod, size_t _group)
{
	return &vecLODs[_lod].vecGroups[_group];
}

void RigidModelV2::Common::CommonFile::updateFileFields_v5_v6()
{
	oFileHeader.dwLodCount = vecLODs.size();

	for (size_t lod = 0; lod < vecLODs.size(); lod++)
	{
		for (size_t group = 0; group < vecLODs[lod].vecGroups.size(); group++)
		{
			vecLODs[lod].vecGroups[group].updateGroupFields_v6_v7_v8();
		}
	}

	size_t start_offset = FileHeader::size_v6_v7_v8() + vecLODs.size() * LodHeaderElementCommon::size_v5_v6;

	oLodHeaderBlock.vecElements.resize(vecLODs.size());
	for (size_t lod = 0; lod < vecLODs.size(); lod++)
	{
		oLodHeaderBlock.vecElements[lod].dwGroupCount = vecLODs[lod].vecGroups.size();
		oLodHeaderBlock.vecElements[lod].dwStartOffset = start_offset;
		oLodHeaderBlock.vecElements[lod].dwIndicesDataLength = vecLODs[lod].getIndexDataSize();
		oLodHeaderBlock.vecElements[lod].dwVerticesDataLength = vecLODs[lod].getVertexDataSize();

		// if an lod distance is set us it, if not, use a default 100.0f (lod+1)
		oLodHeaderBlock.vecElements[lod].fVisibilityDistance =
			(vecLODs[lod].fLodDistance > 0.f) ? vecLODs[lod].fLodDistance : 100.0f * static_cast<float>(lod + 1);

		start_offset += vecLODs[lod].updateSize(); // update the group size and get the sum = LOD size, add it to offset
	}
}

void RigidModelV2::Common::CommonFile::updateFileFields_v7_v8()
{
	oFileHeader.dwLodCount = vecLODs.size();

	for (size_t lod = 0; lod < vecLODs.size(); lod++)
	{
		for (size_t group = 0; group < vecLODs[lod].vecGroups.size(); group++)
		{
			vecLODs[lod].vecGroups[group].updateGroupFields_v6_v7_v8();
		}
	}

	size_t start_offset = FileHeader::size_v6_v7_v8() + vecLODs.size() * LodHeaderElementCommon::size_v7_v8;

	oLodHeaderBlock.vecElements.resize(vecLODs.size());
	for (size_t lod = 0; lod < oLodHeaderBlock.vecElements.size(); lod++)
	{
		oLodHeaderBlock.vecElements[lod].dwGroupCount = vecLODs[lod].vecGroups.size();
		oLodHeaderBlock.vecElements[lod].dwStartOffset = start_offset;
		oLodHeaderBlock.vecElements[lod].dwIndicesDataLength = vecLODs[lod].getIndexDataSize();
		oLodHeaderBlock.vecElements[lod].dwVerticesDataLength = vecLODs[lod].getVertexDataSize();

		// if an lod distance is set us it, if not, use a default 100.0f (lod+1)
		oLodHeaderBlock.vecElements[lod].fVisibilityDistance =
			(vecLODs[lod].fLodDistance > 0.f) ? vecLODs[lod].fLodDistance : 100.0f * static_cast<float>(lod + 1);

		oLodHeaderBlock.vecElements[lod].dwAuthoredLodNumber = lod;
		oLodHeaderBlock.vecElements[lod].oQualityLevel = { 0 };

		start_offset += vecLODs[lod].updateSize(); // update the group size and get the sum = LOD size, add it to offset
	}
}

void RigidModelV2::Common::MeshBlock::unpackAs_Default(size_t _vertex_size, SimpleMath::Vector3 _vPivot, int32_t _bone_index)
{
	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		// cast the raw buffer with the vertex data to a cinematic raw vertex
		Vertex_Raw_Default* pv1_RAW = reinterpret_cast<Vertex_Raw_Default*>(&vecVertexDataRawBuffer[i * _vertex_size]);

		// "shortcut" to dest vertex
		auto& pv2 = vecVertices[i];

		pv2.position.x = (pv1_RAW->position.x + _vPivot.x);
		pv2.position.y = (pv1_RAW->position.y + _vPivot.y);
		pv2.position.z = (pv1_RAW->position.z + _vPivot.z);

		//auto mRotate = SimpleMath::Matrix::CreateRotationX(XM_PI / 2) * SimpleMath::Matrix::CreateRotationZ(XM_PI);
		//pv2.position = SimpleMath::Vector3::Transform(pv2.position, mRotate);

		//pv2.position += SimpleMath::Vector3::Transform(_vPivot, mRotate);

		//pv2.position.w = pv1_RAW->position.w;

		//pv2.position += _vPivot;
		// add the pivot vector to translated the origin.

		pv2.uv.x = pv1_RAW->uv1.x;
		pv2.uv.y = pv1_RAW->uv1.y;

		/*pv2.tex2.x = pv1_RAW->uv2.x;
		pv2.tex2.y = pv1_RAW->uv2.y;*/

		pv2.normal.x = lib3d::unorm8_to_float(pv1_RAW->normal.x);
		pv2.normal.y = lib3d::unorm8_to_float(pv1_RAW->normal.y);
		pv2.normal.z = lib3d::unorm8_to_float(pv1_RAW->normal.z);
		//pv2.normal.w = lib3d::unorm8_to_float(pv1_RAW->normal.w);

		//pv2.normal = SimpleMath::Vector3::Transform(pv2.normal, mRotate);

		pv2.tangent.x = lib3d::unorm8_to_float(pv1_RAW->tangent.x);
		pv2.tangent.y = lib3d::unorm8_to_float(pv1_RAW->tangent.y);
		pv2.tangent.z = lib3d::unorm8_to_float(pv1_RAW->tangent.z);
		//pv2.tangent.w = lib3d::unorm8_to_float(pv1_RAW->tangent.w);
		//pv2.tangent = SimpleMath::Vector3::Transform(pv2.tangent, mRotate);

		pv2.bitangent.x = lib3d::unorm8_to_float(pv1_RAW->bitangent.x);
		pv2.bitangent.y = lib3d::unorm8_to_float(pv1_RAW->bitangent.y);
		pv2.bitangent.z = lib3d::unorm8_to_float(pv1_RAW->bitangent.z);
		//pv2.bitangent.w = lib3d::unorm8_to_float(pv1_RAW->bitangent.w);
		//pv2.bitangent = SimpleMath::Vector3::Transform(pv2.bitangent, mRotate);

		if (_bone_index > -1)
		{
			pv2.bone_indices.x = _bone_index;
			pv2.bone_indices.y = 0;
			pv2.bone_indices.z = 0;
			pv2.bone_indices.w = 0;

			pv2.bone_weights.x = 1.0f;
			pv2.bone_weights.y = 0.f;
			pv2.bone_weights.z = 0.f;
			pv2.bone_weights.w = 0.f;
		}
		else
		{
			pv2.bone_indices.x = 0;
			pv2.bone_indices.y = 0;
			pv2.bone_indices.z = 0;
			pv2.bone_indices.w = 0;

			pv2.bone_weights.x = 1.0f;
			pv2.bone_weights.y = 0.f;
			pv2.bone_weights.z = 0.f;
			pv2.bone_weights.w = 0.f;
		}
	}
}

void RigidModelV2::Common::MeshBlock::unpackAs_Weighted2_V5_V6_V7(size_t _vertex_size)
{
	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		// cast the raw buffer with the vertex data to a cinematic raw vertex
		Vertex_Raw_Weighted* pv1_RAW = reinterpret_cast<Vertex_Raw_Weighted*>(&vecVertexDataRawBuffer[i * _vertex_size]);

		// "shortcut" to dest vertex
		auto& pv2 = vecVertices[i];

		pv2.position.x = pv1_RAW->position.x;
		pv2.position.y = pv1_RAW->position.y;
		pv2.position.z = pv1_RAW->position.z;
		//pv2.position.w = pv1_RAW->position.w;

		pv2.uv.x = pv1_RAW->uv.x;
		pv2.uv.y = pv1_RAW->uv.y;

		pv2.normal.x = lib3d::unorm8_to_float(pv1_RAW->normal.x);
		pv2.normal.y = lib3d::unorm8_to_float(pv1_RAW->normal.y);
		pv2.normal.z = lib3d::unorm8_to_float(pv1_RAW->normal.z);
		//pv2.normal.w = lib3d::unorm8_to_float(pv1_RAW->normal.w);

		pv2.tangent.x = lib3d::unorm8_to_float(pv1_RAW->tangent.x);
		pv2.tangent.y = lib3d::unorm8_to_float(pv1_RAW->tangent.y);
		pv2.tangent.z = lib3d::unorm8_to_float(pv1_RAW->tangent.z);
		//pv2.tangent.w = lib3d::unorm8_to_float(pv1_RAW->tangent.w);

		pv2.bitangent.x = lib3d::unorm8_to_float(pv1_RAW->bitangent.x);
		pv2.bitangent.y = lib3d::unorm8_to_float(pv1_RAW->bitangent.y);
		pv2.bitangent.z = lib3d::unorm8_to_float(pv1_RAW->bitangent.z);
		//2.bitangent.w = lib3d::unorm8_to_float(pv1_RAW->bitangent.w);

		//(pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x)

		pv2.bone_indices.x = pv1_RAW->bone_indices.x;
		pv2.bone_indices.y = pv1_RAW->bone_indices.y;
		pv2.bone_indices.z = 0;
		pv2.bone_indices.w = 0;

		pv2.bone_weights.x = static_cast<float>(pv1_RAW->bone_weights.x) / 255.0;
		pv2.bone_weights.y = 1.f - pv2.bone_weights.x;
		pv2.bone_weights.z = 0.f;
		pv2.bone_weights.w = 0.f;
	}
};

void RigidModelV2::Common::MeshBlock::unpackAs_Weighted4_V5_V6_V7(size_t _vertex_size)
{
	//auto& oMeshBlock = m_spoData->vecLODs[_lod].vecGroups[_group].oMeshBlock;

	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		// cast the raw buffer with the vertex data to a cinematic raw vertex
		Vertex_Raw_Cinematic* pv1_RAW = reinterpret_cast<Vertex_Raw_Cinematic*>(&vecVertexDataRawBuffer[i * 32]);

		// "shortcut" to dest vertex
		auto& pv2 = vecVertices[i];

		pv2.position.x = pv1_RAW->position.x;
		pv2.position.y = pv1_RAW->position.y;
		pv2.position.z = pv1_RAW->position.z;
		//		pv2.position.w = pv1_RAW->position.w;

		pv2.uv.x = pv1_RAW->uv.x;
		pv2.uv.y = pv1_RAW->uv.y;

		pv2.normal.x = lib3d::unorm8_to_float(pv1_RAW->normal.x);
		pv2.normal.y = lib3d::unorm8_to_float(pv1_RAW->normal.y);
		pv2.normal.z = lib3d::unorm8_to_float(pv1_RAW->normal.z);
		//2.normal.w = lib3d::unorm8_to_float(pv1_RAW->normal.w);

		pv2.tangent.x = lib3d::unorm8_to_float(pv1_RAW->tangent.x);
		pv2.tangent.y = lib3d::unorm8_to_float(pv1_RAW->tangent.y);
		pv2.tangent.z = lib3d::unorm8_to_float(pv1_RAW->tangent.z);
		//2.tangent.w = lib3d::unorm8_to_float(pv1_RAW->tangent.w);

		pv2.bitangent.x = lib3d::unorm8_to_float(pv1_RAW->bitangent.x);
		pv2.bitangent.y = lib3d::unorm8_to_float(pv1_RAW->bitangent.y);
		pv2.bitangent.z = lib3d::unorm8_to_float(pv1_RAW->bitangent.z);
		//pv2.bitangent.w = lib3d::unorm8_to_float(pv1_RAW->bitangent.w);

		if ((pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x))
			int debug_1 = 1;

		pv2.bone_indices.x = pv1_RAW->bone_indices.x;
		pv2.bone_indices.y = (pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.y;
		pv2.bone_indices.z = (pv1_RAW->bone_indices.z == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.z;
		pv2.bone_indices.w = (pv1_RAW->bone_indices.w == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.w;

		pv2.bone_weights.x = static_cast<float>(pv1_RAW->bone_weights.x) / 255.0;
		pv2.bone_weights.y = static_cast<float>(pv1_RAW->bone_weights.y) / 255.0;
		pv2.bone_weights.z = static_cast<float>(pv1_RAW->bone_weights.z) / 255.0;
		pv2.bone_weights.w = static_cast<float>(pv1_RAW->bone_weights.w) / 255.0;

		memcpy(&vecVertexDataRawBuffer[i * _vertex_size], &pv1_RAW, _vertex_size);
	}
}

void RigidModelV2::Common::MeshBlock::makePacked_Default_V5_V6_V7(size_t _vertex_size)
{
	// clear, and alocate bytes needed to hold raw vertex data
	vecVertexDataRawBuffer.clear();
	vecVertexDataRawBuffer.resize(vecVertices.size() * _vertex_size);

	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		Vertex_Raw_Default vRaw;
		auto& vSrc = vecVertices[i];

		vRaw.position.x = vSrc.position.x;
		vRaw.position.y = vSrc.position.y;
		vRaw.position.z = vSrc.position.z;
		vRaw.position.w = 0;

		vRaw.uv1.x = vSrc.uv.x;
		vRaw.uv1.y = vSrc.uv.y;

		vRaw.uv2.x = vSrc.uv.x;
		vRaw.uv2.y = vSrc.uv.y;

		vRaw.normal.x = lib3d::float_to_unorm8(vSrc.normal.x);
		vRaw.normal.y = lib3d::float_to_unorm8(vSrc.normal.y);
		vRaw.normal.z = lib3d::float_to_unorm8(vSrc.normal.z);
		vRaw.normal.w = lib3d::float_to_unorm8(0.f);

		vRaw.tangent.x = lib3d::float_to_unorm8(vSrc.tangent.x);
		vRaw.tangent.y = lib3d::float_to_unorm8(vSrc.tangent.y);
		vRaw.tangent.z = lib3d::float_to_unorm8(vSrc.tangent.z);
		vRaw.tangent.w = lib3d::float_to_unorm8(0.f);

		vRaw.bitangent.x = lib3d::float_to_unorm8(vSrc.bitangent.x);
		vRaw.bitangent.y = lib3d::float_to_unorm8(vSrc.bitangent.y);
		vRaw.bitangent.z = lib3d::float_to_unorm8(vSrc.bitangent.z);
		vRaw.bitangent.w = lib3d::float_to_unorm8(0.f);

		//vRaw.bone_indices.x = vSrc.bone_indices.x;
		//vRaw.bone_indices.y = vSrc.bone_indices.y;
		//vRaw.bone_indices.z = vSrc.bone_indices.z;
		//vRaw.bone_indices.w = vSrc.bone_indices.w;

		//vRaw.bone_weights.x = max < float>(255.0f, (vSrc.bone_weights.x * 255.0f));
		//vRaw.bone_weights.y = max < float>(255.0f, (vSrc.bone_weights.y * 255.0f));
		//vRaw.bone_weights.z = max < float>(255.0f, (vSrc.bone_weights.z * 255.0f));
		//vRaw.bone_weights.w = max < float>(255.0f, (vSrc.bone_weights.w * 255.0f));

		///*
		//	set the indices, like in the vanilla models
		//	if weighted it 1.0 for weight 1
		//	make index 2, 3, 4 = index 1

		//*/
		//if (vRaw.bone_weights.x = 255)
		//{
		//	vRaw.bone_indices.y = vRaw.bone_indices.x;
		//	vRaw.bone_indices.z = vRaw.bone_indices.x;
		//	vRaw.bone_indices.w = vRaw.bone_indices.x;
		//}

		memcpy(&vecVertexDataRawBuffer[i * _vertex_size], &vRaw, _vertex_size);
	}
}

void RigidModelV2::Common::MeshBlock::makePacked_Weighted2_V5_V6_V7(size_t _vertex_size)
{
	// clear, and alocate bytes needed to hold raw vertex data
	vecVertexDataRawBuffer.clear();
	vecVertexDataRawBuffer.resize(vecVertices.size() * _vertex_size);

	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		Vertex_Raw_Weighted vRaw;
		auto& vSrc = vecVertices[i];

		vRaw.position.x = vSrc.position.x;
		vRaw.position.y = vSrc.position.y;
		vRaw.position.z = vSrc.position.z;
		vRaw.position.w = 0;

		vRaw.uv.x = vSrc.uv.x;
		vRaw.uv.y = vSrc.uv.y;

		vRaw.normal.x = lib3d::float_to_unorm8(vSrc.normal.x);
		vRaw.normal.y = lib3d::float_to_unorm8(vSrc.normal.y);
		vRaw.normal.z = lib3d::float_to_unorm8(vSrc.normal.z);
		vRaw.normal.w = lib3d::float_to_unorm8(0.f);

		vRaw.tangent.x = lib3d::float_to_unorm8(vSrc.tangent.x);
		vRaw.tangent.y = lib3d::float_to_unorm8(vSrc.tangent.y);
		vRaw.tangent.z = lib3d::float_to_unorm8(vSrc.tangent.z);
		vRaw.tangent.w = lib3d::float_to_unorm8(0.f);

		vRaw.bitangent.x = lib3d::float_to_unorm8(vSrc.bitangent.x);
		vRaw.bitangent.y = lib3d::float_to_unorm8(vSrc.bitangent.y);
		vRaw.bitangent.z = lib3d::float_to_unorm8(vSrc.bitangent.z);
		vRaw.bitangent.w = lib3d::float_to_unorm8(0.f);

		vRaw.bone_indices.x = vSrc.bone_indices.x;
		vRaw.bone_indices.y = vSrc.bone_indices.y;
		//vRaw.bone_indices.z = vSrc.bone_indices.z;
		//vRaw.bone_indices.w = vSrc.bone_indices.w;

		vRaw.bone_weights.x = min < float>(255.0f, (vSrc.bone_weights.x * 255.0f));
		vRaw.bone_weights.y = min < float>(255.0f, (vSrc.bone_weights.y * 255.0f));
		//vRaw.bone_weights.z = max < float>(255.0f, (vSrc.bone_weights.z * 255.0f));
		//vRaw.bone_weights.w = max < float>(255.0f, (vSrc.bone_weights.w * 255.0f));

		/*
			set the indices, like in the vanilla models
			if weighted it 1.0 for weight 1
			make index 2, 3, 4 = index 1

		*/
		if (vRaw.bone_weights.x = 255)
		{
			vRaw.bone_indices.y = vRaw.bone_indices.x;
			//vRaw.bone_indices.z = vRaw.bone_indices.x;
			//vRaw.bone_indices.w = vRaw.bone_indices.x;
		}

		memcpy(&vecVertexDataRawBuffer[i * _vertex_size], &vRaw, _vertex_size);
	}
}

void RigidModelV2::Common::MeshBlock::makePacked_Weighted4_V5_V6_V7(size_t _vertex_size)
{
	// clear, and alocate bytes needed to hold raw vertex data
	vecVertexDataRawBuffer.clear();
	vecVertexDataRawBuffer.resize(vecVertices.size() * _vertex_size);

	for (size_t i = 0; i < vecVertices.size(); i++)
	{
		Vertex_Raw_Cinematic vRaw;
		auto& vSrc = vecVertices[i];

		vRaw.position.x = vSrc.position.x;
		vRaw.position.y = vSrc.position.y;
		vRaw.position.z = vSrc.position.z;
		vRaw.position.w = 0;

		vRaw.uv.x = vSrc.uv.x;
		vRaw.uv.y = vSrc.uv.y;

		vRaw.normal.x = lib3d::float_to_unorm8(vSrc.normal.x);
		vRaw.normal.y = lib3d::float_to_unorm8(vSrc.normal.y);
		vRaw.normal.z = lib3d::float_to_unorm8(vSrc.normal.z);
		vRaw.normal.w = lib3d::float_to_unorm8(0.f);

		vRaw.tangent.x = lib3d::float_to_unorm8(vSrc.tangent.x);
		vRaw.tangent.y = lib3d::float_to_unorm8(vSrc.tangent.y);
		vRaw.tangent.z = lib3d::float_to_unorm8(vSrc.tangent.z);
		vRaw.tangent.w = lib3d::float_to_unorm8(0.f);

		vRaw.bitangent.x = lib3d::float_to_unorm8(vSrc.bitangent.x);
		vRaw.bitangent.y = lib3d::float_to_unorm8(vSrc.bitangent.y);
		vRaw.bitangent.z = lib3d::float_to_unorm8(vSrc.bitangent.z);
		vRaw.bitangent.w = lib3d::float_to_unorm8(0.f);

		vRaw.bone_weights.x = static_cast<uint8_t>(min<float>(vSrc.bone_weights.x * 255.0f, 255.0f));
		vRaw.bone_weights.y = static_cast<uint8_t>(min<float>(vSrc.bone_weights.y * 255.0f, 255.0f));
		vRaw.bone_weights.z = static_cast<uint8_t>(min<float>(vSrc.bone_weights.z * 255.0f, 255.0f));
		vRaw.bone_weights.w = static_cast<uint8_t>(min<float>(vSrc.bone_weights.w * 255.0f, 255.0f));

		vRaw.bone_indices.x = vSrc.bone_indices.x;
		vRaw.bone_indices.y = (vRaw.bone_weights.x == 255) ? vRaw.bone_indices.x : vSrc.bone_indices.y;
		vRaw.bone_indices.z = (vRaw.bone_weights.x == 255) ? vRaw.bone_indices.x : vSrc.bone_indices.z;
		vRaw.bone_indices.w = (vRaw.bone_weights.x == 255) ? vRaw.bone_indices.x : vSrc.bone_indices.w;

		/*
			set the indices, like in the vanilla models
			if weighted it 1.0 for weight 1
			make index 2, 3, 4 = index 1

		*/
		/*if (vRaw.bone_weights.x = 255)
		{
			vRaw.bone_indices.y = vRaw.bone_indices.x;
			vRaw.bone_indices.z = vRaw.bone_indices.x;
			vRaw.bone_indices.w = vRaw.bone_indices.x;
		}*/

		memcpy(&vecVertexDataRawBuffer[i * _vertex_size], &vRaw, _vertex_size);
	}
}

void RigidModelV2::Common::GroupBlock::setMesh_v6_v7_v8(std::vector<ModelVertex>& _vecVertices, std::vector<uint16_t>& _vecIndices)
{
	oMeshBlock.vecVertices = _vecVertices;
	oMeshBlock.vecIndices = _vecIndices;

	throw invalid_argument(""); // TODO: Insert conversion of "unpacked" mesh to "packed" mesh

	//updateGroupFields_v6_v7_v8();
}

void RigidModelV2::Common::GroupBlock::setTextures(const std::vector<TextureElement>& _vecTextures)
{
	oMaterialBlock.vecTextures = _vecTextures;
}

size_t RigidModelV2::Common::GroupBlock::getVertexSize()
{
	if (oPreHeader.dwVertexCount == 0)
		return 0;

	return vertex_size;
}

size_t RigidModelV2::Common::GroupBlock::calculateVertexSize()
{
	vertex_size = (
		oPreHeader.dwVertexData_TextAndAttach_BlockSize -
		oPreHeader.uiTextureAndAttchmentBlockSize
		) / oPreHeader.dwVertexCount;

	return vertex_size;
}

void RigidModelV2::Common::GroupBlock::setVertexSize(size_t _size)
{
	vertex_size = _size;
}

size_t RigidModelV2::Common::GroupBlock::getDataSizeVertices()
{
	return
		oPreHeader.dwVertexCount * getVertexSize();
}

size_t RigidModelV2::Common::GroupBlock::getDataSizeIndices()
{
	return
		oPreHeader.dwIndexCount * 2U;
}

size_t RigidModelV2::Common::GroupBlock::updateGroupFields_v6_v7_v8()
{
	// set new values for mesh
	oPreHeader.dwVertexCount = oMeshBlock.vecVertices.size();
	oPreHeader.dwIndexCount = oMeshBlock.vecIndices.size();

	// set attachment point and texture counts
	oSubMeshHeader.dwAttachmentPointCount = oAttachBlock.vecAttachmentPoints.size();
	oSubMeshHeader.dwTextureCount = oMaterialBlock.vecTextures.size();

	// this is the extra data that some RigidMaterial need/make
	size_t extra_material_info = oExtraMaterialBlock.vecExtraMaterialData.size(); // extra material data;

	oPreHeader.uiTextureAndAttchmentBlockSize = 940L + oAttachBlock.size_v6_v7_v8() + oMaterialBlock.size_v6_v7_v8() +
		extra_material_info;

	size_t v_size = getVertexSize();

	size_t text_vert_attch_before = oPreHeader.dwVertexData_TextAndAttach_BlockSize;
	oPreHeader.dwVertexData_TextAndAttach_BlockSize =
		oPreHeader.uiTextureAndAttchmentBlockSize +
		oMeshBlock.vecVertices.size() * v_size;

	oPreHeader.dwGroupSize =
		oPreHeader.dwVertexData_TextAndAttach_BlockSize +
		(oMeshBlock.vecIndices.size() * 2);

	return oPreHeader.dwGroupSize;
}

size_t RigidModelV2::Common::ExtraMaterialDataBlock::sizeFromMaterialId(ERigidMaterial Material)
{
	switch (Material)
	{
	case ERigidMaterial::dirtmap:
	case ERigidMaterial::weighted_dirtmap:
	case ERigidMaterial::weighted_skin_dirtmap: return 32;

	case ERigidMaterial::weighted_skin_decal:
	case ERigidMaterial::weighted_decal:
	case ERigidMaterial::decal: return 20;

	case ERigidMaterial::decal_dirtmap:
	case ERigidMaterial::weighted_decal_dirtmap:
	case ERigidMaterial::weighted_skin_decal_dirtmap: return 52;

	case ERigidMaterial::tree: return 56;

	default:
		return 0;
	}
}

void RigidModelV2::Common::GroupBlock::simplifyGroup(float _threshold, float _error, bool _bUseSloppy)
{
	_log_action("Mesh simplifier: Grouop '" + string(this->oSubMeshHeader.szGroupName) + "'...");

	//Simplygon::ISimplygon* sg = NULL;
	//Simplygon::EErrorCodes initval = Simplygon::Initialize(&sg);
	//if (initval != Simplygon::EErrorCodes::NoError)
	//{
	//	return int(initval);
	//}

	//RunReduction(sg);

	//Simplygon::Deinitialize(sg);

	struct Vertex
	{
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};

	vector<Vertex> mesh_vertices(oMeshBlock.vecVertices.size());

	for (size_t i = 0; i < mesh_vertices.size(); i++)
	{
		mesh_vertices[i].px = oMeshBlock.vecVertices[i].position.x;
		mesh_vertices[i].py = oMeshBlock.vecVertices[i].position.y;
		mesh_vertices[i].pz = oMeshBlock.vecVertices[i].position.z;

		mesh_vertices[i].nx = oMeshBlock.vecVertices[i].normal.x;
		mesh_vertices[i].ny = oMeshBlock.vecVertices[i].normal.y;
		mesh_vertices[i].nz = oMeshBlock.vecVertices[i].normal.z;

		mesh_vertices[i].tx = oMeshBlock.vecVertices[i].uv.x;
		mesh_vertices[i].ty = oMeshBlock.vecVertices[i].uv.y;
	}

	vector<ModelVertex> lod_vertices;
	vector<uint16_t> lod_indices;

	size_t target_index_count = size_t(oMeshBlock.vecIndices.size() * _threshold);

	//////////////////////////////////////////////////
	lod_indices.resize(oMeshBlock.vecIndices.size()); // note: simplify needs space for index_count elements in the destination array, not target_index_count

	lod_indices.resize(
		meshopt_simplify(
		&lod_indices[0],
		&oMeshBlock.vecIndices[0],
		oMeshBlock.vecIndices.size(),
		&mesh_vertices[0].px,
		mesh_vertices.size(),
		sizeof(Vertex),
		target_index_count,
		_error
	));

	lod_vertices.resize(lod_indices.size() < mesh_vertices.size() ? lod_indices.size() : mesh_vertices.size()); // note: this is just to reduce the cost of resize()

	lod_vertices.resize(meshopt_optimizeVertexFetch(
		&lod_vertices[0],
		&lod_indices[0],
		lod_indices.size(),
		&oMeshBlock.vecVertices[0],
		oMeshBlock.vecVertices.size(),
		sizeof(ModelVertex)));

	float vertex_reduction = 1.0f - ((float)lod_vertices.size() / (float)oMeshBlock.vecVertices.size());
	float index_reduction = 1.0f - ((float)lod_indices.size() / (float)oMeshBlock.vecIndices.size());

	_log_action("Mesh simplifier, vertices: " + to_string(vertex_reduction * 100.0f) + "% reduction");
	_log_action("Mesh simplifier, indicies: " + to_string(index_reduction * 100.0f) + "% reduction");
	_log_action("From " +
		to_string(oMeshBlock.vecIndices.size() / 3) +
		" polygons (triangles) to " +
		to_string(lod_indices.size() / 3) +
		" polygons (triangles).");

	oMeshBlock.vecIndices = lod_indices;
	oMeshBlock.vecVertices = lod_vertices;

	return;

	if (_bUseSloppy && (lod_indices.size() > target_index_count))
	{
		mesh_vertices.clear();
		mesh_vertices.resize(oMeshBlock.vecVertices.size());

		for (size_t i = 0; i < mesh_vertices.size(); i++)
		{
			mesh_vertices[i].px = oMeshBlock.vecVertices[i].position.x;
			mesh_vertices[i].py = oMeshBlock.vecVertices[i].position.y;
			mesh_vertices[i].pz = oMeshBlock.vecVertices[i].position.z;

			mesh_vertices[i].nx = oMeshBlock.vecVertices[i].normal.x;
			mesh_vertices[i].ny = oMeshBlock.vecVertices[i].normal.y;
			mesh_vertices[i].nz = oMeshBlock.vecVertices[i].normal.z;

			mesh_vertices[i].tx = oMeshBlock.vecVertices[i].uv.x;
			mesh_vertices[i].ty = oMeshBlock.vecVertices[i].uv.y;
		}

		lod_indices.clear();
		lod_indices.resize(target_index_count); // note: simplifySloppy, unlike simplify, is guaranteed to output results that don't exceed the requested target_index_count
		lod_indices.resize(meshopt_simplifySloppy(&lod_indices[0], &oMeshBlock.vecIndices[0], oMeshBlock.vecIndices.size(), &mesh_vertices[0].px, mesh_vertices.size(), sizeof(Vertex), target_index_count));

		lod_vertices.clear();
		lod_vertices.resize(lod_indices.size() < mesh_vertices.size() ? lod_indices.size() : mesh_vertices.size()); // note: this is just to reduce the cost of resize()

		lod_vertices.resize(meshopt_optimizeVertexFetch(
			&lod_vertices[0],
			&lod_indices[0],
			lod_indices.size(),
			&oMeshBlock.vecVertices[0],
			oMeshBlock.vecVertices.size(),
			sizeof(ModelVertex)));

		//	lod_vertices.resize(meshopt_optimizeVertexFetch(&lod_vertices[0], &lod_indices[0], lod_indices.size(), &mesh_vertices[0], mesh_vertices.size(), sizeof(ModelVertex)));

		oMeshBlock.vecIndices = lod_indices;
		oMeshBlock.vecVertices = lod_vertices;
	}
}

void RigidModelV2::Common::GroupBlock::cleanUpMesh()
{
	size_t start_total_indices = this->oMeshBlock.vecIndices.size();
	size_t start_total_vertices = this->oMeshBlock.vecVertices.size();

	//std::vector<unsigned int> remap(start_total_indices);

	//// generate an index remap + get the nummber of UNIQUE vertices
	//size_t unique_vertices = meshopt_generateVertexRemap<uint16_t>(
	//	&remap[0],
	//	&oMeshBlock.vecIndices[0], start_total_indices, &oMeshBlock.vecVertices[0], start_total_vertices, sizeof(ModelVertex));

	//vector<uint16_t> newIndices(start_total_indices);
	//meshopt_remapIndexBuffer<uint16_t>(&newIndices[0], &oMeshBlock.vecIndices[0], start_total_indices, &remap[0]);

	//vector<ModelVertex> newVertices(unique_vertices);
	//meshopt_remapVertexBuffer(&newVertices[0], &oMeshBlock.vecVertices[0],
	//	start_total_vertices, sizeof(ModelVertex), &remap[0]);

	//oMeshBlock.vecVertices = newVertices;
	//oMeshBlock.vecIndices = newIndices;

	//if (oMeshBlock.vecIndices.size() < 4)
	//	return;

	_log_action("Performing Mesh Simplification (LOD generation).");
	float threshold = 0.1;

	//if (m_vecRMV2_Vertices_Cinematic_Raw[_group].size() > 65536)
	//if (false)

	struct Vertex
	{
		float px, py, pz;
		float nx, ny, nz;
		float tx, ty;
	};

	vector<Vertex> mesh_vertices(oMeshBlock.vecVertices.size());

	for (size_t i = 0; i < mesh_vertices.size(); i++)
	{
		mesh_vertices[i].px = oMeshBlock.vecVertices[i].position.x;
		mesh_vertices[i].py = oMeshBlock.vecVertices[i].position.y;
		mesh_vertices[i].pz = oMeshBlock.vecVertices[i].position.z;

		mesh_vertices[i].nx = oMeshBlock.vecVertices[i].normal.x;
		mesh_vertices[i].ny = oMeshBlock.vecVertices[i].normal.y;
		mesh_vertices[i].nz = oMeshBlock.vecVertices[i].normal.z;

		mesh_vertices[i].tx = oMeshBlock.vecVertices[i].uv.x;
		mesh_vertices[i].ty = oMeshBlock.vecVertices[i].uv.y;
	}

	vector<uint16_t> lod_indices;
	//vector<vertex> lod_vertices;

	vector<uint16_t> inverse_indices(oMeshBlock.vecIndices.size());
	for (size_t i = 0; i < oMeshBlock.vecIndices.size() / 3; i++)
	{
		inverse_indices[i * 3] = oMeshBlock.vecIndices[i * 3];
		inverse_indices[i * 3 + 1] = oMeshBlock.vecIndices[i * 3 + 2];
		inverse_indices[i * 3 + 2] = oMeshBlock.vecIndices[i * 3 + 1];
	}

	vector<ModelVertex> lod_vertices;

	size_t target_index_count = size_t(oMeshBlock.vecIndices.size() * threshold);
	float target_error = 1;

	//////////////////////////////////////////////////
	lod_indices.resize(oMeshBlock.vecIndices.size()); // note: simplify needs space for index_count elements in the destination array, not target_index_count

	lod_indices.resize(
		meshopt_simplify(
		&lod_indices[0],
		&oMeshBlock.vecIndices[0],
		oMeshBlock.vecIndices.size(),
		&mesh_vertices[0].px,
		mesh_vertices.size(),
		sizeof(Vertex),
		target_index_count,
		target_error
	));

	//oMeshBlock.vecIndices = lod_indices;

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

	//vector<ModelVertex> vecVert(oMeshBlock.vecVertices.size());
	//map<int, ModelVertex> remapped_v;
	//map<uint16_t, ModelVertex> map1;
	//map<int, int> _Map;
	//vector<int> re_map(oMeshBlock.vecVertices.size(), -1);

	//vector<ModelVertex> _v;
	//vector<uint16_t> new_in;
	//int _i1 = 0;
	//for (size_t i = 0; i < lod_indices.size(); i++)
	//{
	//	uint16_t index = lod_indices[i];

	//	// is index already in map?
	//	if (map1.find(index) == map1.end())
	//	{
	//		map1[index] = oMeshBlock.vecVertices[index];
	//		_Map[index] = _v.size();
	//		_v.push_back(oMeshBlock.vecVertices[index]);
	//		_i1++;
	//	}
	//}

	//new_in.resize(lod_indices.size());
	//for (size_t i = 0; i < lod_indices.size(); i++)
	//{
	//	uint16_t index = lod_indices[i];
	//	new_in[i] = _Map[index];
	//}
	//vector <unsigned int> remap(oMeshBlock.vecVertices.size());
	//size_t unique_vertices_simplify =
	//	//meshopt_optimizeVertexFetchRemap<uint16_t>
	////	(
	////	&remap[0],
	////	&lod_indices[0], lod_indices.size(), /*&oMeshBlock.vecVertices[0], */oMeshBlock.vecVertices.size()/*, sizeof(ModelVertex)*/
	////	);

	//	meshopt_generateVertexRemap<uint16_t>
	//	(
	//	&remap[0],
	//	&lod_indices[0], lod_indices.size(), &oMeshBlock.vecVertices[0], oMeshBlock.vecVertices.size(), sizeof(ModelVertex)
	//	);

	//vector<ModelVertex> newVertices(unique_vertices_simplify);
	//meshopt_remapVertexBuffer(&newVertices[0], &oMeshBlock.vecVertices[0], unique_vertices_simplify, sizeof(ModelVertex), &remap[0]);

	//meshopt_remapIndexBuffer(&lod_indices[0], &lod_indices[0], lod_indices.size(), &remap[0]);

	//oMeshBlock.vecIndices = lod_indices;
	////oMeshBlock.vecIndices.resize(lod_indices.size());
	//oMeshBlock.vecVertices = newVertices;

	//return;

	lod_vertices.resize(meshopt_optimizeVertexFetch(
		&lod_vertices[0],
		&lod_indices[0],
		lod_indices.size(),
		&oMeshBlock.vecVertices[0],
		oMeshBlock.vecVertices.size(),
		sizeof(ModelVertex)));

	//oMeshBlock.vecVertices.resize(lod_vertices.size());

	float vertex_reduction = 1.0f - ((float)lod_vertices.size() / (float)oMeshBlock.vecVertices.size());
	float index_reduction = 1.0f - ((float)lod_indices.size() / (float)oMeshBlock.vecIndices.size());

	_log_action("Mesh simplifier, vertices: " + to_string(vertex_reduction * 100.0f) + "% reduction");
	_log_action("Mesh simplifier, indicies: " + to_string(index_reduction * 100.0f) + "% reduction");
	_log_action("From " + to_string(oMeshBlock.vecIndices.size() / 3) + " polygons (triangles) to " +
		to_string(lod_indices.size() / 3) + " polygons (triangles).");

	//m_vecRMV2_Vertices_Cinematic_Raw[_group].resize(lod_vertices.size());
	//oMeshBlock.vecVertices = lod_vertices;

	//m_vecRMV2_Indices[_group].resize(lod_indices.size());

//	oMeshBlock.vecIndices = new_in;

	oMeshBlock.vecIndices = lod_indices;
	oMeshBlock.vecVertices = lod_vertices;

	int debug_1 = 1;
	return;
	/**********************************************************
		Mesh Reduction
	***********************************************************/
	//if (false)

	//float threshold = 0.5;
	//float _target_error = 0.001;

	//vector<uint16_t> lod_indices;
	//vector<ModelVertex> lod_vertices;

	//size_t target_index_count = size_t(oMeshBlock.vecIndices.size() * threshold);
	//float target_error = _target_error;

	//struct Vertex
	//{
	//	float px, py, pz;
	//	float nx, ny, nz;
	//	float tx, ty;
	//};

	//vector<Vertex> mesh_vertices(oMeshBlock.vecVertices.size());

	//for (size_t i = 0; i < mesh_vertices.size(); i++)
	//{
	//	auto& p1 = oMeshBlock.vecVertices[i];

	//	mesh_vertices[i].px = p1.position.x;
	//	mesh_vertices[i].py = p1.position.y;
	//	mesh_vertices[i].pz = p1.position.z;

	//	mesh_vertices[i].nx = p1.normal.x;
	//	mesh_vertices[i].ny = p1.normal.y;
	//	mesh_vertices[i].nz = p1.normal.z;

	//	mesh_vertices[i].tx = p1.uv.x;
	//	mesh_vertices[i].ty = p1.uv.y;
	//}

	//lod_indices.resize(oMeshBlock.vecIndices.size()); // note: simplify needs space for index_count elements in the destination array, not target_index_count

	//lod_indices.resize(
	//	meshopt_simplify<uint16_t>(
	//	&lod_indices[0],
	//	&oMeshBlock.vecIndices[0],
	//	oMeshBlock.vecIndices.size(),
	//	&mesh_vertices[0].px,
	//	mesh_vertices.size(),
	//	sizeof(Vertex),
	//	target_index_count,
	//	target_error
	//));

	//lod_vertices.resize(lod_indices.size() < mesh_vertices.size() ? lod_indices.size() : mesh_vertices.size()); // note: this is just to reduce the cost of resize()

	//lod_vertices.resize(meshopt_optimizeVertexFetch(
	//	&lod_vertices[0],
	//	&lod_indices[0],
	//	lod_indices.size(),
	//	&oMeshBlock.vecVertices[0],
	//	oMeshBlock.vecVertices.size(),
	//	sizeof(ModelVertex)));

	///*lod_vertices.resize(meshopt_optimizeVertexFetch(
	//	&lod_vertices[0],
	//	&lod_indices[0],
	//	lod_indices.size(),
	//	&vecVertices_RAW[0],
	//	vecVertices_RAW.size(),
	//	sizeof(RMV2_Vertex_Common_RAW)));*/

	//	//float vertex_reduction = 1.0f - ((float)lod_vertices.size() / start_total_vertices /*(float)oMeshBlock.vecVertices.size()*/);
	//	//float index_reduction = 1.0f - ((float)lod_indices.size() / start_total_indices /* (float)oMeshBlock.vecIndices.size()*/);

	//oMeshBlock.vecVertices = lod_vertices;
	//oMeshBlock.vecIndices = lod_indices;

	int debug_2 = 1;
}

//if (m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::tiled_dirtmap)
//		seek(16, seek_direction::seek_forward);  // file.seekg(16, ios_base::cur);
//	else if (m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::decal
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_decal
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_skin_decal)
//		seek(20, seek_direction::seek_forward); // file.seekg(20, ios_base::cur);
//	else if (m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::dirtmap
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_dirtmap
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_skin_dirtmap)
//	{
//		//seek(32, seek_direction::seek_forward); // file.seekg(32, ios_base::cur);
//		read(&data, 32);
//		int i3 = 0;
//	}
//	else if (m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::decal_dirtmap
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_decal_dirtmap
//		|| m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::weighted_skin_decal_dirtmap)
//		seek(52, seek_direction::seek_forward); // file.seekg(52, ios_base::cur);
//	else if (m_File.LodData[m_lod][m_subgroup]->Header.uiMaterialId == RigidMaterial::tree)
//		seek(56, seek_direction::seek_forward); // file.seekg(56, ios_base::cur);

void RigidModelV2::Common::LodBLock::simplifyLod(float reduce_rate, float _error, bool _bUseSloppy)
{
	for (size_t group = 0; group < vecGroups.size(); group++)
	{
		vecGroups[group].simplifyGroup(reduce_rate, _error, _bUseSloppy);
	}
}

void RigidModelV2::Common::LodBLock::setDefaultTextures()
{
	for (auto& itGroup : vecGroups)
	{
		itGroup.setTextures();
	}
}

uint32_t RigidModelV2::Common::LodBLock::updateSize()
{
	uint32_t lod_size = 0;
	for (auto& itGroup : vecGroups)
	{
		lod_size += itGroup.updateGroupFields_v6_v7_v8();
	}
	return lod_size;
}

uint32_t RigidModelV2::Common::LodBLock::getVertexDataSize()
{
	uint32_t vertex_data_size = 0;

	for (size_t group = 0; group < vecGroups.size(); group++)
	{
		vertex_data_size +=
			vecGroups[group].getDataSizeVertices();
	}

	return vertex_data_size;
}

uint32_t RigidModelV2::Common::LodBLock::getIndexDataSize()
{
	uint32_t index_data_size = 0;

	for (size_t group = 0; group < vecGroups.size(); group++)
	{
		index_data_size +=
			vecGroups[group].getDataSizeIndices();
	}

	return index_data_size;
}