#include "RigidModelV2_Writer.h"

#include "RigidModelV2_Writer.h"
#include "RigidModelV2_Errors.h"
#include "..\RenderLib\lib3d.h"
#include "..\SystemLib\tools.h"

bool RigidModelV2::File_Exporter_Common::ErrorString(const std::string& _error)
{
	m_strLastErrorString = _error;
	_log_action_error(_error);

	return false;
}

std::shared_ptr<RigidModelV2::File_Exporter_Common> RigidModelV2::File_Exporter_Common::create_empty()
{
	auto spoFileImporter = std::shared_ptr < RigidModelV2::File_Exporter_Common>(new RigidModelV2::File_Exporter_Common);
	spoFileImporter->m_spoData = std::shared_ptr<Common::CommonFile>(new Common::CommonFile);
	spoFileImporter->m_spoStream = std::shared_ptr<SystemLib::DataStream>(new SystemLib::DataStream());

	return spoFileImporter;
}

std::shared_ptr<RigidModelV2::File_Exporter_Common> RigidModelV2::File_Exporter_Common::create_from_mem(void* pSrcMem, size_t _size_in_bytes)
{
	auto spoFileImporter = std::shared_ptr < RigidModelV2::File_Exporter_Common>(new RigidModelV2::File_Exporter_Common);
	spoFileImporter->m_spoStream = std::shared_ptr<SystemLib::DataStream>(new SystemLib::DataStream(pSrcMem, _size_in_bytes));

	spoFileImporter->m_spoData = std::shared_ptr<Common::CommonFile>(new Common::CommonFile);

	return spoFileImporter;
}

std::shared_ptr<RigidModelV2::File_Exporter_Common> RigidModelV2::File_Exporter_Common::create_from_disk(const std::wstring& _strPath)
{
	auto spoFileExporter = std::shared_ptr < RigidModelV2::File_Exporter_Common>(new RigidModelV2::File_Exporter_Common);

	spoFileExporter->m_spoStream = std::shared_ptr<SystemLib::DataStream>(new SystemLib::DataStream((wstring&)_strPath));

	spoFileExporter->m_spoData = std::shared_ptr<Common::CommonFile>(new Common::CommonFile);

	/*if (!spoFileExporter->read())
	{
		spoFileExporter->m_bIsValid = false;
	}*/

	return spoFileExporter;
}

std::shared_ptr<RigidModelV2::Common::CommonFile> RigidModelV2::File_Exporter_Common::getFile()
{
	return m_spoData;
}

bool RigidModelV2::File_Exporter_Common::setFileData(std::shared_ptr<RigidModelV2::Common::CommonFile> _spoFile)
{
	m_spoData = _spoFile;

	m_spoStream->clearAndReset();

	return write();
}

bool RigidModelV2::File_Exporter_Common::write()
{
	_log_action("Writing RigidModelV2...");

	_log_action("Calculating block size fields...");
	m_spoData->updateFileFields();

	if (!writeFileInfoHeader_V5_V6_V7_V8())
	{
		return false;
	}

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
	{
		if (!writeFileHeader_V5())
		{
			return false;
		};
	}
	else
	{
		if (!writeFileHeader_V6_V7_V8())
		{
			return false;
		};
	}

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion > 6)
	{
		if (!writeLodHeaderBlock_V7_V8())
		{
			return false;
		};
	}
	else
	{
		if (!writeLodHeaderBlock_V5_V6())
		{
			return false;
		};
	}

	return writeLods();
}

