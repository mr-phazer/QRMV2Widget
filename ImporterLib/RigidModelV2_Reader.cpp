#include "RigidModelV2_Reader.h"
#include "..\RenderLib\lib3d.h"
#include "..\SystemLib\tools.h"
#include "..\QtTestRMV2\QConsole.h"

#include <d3d11.h>

std::shared_ptr <RigidModelV2::Common::CommonFile>  RigidModelV2::File_Importer_Common::getFile()
{
	return m_spoData;
}

bool RigidModelV2::File_Importer_Common::read()
{
	if (!readFileHeader_V5_V6_V7_V8())
	{
		return false;
	};

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion > 6)
	{
		if (!readLodHeaderBlock_V7_V8())
		{
			return false;
		};
	}
	else
	{
		if (!readLodHeaderBlock_V5_V6())
		{
			return false;
		};
	}

	return readLods();
}

bool RigidModelV2::File_Importer_Common::readFileHeader_V5_V6_V7_V8()
{
	m_spoStream->readMem(&m_spoData->oFileHeader.oFileInfoHeader.Signature.dwSignature, 4);
	m_spoStream->readMem(&m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion, 4);

	m_spoStream->readMem(&m_spoData->oFileHeader.dwLodCount, 4);

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion <= 5) // RMV2 version 5 uses 16 bit chars (UTF-16 or unicode)
	{
		m_spoStream->readMem(&m_spoData->oFileHeader.wszSkeletonId, 128 * 2);
	}
	else // newer versions use 8 bit chars
	{
		m_spoStream->readMem(&m_spoData->oFileHeader.szSkeletonId, 128);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readLods()
{
	m_spoData->vecLODs.resize(m_spoData->oFileHeader.wLod);

	if (m_spoData->oFileHeader.wLod > 1)
	{
		int i = 1;
	}

	for (size_t lodCounter = 0; lodCounter < m_spoData->oFileHeader.wLod; lodCounter++)
	{
		// resize group "array" to contain the number of number in this LOD

		m_spoData->vecLODs[lodCounter].vecGroups.resize(m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwGroupCount);

		for (size_t groupCounter = 0; groupCounter < m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwGroupCount; groupCounter++)
		{
			if (!readGroupBlock(lodCounter, groupCounter))
			{
				return false;
			}
		}
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readGroupBlock(size_t _lod, size_t _group)
{
	if (!readGroupPreHeader(_lod, _group))
	{
		return false;
	}

	if (!readGroupHeader(_lod, _group))
	{
		return false;
	}
	size_t first_index = m_spoStream->tellp();

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion > 5)
	{
		if (!readAttachmentPointBlock_V6_V7_V8(_lod, _group))
		{
			return false;
		}

		if (!readTextureBlock_V6_V7_V8(_lod, _group))
		{
			return false;
		}
	}
	else
	{
		if (!readAttachmentPointBlock_V5(_lod, _group))
		{
			return false;
		}

		if (!readTextureBlock_V5(_lod, _group))
		{
			return false;
		}
	}

	size_t read = m_spoStream->tellp() - first_index;

	size_t extra = (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.uiTextureAndAttchmentBlockSize - 940) - read;

	// store the extra data some material type put here.

	m_spoData->vecLODs[_lod].vecGroups[_group].oExtraMaterialBlock.vecExtraMaterialData.clear();

	if (extra)
	{
		_log_action("Reading " + to_string(extra) + " addtional bytes of material data");
		m_spoData->vecLODs[_lod].vecGroups[_group].oExtraMaterialBlock.vecExtraMaterialData.resize(extra);

		m_spoStream->readMem(m_spoData->vecLODs[_lod].vecGroups[_group].oExtraMaterialBlock.vecExtraMaterialData.data(), extra);
	}

	if (!readVertexBlock_Raw(_lod, _group))
	{
		return false;
	}

	if (!readIndices(_lod, _group))
	{
		return false;
	}

	//m_spoData->vecLODs[_lod].vecGroups[_group].cleanUpMesh();

	if ((*m_spoData)[_lod][_group].oPreHeader.RigidMaterialId == ERigidMaterial::decal_dirtmap)
		int i = 1;

	return true;
}

bool RigidModelV2::File_Importer_Common::readGroupHeader(size_t _lod, size_t _group) {
	return RigidModelV2::File_Importer_Common::readGroupHeaderDefaultWeighted(_lod, _group);
}

bool RigidModelV2::File_Importer_Common::readGroupHeader(ERigidMaterial _eRigidMaterial, size_t _lod, size_t _group)
{
	return true;
}

bool RigidModelV2::File_Importer_Common::readLodHeaderBlock_V5_V6()
{
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->readMem(&itLod.dwGroupCount, 4);
		m_spoStream->readMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->readMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->readMem(&itLod.dwStartOffset, 4);
		m_spoStream->readMem(&itLod.fVisibilityDistance, 4);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readLodHeaderBlock_V7_V8()
{
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->readMem(&itLod.dwGroupCount, 4);
		m_spoStream->readMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->readMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->readMem(&itLod.dwStartOffset, 4);
		m_spoStream->readMem(&itLod.fVisibilityDistance, 4);
		m_spoStream->readMem(&itLod.dwAuthoredLodNumber, 4);
		m_spoStream->readMem(&itLod.oQualityLevel, 4);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readGroupPreHeader(size_t _lod, size_t _group)
{
	m_spoStream->readMem(&m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader, sizeof(RigidModelV2::Common::GroupPreHeader));

	// calculate the vertex size based on the groups block size fields.
	m_spoData->vecLODs[_lod].vecGroups[_group].calculateVertexSize();

	return true;
}

bool RigidModelV2::File_Importer_Common::readGroupHeaderDefaultWeighted(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecGroups[_group].oSubMeshHeader;

	auto first = m_spoStream->tellp();

	m_spoStream->readMem(&o.szShaderName, 12);
	m_spoStream->readMem(&o.oFlags, sizeof(o.oFlags));
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));
	m_spoStream->readMem(&o.szGroupName, 32);
	m_spoStream->readMem(&o.szTextureDirectory, 256);
	m_spoStream->readMem(&o.uk_256, 256);
	m_spoStream->readMem(&o.wUK_2, 2);
	m_spoStream->readMem(&o.vPivot, 4 * 3);
	m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);

	m_spoStream->readMem(&o.oValues, sizeof(o.oValues));

	m_spoStream->readMem(&o.Unknown140, 140 - 32);

	auto bytes_read = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Importer_Common::readAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecGroups[_group];

	oGroup.oAttachBlock.vecAttachmentPoints.resize(oGroup.oSubMeshHeader.dwAttachmentPointCount);

	for (auto& it : oGroup.oAttachBlock.vecAttachmentPoints)
	{
		m_spoStream->readMem(&it.szAttachmentPointName, sizeof(it.szAttachmentPointName));
		m_spoStream->readMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->readMem(&it.dwBoneId, 4);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readAttachmentPointBlock_V5(size_t _lod, size_t _group)
{
	auto& ref_vecAttachmentPoints = m_spoData->vecLODs[_lod].vecGroups[_group].oAttachBlock.vecAttachmentPoints;

	for (auto& it : ref_vecAttachmentPoints)
	{
		m_spoStream->readMem(&it.wszAttachmentPointName, 256 * 2);
		m_spoStream->readMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->readMem(&it.dwBoneId, 4);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readTextureBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecGroups[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		it.szTextureDirAndFileName.resize(258);
		m_spoStream->readMem(&it.Type, 4);
		m_spoStream->readMem((void*)it.szTextureDirAndFileName.data(), 256);
	}

	m_spoStream->readMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->readMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Importer_Common::readTextureBlock_V5(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecGroups[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		it.wszTextureDirAndFileName.resize(258);
		m_spoStream->readMem(&it.Type, 4);
		m_spoStream->readMem((void*)it.wszTextureDirAndFileName.data(), 256 * 2);
	}

	m_spoStream->readMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->readMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Importer_Common::readVertexBlock_Raw(size_t _lod, size_t _group)
{
	auto& oGroupBlock = m_spoData->vecLODs[_lod].vecGroups[_group];

	size_t vertex_data_size = oGroupBlock.getDataSizeVertices();

	/*oGroupBlock.oPreHeader.dwVertexData_TextAndAttach_BlockSize -
	oGroupBlock.oPreHeader.uiTextureAndAttchmentBlockSize;*/

	size_t vertex_size = oGroupBlock.calculateVertexSize();
	// this is a raw memory buffer holding bytes
	oGroupBlock.oMeshBlock.vecVertexDataRawBuffer.resize(vertex_data_size);

	// this is a buffer holding actual vertices
	oGroupBlock.oMeshBlock.vecVertices.resize(oGroupBlock.oPreHeader.dwVertexCount);

	size_t first = m_spoStream->tellp();
	m_spoStream->readMem(
		&oGroupBlock.oMeshBlock.vecVertexDataRawBuffer[0],
		vertex_data_size
	);

	size_t read = m_spoStream->tellp() - first;

	/*    Convert to common vertex type    */

	if (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.RigidMaterialId == ERigidMaterial::weighted_dirtmap)
		int i = 1;

	if (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.RigidMaterialId == ERigidMaterial::weighted)
		int i = 1;

	if (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.RigidMaterialId == ERigidMaterial::weighted_skin)
		int i = 1;

	switch (oGroupBlock.oSubMeshHeader.VertexFormatId)
	{
	case EVertexFormat::eDefaultFormat:
		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.unpackAs_Default(vertex_size);
		}
		else
		{
		}
		break;

	case EVertexFormat::eWeightedFormat:
		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.unpackAs_Weighted2_V5_V6_V7(vertex_size);
		}
		else
		{
		}
		break;

	case EVertexFormat::eCinematicFormat:

		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.unpackAs_Weighted4_V5_V6_V7(vertex_size);
		}
		else
		{
		}
		break;
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readIndices(size_t _lod, size_t _group)
{
	auto& ref_vecIndices = m_spoData->vecLODs[_lod].vecGroups[_group].oMeshBlock.vecIndices;

	ref_vecIndices.resize(m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.dwIndexCount);

	m_spoStream->readMem(ref_vecIndices.data(), m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.dwIndexCount * 2);

	return true;
}

std::shared_ptr<RigidModelV2::File_Importer_Common> RigidModelV2::File_Importer_Common::create(void* pSrcMem, size_t _size_in_bytes)
{
	auto spoFileImporter = std::shared_ptr < RigidModelV2::File_Importer_Common>(new RigidModelV2::File_Importer_Common);
	spoFileImporter->m_spoStream = std::shared_ptr<SystemLib::DataStream>(new SystemLib::DataStream(pSrcMem, _size_in_bytes));

	spoFileImporter->m_spoData = std::shared_ptr<Common::CommonFile>(new Common::CommonFile);

	if (!spoFileImporter->read())
	{
		return nullptr;
	}

	return spoFileImporter;
}

std::shared_ptr<RigidModelV2::File_Importer_Common> RigidModelV2::File_Importer_Common::create(const std::wstring& _strPath)
{
	if (tools::lower(_strPath) == tools::lower(L"F:/Temp_RoM/variantmeshes/_variantmodels/erebor/armour/erebor_armour.rigid_model_v2"))
		int i = 1;

	size_t file_size = tools::getFileSize(_strPath);

	vector<uint8_t> vecBuffer(file_size);

	ifstream inFile(_strPath, ios::binary);
	if (!inFile.is_open())
	{
		return nullptr;
	}

	inFile.read((char*)&vecBuffer[0], file_size);
	inFile.close();

	return create(&vecBuffer[0], file_size);
}

//std::shared_ptr<RigidModelV2::File_Importer_Common> RigidModelV2::File_Importer_Common::create(SystemLib::DataStream& _Stream)
//{
//	return std::shared_ptr<File_Importer_Common>();
//}

//bool RigidModelV2::File_Importer_Common::processVertices_As_Cinematic_V5_V6_V7(size_t _lod, size_t _group)
//{
//	auto& oMeshBlock = m_spoData->vecLODs[_lod].vecGroups[_group].oMeshBlock;
//
//	for (size_t i = 0; i < oMeshBlock.vecVertices.size(); i++)
//	{
//		// cast the raw buffer with the vertex data to a cinematic raw vertex
//		Vertex_Raw_Cinematic* pv1_RAW = reinterpret_cast<Vertex_Raw_Cinematic*>(&oMeshBlock.vecVertexDataRaw[i * 32]);
//
//		// "shortcut" to dest vertex
//		auto& pv2 = oMeshBlock.vecVertices[i];
//
//		pv2.position.x = pv1_RAW->position.x;
//		pv2.position.y = pv1_RAW->position.y;
//		pv2.position.z = pv1_RAW->position.z;
//		//		pv2.position.w = pv1_RAW->position.w;
//
//		pv2.uv.x = pv1_RAW->uv.x;
//		pv2.uv.y = pv1_RAW->uv.y;
//
//		pv2.normal.x = lib3d::unorm8_to_float(pv1_RAW->normal.x);
//		pv2.normal.y = lib3d::unorm8_to_float(pv1_RAW->normal.y);
//		pv2.normal.z = lib3d::unorm8_to_float(pv1_RAW->normal.z);
//		//2.normal.w = lib3d::unorm8_to_float(pv1_RAW->normal.w);
//
//		pv2.tangent.x = lib3d::unorm8_to_float(pv1_RAW->tangent.x);
//		pv2.tangent.y = lib3d::unorm8_to_float(pv1_RAW->tangent.y);
//		pv2.tangent.z = lib3d::unorm8_to_float(pv1_RAW->tangent.z);
//		//2.tangent.w = lib3d::unorm8_to_float(pv1_RAW->tangent.w);
//
//		pv2.bitangent.x = lib3d::unorm8_to_float(pv1_RAW->bitangent.x);
//		pv2.bitangent.y = lib3d::unorm8_to_float(pv1_RAW->bitangent.y);
//		pv2.bitangent.z = lib3d::unorm8_to_float(pv1_RAW->bitangent.z);
//		//pv2.bitangent.w = lib3d::unorm8_to_float(pv1_RAW->bitangent.w);
//
//		if ((pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x))
//			int debug_1 = 1;
//
//		pv2.bone_indices.x = pv1_RAW->bone_indices.x;
//		pv2.bone_indices.y = (pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.y;
//		pv2.bone_indices.z = (pv1_RAW->bone_indices.z == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.z;
//		pv2.bone_indices.w = (pv1_RAW->bone_indices.w == pv1_RAW->bone_indices.x) ? 0 : pv1_RAW->bone_indices.w;
//
//		pv2.bone_weights.x = static_cast<float>(pv1_RAW->bone_weights.x) / 255.0;
//		pv2.bone_weights.y = (pv1_RAW->bone_indices.y == pv1_RAW->bone_indices.x) ? 0.f : static_cast<float>(pv1_RAW->bone_weights.y) / 255.0;
//		pv2.bone_weights.z = (pv1_RAW->bone_indices.z == pv1_RAW->bone_indices.x) ? 0.f : static_cast<float>(pv1_RAW->bone_weights.z) / 255.0;
//		pv2.bone_weights.w = (pv1_RAW->bone_indices.w == pv1_RAW->bone_indices.x) ? 0.f : static_cast<float>(pv1_RAW->bone_weights.w) / 255.0;
//	}
//
//	return true;
//}