bool RigidModelV2::File_Exporter_Common::writeFileInfoHeader_V5_V6_V7_V8() {
	m_spoStream->writeMem(&m_spoData->oFileHeader.oFileInfoHeader.Signature.dwSignature, 4);
	m_spoStream->writeMem(&m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion, 4);
	m_spoStream->writeMem(&m_spoData->oFileHeader.dwLodCount, 4);

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeFileHeader_V5()
{
	_log_action(string("Write File: File Has " + std::to_string(m_spoData->oFileHeader.dwLodCount) + " LODs").c_str());
	wstring wstrTempSkeletonId = tools::string_to_wstring(m_spoData->oFileHeader.szSkeletonId);

	memset(m_spoData->oFileHeader.wszSkeletonId, 128 * 2, 0);
	memcpy(m_spoData->oFileHeader.wszSkeletonId, &wstrTempSkeletonId[0], 128 * 2);

	// write to file
	m_spoStream->writeMem(&m_spoData->oFileHeader.wszSkeletonId, 128 * 2);

	_log_action("Write File: Skeleton: " + string(m_spoData->oFileHeader.szSkeletonId));

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeFileHeader_V6_V7_V8()
{
	//if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion <= 5) // RMV2 version 5 uses 16 bit chars (UTF-16 or unicode)
	//{
	//	m_spoStream->writeMem(&m_spoData->oFileHeader.wszSkeletonId, 128 * 2);
	//}
	//else // newer versions use 8 bit chars
	{
		m_spoStream->writeMem(&m_spoData->oFileHeader.szSkeletonId, 128);
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeLods()
{
	m_spoData->vecLODs.resize(m_spoData->oFileHeader.wLod);

	for (size_t lodCounter = 0; lodCounter < m_spoData->oFileHeader.wLod; lodCounter++)
	{
		uint32_t file_position = m_spoStream->tellp();

		_log_action("Lod " + to_string(lodCounter) + string(" calculated offset: ") +
			to_string(m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwStartOffset)
		);

		_log_action(string("Actual offset: ") +
			to_string(file_position)
		);

		if (!RigidModelV2::Error::lodOffset_Check(file_position, m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwStartOffset))
		{
			return RigidModelV2::Common::CommonFile::ErrorString(m_spoData,
				"FATAL ERROR: LOD: " + to_string(lodCounter) + " failed integrity test. + start offset mismatch"

			);
		}

		// resize group "array" to contain the number of number in this LOD
		m_spoData->vecLODs[lodCounter].vecMeshBlocks.resize(m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwMeshCount);

		auto first = m_spoStream->tellp();
		for (size_t groupCounter = 0; groupCounter < m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwMeshCount; groupCounter++)
		{
			if (!writeMeshBlock(lodCounter, groupCounter))
			{
				return false;
			}
		}
		auto read = m_spoStream->tellp() - first;

		int debug_1 = 1;
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeMeshBlock(size_t _lod, size_t _group)
{
	if (!writeMeshPreHeader(_lod, _group))
	{
		return false;
	}

	if (!writeMeshHeader(_lod, _group))
	{
		return false;
	}

	size_t first_index = m_spoStream->tellp();

	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion > 5)
	{
		if (!writeAttachmentPointBlock_V6_V7_V8(_lod, _group))
		{
			return false;
		}

		if (!writeTextureBlock_V6_V7_V8(_lod, _group))
		{
			return false;
		}
	}
	else
	{
		if (!writeAttachmentPointBlock_V5(_lod, _group))
		{
			return false;
		}

		if (!writeTextureBlock_V5(_lod, _group))
		{
			return false;
		}
	};

	auto& extra = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oExtraMaterialBlock;
	auto& mesh = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	auto extra_size = extra.vecExtraMaterialData.size();

	if (extra_size)
	{
		extra.vecExtraMaterialData.resize(extra_size);
		m_spoStream->writeMem(&extra.vecExtraMaterialData[0], extra_size);
	}

	if (!writeVertexBlock_Raw(_lod, _group))
	{
		return false;
	}

	if (!writeIndices(_lod, _group))
	{
		return false;
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeMeshHeader(size_t _lod, size_t _group) {
	return RigidModelV2::File_Exporter_Common::writeGroupHeaderDefaultWeighted_V6_V7_V8(_lod, _group);
}

//bool RigidModelV2::File_Exporter_Common::writeGroupHeader(ERigidMaterial _eRigidMaterial, size_t _lod, size_t _group)
//{
//	return true;
//}

bool RigidModelV2::File_Exporter_Common::writeLodHeaderBlock_V5_V6()
{
	auto first = m_spoStream->tellp();
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->writeMem(&itLod.dwMeshCount, 4);
		m_spoStream->writeMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->writeMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->writeMem(&itLod.dwStartOffset, 4);
		m_spoStream->writeMem(&itLod.fVisibilityDistance, 4);
	}

	auto read = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeLodHeaderBlock_V7_V8()
{
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->writeMem(&itLod.dwMeshCount, 4);
		m_spoStream->writeMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->writeMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->writeMem(&itLod.dwStartOffset, 4);
		m_spoStream->writeMem(&itLod.fVisibilityDistance, 4);
		m_spoStream->writeMem(&itLod.dwAuthoredLodNumber, 4);
		m_spoStream->writeMem(&itLod.oQualityLevel, 4);
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeMeshPreHeader(size_t _lod, size_t _group)
{
	m_spoStream->writeMem(&m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader, sizeof(RigidModelV2::Common::MeshPreHeader));

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeGroupHeaderDefaultWeighted_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& SubMeshPreHeader = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;
	auto& group = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	auto first = m_spoStream->tellp();

	SubMeshPreHeader.strShaderName.resize(12);
	m_spoStream->writeMem(&SubMeshPreHeader.strShaderName[0], 12);
	m_spoStream->writeMem(&SubMeshPreHeader.oFlags, 5 * 4);
	m_spoStream->writeMem(&SubMeshPreHeader.VertexFormatId, sizeof(SubMeshPreHeader.VertexFormatId));
	m_spoStream->writeMem(&SubMeshPreHeader.strMeshName[0], 32);
	m_spoStream->writeMem(&SubMeshPreHeader.strTextureDirectory[0], 256);
	m_spoStream->writeMem(&SubMeshPreHeader.uk2_512, 256);
	m_spoStream->writeMem(&SubMeshPreHeader.wUK_2, 2);
	m_spoStream->writeMem(&SubMeshPreHeader.vPivot, 4 * 3);
	m_spoStream->writeMem(&SubMeshPreHeader.oTransformation, (4 * 3 * 4) * 3);

	m_spoStream->writeMem(&SubMeshPreHeader.bone_id_building, 4);
	m_spoStream->writeMem(&SubMeshPreHeader.bone_id_unknown, 4);

	m_spoStream->writeMem(&SubMeshPreHeader.dwAttachmentPointCount, 4);
	m_spoStream->writeMem(&SubMeshPreHeader.dwTextureCount, 4);

	m_spoStream->writeMem(&SubMeshPreHeader.oValues, sizeof(SubMeshPreHeader.oValues));

	// correct version
	m_spoStream->writeMem(&SubMeshPreHeader.vecUnknownDataBuffer[0], 140 - sizeof(SubMeshPreHeader.oValues));

	// TODO: debuggin version, on purpose crashy
	//m_spoStream->writeMem(&SubMeshPreHeader.vecUnknownDataBuffer[0], 140);

	auto read = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeGroupHeaderDefaultWeighted_V5(size_t _lod, size_t _group)
{
	return false;
}

bool RigidModelV2::File_Exporter_Common::writeAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oAttachBlock.vecAttachmentPoints.resize(oGroup.oSubMeshHeader.dwAttachmentPointCount);

	for (auto& it : oGroup.oAttachBlock.vecAttachmentPoints)
	{
		it.szAttachmentPointName.resize(32); // make string fit the field size
		m_spoStream->writeMem(&it.szAttachmentPointName[0], 32);
		m_spoStream->writeMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->writeMem(&it.dwBoneId, 4);
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeAttachmentPointBlock_V5(size_t _lod, size_t _group)
{
	auto& ref_vecAttachmentPoints = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oAttachBlock.vecAttachmentPoints;

	for (auto& it : ref_vecAttachmentPoints)
	{
		it.wszAttachmentPointName = tools::string_to_wstring(it.szAttachmentPointName);

		m_spoStream->writeMem(&it.wszAttachmentPointName, 256 * 2);
		m_spoStream->writeMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->writeMem(&it.dwBoneId, 4);
	}

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeTextureBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		it.szTextureDirAndFileName.resize(258);
		m_spoStream->writeMem(&it.Type, 4);
		m_spoStream->writeMem(&it.szTextureDirAndFileName[0], 256);
	}

	m_spoStream->writeMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->writeMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeTextureBlock_V5(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		// convert the texture path from to wstring for v5's 16 bit chars
		it.wszTextureDirAndFileName = tools::string_to_wstring(it.szTextureDirAndFileName);

		it.wszTextureDirAndFileName.resize(512);
		m_spoStream->writeMem(&it.Type, 4);
		m_spoStream->writeMem((void*)it.wszTextureDirAndFileName.data(), 256 * 2);
	}

	m_spoStream->writeMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->writeMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Exporter_Common::writeVertexBlock_Raw(size_t _lod, size_t _group)
{
	auto& oGroupBlock = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	size_t vertex_data_size = oGroupBlock.oPreHeader.dwVertexData_TextAndAttach_BlockSize -
		oGroupBlock.oPreHeader.uiTextureAndAttchmentBlockSize;

	size_t vertex_size = oGroupBlock.getVertexSize();

	// this is a raw memory buffer holding bytes
	m_spoStream->writeMem(oGroupBlock.oMeshBlock.vecVertexDataRawBuffer.data(),
		oGroupBlock.oMeshBlock.vecVertexDataRawBuffer.size());

	// this is a buffer holding actual vertices
	//oGroupBlock.oMeshBlock.vecVertices.resize(oGroupBlock.oPreHeader.dwVertexCount);

	/*    pack vertices from float32 based to flaot16 and 8 bit unorm based   */

	return true;

	switch (oGroupBlock.oSubMeshHeader.VertexFormatId)
	{
	case EVertexFormat::eDefaultFormat:
		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.makePacked_Default_V5_V6_V7(vertex_size);
		}
		else
		{
		}
		break;

	case EVertexFormat::eWeightedFormat:
		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.makePacked_Weighted2_V5_V6_V7(vertex_size);
		}
		else
		{
		}
		break;

	case EVertexFormat::eCinematicFormat:

		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion < 8)
		{
			oGroupBlock.oMeshBlock.makePacked_Weighted4_V5_V6_V7(vertex_size);
		}
		else
		{
		}
		break;
	}

	size_t first = m_spoStream->tellp();
	m_spoStream->writeMem(
		&oGroupBlock.oMeshBlock.vecVertexDataRawBuffer[0],
		vertex_data_size
	);

	size_t write = m_spoStream->tellp() - first;
	return true;
}

bool RigidModelV2::File_Exporter_Common::writeIndices(size_t _lod, size_t _group)
{
	auto& ref_vecIndices = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oMeshBlock.vecIndices;

	ref_vecIndices.resize(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwIndexCount);

	m_spoStream->writeMem(ref_vecIndices.data(), m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwIndexCount * 2);

	return true;
}

size_t RigidModelV2::File_Exporter_Common::write_file(const std::wstring& _strPath)
{
	m_spoData->updateFileFields();

	return m_spoStream->write_file(_strPath);
}

//std::shared_ptr<RigidModelV2::File_Exporter_Common> RigidModelV2::File_Exporter_Common::create(SystemLib::DataStream& _Stream)
//{
//	return std::shared_ptr<File_Exporter_Common>();
//}

//bool RigidModelV2::File_Exporter_Common::processVertices_As_Cinematic_V5_V6_V7(size_t _lod, size_t _group)
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