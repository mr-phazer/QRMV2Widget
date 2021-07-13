#include "RigidModelV2_Reader.h"
#include "..\RenderLib\lib3d.h"
#include "..\SystemLib\tools.h"
#include "..\QtTestRMV2\QConsole.h"
#include "RigidModelV2_Data_Structs.h"

#include <d3d11.h>

std::shared_ptr <RigidModelV2::Common::CommonFile>  RigidModelV2::File_Importer_Common::getFile()
{
	return m_spoData;
}

bool RigidModelV2::File_Importer_Common::read()
{
	if (!checkEntireFile())
	{
		_log_action("File Contains Unknown RigidMaterial Headers");
		return false;
	}

	_log_action("reading RMV2 file header");

	if (!readFileHeaderAndLodBlocks())
	{
		return false;
	}

	if (!m_spoData->oValidator.validateFileHeader(&m_spoData->oFileHeader))
	{
		return false;
	}

	return readLods();
}

bool RigidModelV2::File_Importer_Common::checkEntireFile()
{
	_log_action("checkEntireFile()");

	//if (!readFileInfoHeader_V5_V6_V7_V8())
	//{
	//	return false;
	//};

	//// RMV2 v5 uses 16 bit chars
	//if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
	//{
	//	if (!readFileHeader_V5())
	//	{
	//		return false;
	//	};
	//}
	//else
	//{
	//	if (!readFileHeader_V6_V7_V8())
	//	{
	//		return false;
	//	};
	//}

	//if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion > 6)
	//{
	//	if (!readLodHeaderBlock_V7_V8())
	//	{
	//		return false;
	//	};
	//}
	//else
	//{
	//	if (!readLodHeaderBlock_V5_V6())
	//	{
	//		return false;
	//	};
	//}

	if (!readFileHeaderAndLodBlocks())
	{
		_log_action("readFileHeaderAndLodBlocks() issue!");
		return false;
	}

	for (size_t lod = 0; lod < m_spoData->oLodHeaderBlock.vecElements.size(); lod++)
	{
		// seek to the start of LOD x data

		auto real_offset = m_spoStream->tellp();

		if (real_offset != m_spoData->oLodHeaderBlock.vecElements[lod].dwStartOffset)
		{
			_log_action("FATAL ERROR: LOD: " + to_string(lod) + " failed integrity test. Start offset incorrect.");

			return RigidModelV2::Common::CommonFile::ErrorString(m_spoData,
				"FATAL ERROR: LOD: " + to_string(lod) + " failed integrity test. Start offset incorrect.");
		}

		// skip drectly to a certain LOD, if needed
		//m_spoStream->seek_absolute((size_t)m_spoData->oLodHeaderBlock.vecElements[lod].dwStartOffset);

		// iterate over the meshes in the LOD x
		for (size_t mesh = 0; mesh < m_spoData->oLodHeaderBlock.vecElements[lod].dwMeshCount; mesh++)
		{
			RigidModelV2::Common::MeshPreHeader oMeshPreHeader;
			m_spoStream->readMem(&oMeshPreHeader, sizeof(RigidModelV2::Common::MeshPreHeader)); // read the material ID from the "preheader"

			// check if header describes a known material
			// (where the header lenght+contents is known and there is a working reader/writer for it)
			if (!RigidModelV2::Common::isKnownMaterial(oMeshPreHeader.RigidMaterialId))
			{
				_log_action("RMV2 File Contains Yet - to - be - decoded RigidgMaterial Header : "
					+ getStringFrom_RigidMaterialId(oMeshPreHeader.RigidMaterialId));

				m_spoData->m_strLastErrorString = "RMV2 File Contains Yet-to-be-decoded RigidgMaterial Header: "
					+ getStringFrom_RigidMaterialId(oMeshPreHeader.RigidMaterialId);

				return false;
			}

			// skip to next mesh block;
			m_spoStream->seek_relative(oMeshPreHeader.dwMeshBlockSize - sizeof(RigidModelV2::Common::MeshPreHeader));
		}
	}

	// if this point is reached, file does not contain any unknown mesh headers.

	// reset file
	m_spoStream->seek_absolute(0);

	_log_action("checkEntireFile(): Success.");
	return true;
}

bool RigidModelV2::File_Importer_Common::readFileHeaderAndLodBlocks()
{
	// common to all formats. Contains version, lod count etc
	if (!readFileInfoHeader_V5_V6_V7_V8())
	{
		return false;
	};

	// RMV2 v5 uses 16 bit chars
	if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
	{
		if (!readFileHeader_V5())
		{
			return false;
		};
	}
	else
	{
		if (!readFileHeader_V6_V7_V8())
		{
			return false;
		};
	}

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

	return true;
}

bool RigidModelV2::File_Importer_Common::readFileHeader_V5()
{
	{
		_log_action(string("File Has " + std::to_string(m_spoData->oFileHeader.dwLodCount) + " LODs").c_str());

		m_spoStream->readMem(&m_spoData->oFileHeader.wszSkeletonId, 128 * 2);
		std::wstring wstrTemp = m_spoData->oFileHeader.wszSkeletonId;
		std::string strTemp = tools::wstring_to_string(wstrTemp);

		strTemp.resize(128);

		memcpy(m_spoData->oFileHeader.szSkeletonId, &strTemp[0], 128);

		_log_action("Skeleton: " + string(m_spoData->oFileHeader.szSkeletonId));
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readFileHeader_V6_V7_V8()
{
	_log_action(string("File Has " + std::to_string(m_spoData->oFileHeader.dwLodCount) + " LODs").c_str());

	m_spoStream->readMem(&m_spoData->oFileHeader.szSkeletonId, 128);

	return true;
}

bool RigidModelV2::File_Importer_Common::readFileInfoHeader_V5_V6_V7_V8()
{
	m_spoStream->readMem(&m_spoData->oFileHeader.oFileInfoHeader.Signature.dwSignature, 4);

	// return false and set error string if 4 byte signature does not match
	if (m_spoData->oFileHeader.oFileInfoHeader.Signature.dwSignature != RMV2_SIGNATURE)
	{
		return ErrorString("This is not a valid RigidModelv2 file!");
	}

	m_spoStream->readMem(&m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion, 4);

	m_spoStream->readMem(&m_spoData->oFileHeader.dwLodCount, 4);

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

		m_spoData->vecLODs[lodCounter].vecMeshBlocks.resize(m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwMeshCount);

		for (size_t groupCounter = 0; groupCounter < m_spoData->oLodHeaderBlock.vecElements[lodCounter].dwMeshCount; groupCounter++)
		{
			if (!readMeshBlock(lodCounter, groupCounter))
			{
				return false;
			}
		}
	}

	m_bIsValid = true;
	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshBlock(size_t _lod, size_t _group)
{
	_log_action("LOD " + std::to_string(_lod) + "group " + to_string(_group));

	auto& mesh_block = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	if (!readMeshHeader(_lod, _group))
	{
		return false;
	}

	m_spoData->m_strLastErrorString = m_spoData->oValidator.validateMeshPreHeader(&mesh_block.oPreHeader);
	if (m_spoData->m_strLastErrorString != "")
	{
		return false;
	}

	size_t first_index = m_spoStream->tellp();

	_log_action("");
	_log_action("Ridig_Material: " + getStringFrom_RigidMaterialId(this->getFile()->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.RigidMaterialId)
		+ string("(") + to_string(this->getFile()->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.RigidMaterialId) + ")");

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

	auto& extra = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oExtraMaterialBlock;
	auto& mesh = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	auto extra_size = extra.sizeFromMaterialId(mesh.oPreHeader.RigidMaterialId);

	if (extra_size)
	{
		extra.vecExtraMaterialData.resize(extra_size);
		m_spoStream->readMem(&extra.vecExtraMaterialData[0], extra_size);
	}

	//	return true;

	//auto& g = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];
	//_log_action("Attachmen Points: " + to_string(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount));
	//_log_action("Textures: " + to_string(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount));
	//size_t read = m_spoStream->tellp() - first_index;
	////size_t header_size = 0;
	////if (this->m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
	////{
	////	header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize -
	////		Common::TextureElement::_size_v5 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount -
	////		Common::AttachmentPointTableEntry::_size_v5 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount
	////		- 8;
	////}
	////else
	////{
	////	header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize -
	////		Common::TextureElement::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount -
	////		Common::AttachmentPointTableEntry::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount
	////		- 8;
	////}

	//// read the extra data some material type put here. (calculate its length)

	//size_t header_size =
	//	(m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
	//	?
	//	RigidModelV2::_mesh_header_default_weighted_v5_length
	//	:
	//	RigidModelV2::_mesh_header_default_weighted_v6_v7_v8_length;

	//size_t extra_material_data_size = (m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize - header_size) - read;

	//m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oExtraMaterialBlock.vecExtraMaterialData.clear();

	//if (extra_material_data_size)
	//{
	//	//_log_action(to_string(extra) + " extra bytes for material " + getStringFrom_RigidMaterialId(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.RigidMaterialId) +
	//	//	string("(") + to_string(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.RigidMaterialId) + ")");

	//	m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oExtraMaterialBlock.vecExtraMaterialData.resize(extra_material_data_size);

	//	m_spoStream->readMem(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oExtraMaterialBlock.vecExtraMaterialData.data(), extra_material_data_size);
	//}

	switch (mesh.oPreHeader.RigidMaterialId)
	{
	case ERigidMaterial::alpha_blend: // "alpha_blend" material are a cuber
		break;

	case ERigidMaterial::unknown1: // "alpha_blend" material are a cuber

	default:
		if (!readVertexBlock_STANDARD_Raw(_lod, _group))
		{
			return false;
		}
		break;
	}

	if (!readIndices(_lod, _group))
	{
		return false;
	}

	//m_spoData->vecLODs[_lod].vecGroups[_group].cleanUpMesh();
	/*size_t index_now = m_spoStream->tellp();
	size_t read2 = m_spoStream->tellp() - first_index;

	size_t skip = m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.dwGroupSize - read2;

	m_spoStream->seek_relative(skip - 940);*/

	if ((*m_spoData)[_lod][_group].oPreHeader.RigidMaterialId == ERigidMaterial::decal_dirtmap)
		int i = 1;

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader(size_t _lod, size_t _group) {
	readMeshPreHeader(_lod, _group);

	_log_action("Pre Group Header: ");
	_log_action("LOD: " + to_string(_lod) + ". Group: " + to_string(_group));

	auto& mesh_block = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	auto& dwMeshBlockSize = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwMeshBlockSize;
	auto& dwTextAndAttachSize = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize;
	auto& dwText_Attach_Vertex_Size_ = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwVertexData_TextAndAttach_BlockSize;

	switch (mesh_block.oPreHeader.RigidMaterialId)
	{
	case ERigidMaterial::tree_billboard_material:
		return RigidModelV2::File_Importer_Common::readMeshHeader_AlphaBlend(_lod, _group);

	case ERigidMaterial::alpha_blend:
		return RigidModelV2::File_Importer_Common::readMeshHeader_AlphaBlend(_lod, _group);

	case ERigidMaterial::weighted_texture_blend:
		return RigidModelV2::File_Importer_Common::readMeshHeader_Weighted_Texture_Blend(_lod, _group);

	case ERigidMaterial::unknown1:
		return RigidModelV2::File_Importer_Common::readMeshHeader_Unknown1(_lod, _group);

		// TODO: 8 bytes (mask and alpha, maybe?) are read too much in one of these, masking it crash, find out which one,
		// and/or is it a another material not yet in switch block
	case ERigidMaterial::custom_terrain:
		return RigidModelV2::File_Importer_Common::readMeshHeader_CustomTerrain(_lod, _group);

	case ERigidMaterial::weighted:
	case ERigidMaterial::weighted_dirtmap:

	case ERigidMaterial::weighted_decal:
	case ERigidMaterial::weighted_decal_dirtmap:

	case ERigidMaterial::weighted_skin:
	case ERigidMaterial::weighted_skin_dirtmap:
	case ERigidMaterial::weighted_skin_decal_dirtmap:

	case ERigidMaterial::default_material:
	{
		if (m_spoData->oFileHeader.oFileInfoHeader.dwModelVersion == 5)
			return RigidModelV2::File_Importer_Common::readMeshHeader_Default_Weighted_v5(_lod, _group);
		else
			return RigidModelV2::File_Importer_Common::readMeshHeader_Default_Weighted_v6_v7_v8(_lod, _group);
	}
	default:
		return RigidModelV2::File_Importer_Common::readMeshHeader_Default_Weighted_v6_v7_v8(_lod, _group);
	}
}

bool RigidModelV2::File_Importer_Common::readMeshHeader(ERigidMaterial _eRigidMaterial, size_t _lod, size_t _group)
{
	return true;
}

bool RigidModelV2::File_Importer_Common::readLodHeaderBlock_V5_V6()
{
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->readMem(&itLod.dwMeshCount, 4);
		m_spoStream->readMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->readMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->readMem(&itLod.dwStartOffset, 4);
		m_spoStream->readMem(&itLod.fVisibilityDistance, 4);

		/*if (!oValidDator.validateLodHeader(&itLod))
		{
			return false;
		}*/
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readLodHeaderBlock_V7_V8()
{
	m_spoData->oLodHeaderBlock.vecElements.resize(m_spoData->oFileHeader.wLod);

	for (auto& itLod : m_spoData->oLodHeaderBlock.vecElements)
	{
		m_spoStream->readMem(&itLod.dwMeshCount, 4);
		m_spoStream->readMem(&itLod.dwVerticesDataLength, 4);
		m_spoStream->readMem(&itLod.dwIndicesDataLength, 4);
		m_spoStream->readMem(&itLod.dwStartOffset, 4);
		m_spoStream->readMem(&itLod.fVisibilityDistance, 4);
		m_spoStream->readMem(&itLod.dwAuthoredLodNumber, 4);
		m_spoStream->readMem(&itLod.oQualityLevel, 4);

		//if (!oValidDator.validateLodHeader(&itLod))
		//{
		//	return false;
		//}
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshPreHeader(size_t _lod, size_t _group)
{
	m_spoStream->readMem(&m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader, sizeof(RigidModelV2::Common::MeshPreHeader));

	// calculate the vertex size based on the groups block size fields.
	m_spoData->vecLODs[_lod].vecMeshBlocks[_group].calculateVertexSize();

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_Default_Weighted_v5(size_t _lod, size_t _group)
{
	auto first = m_spoStream->tellp();

	/*
	//m_spoStream->readMem(&o.strShaderName, 12);
	m_spoStream->readMem(&o.oFlags, sizeof(o.oFlags));
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));
	m_spoStream->readMem(&o.szGroupName, 32);
	m_spoStream->readMem(&o.szTextureDirectory, 256);
	m_spoStream->readMem(&o.uk2_512, 256);
	m_spoStream->readMem(&o.wUK_2, 2);
	m_spoStream->readMem(&o.vPivot, 4 * 3);
	m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	size_t header_size = 0;

	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);

	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	auto first = m_spoStream->tellp();*/

	//wchar_t ch = L'\0';
	//o.strShaderName.clear();
	//do {
	//	m_spoStream->readMem(&ch, 2);
	//	o.strShaderName += ch;
	//} while (ch != L'\0');

	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	m_spoStream->readWSZ(o.wstrShaderName);
	m_spoStream->readMem(o.oFlags.szFlags, 40);
	m_spoStream->readMem(&o.VertexFormatId, 2);

	o.wstrMeshName.resize(64);
	m_spoStream->readMem(&o.wstrMeshName[0], 64);

	o.wstrTextureDirectory.resize(512);
	m_spoStream->readMem(&o.wstrTextureDirectory[0], 512);

	m_spoStream->readMem(o.uk2_512, 512);
	m_spoStream->readMem(&o.wUK_2, 2);
	m_spoStream->readMem(&o.vPivot, 4 * 3);
	m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	size_t header_size = 0;

	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);

	m_spoStream->readMem(&o.oValues, sizeof(o.oValues));

	o.vecUnknownDataBuffer.resize(140 - sizeof(o.oValues));
	m_spoStream->readMem(&o.vecUnknownDataBuffer[0], 140 - sizeof(o.oValues));

	// copy+convert the 16 bit strings into the 8 bit ones
	o.strMeshName = tools::wstring_to_string(o.wstrMeshName);
	o.strShaderName = tools::wstring_to_string(o.wstrShaderName);
	o.strTextureDirectory = tools::wstring_to_string(o.wstrTextureDirectory);

	//header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize -
	//	Common::TextureElement::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount -
	//	Common::AttachmentPointTableEntry::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount
	//	- 8;

	////sizeof(Common::GroupPreHeader);

	//auto bytes_read = m_spoStream->tellp() - first;

	//auto bytes_left = ((header_size)-bytes_read) - sizeof(Common::MeshPreHeader);

	//o.vecUnknownDataBuffer.resize(bytes_left);

	//auto bytes_read1 = m_spoStream->tellp();

	//m_spoStream->readMem(&o.vecUnknownDataBuffer[0], bytes_left);

	///*m_spoStream->readMem(&o.oValues, sizeof(o.oValues));

	//m_spoStream->readMem(&o.Unknown140, 140 - 32);*/

	//auto bytes_read3 = m_spoStream->tellp() - bytes_read1;

	//auto bytes_read2 = m_spoStream->tellp() - first;

	auto bytes_read = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_Default_Weighted_v6_v7_v8(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	auto& mesh = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	auto first = m_spoStream->tellp();

	o.strShaderName.resize(12);
	m_spoStream->readMem(&o.strShaderName[0], 12);
	m_spoStream->readMem(&o.oFlags, 20);
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));

	o.strMeshName.resize(32);
	m_spoStream->readMem(&o.strMeshName[0], 32);

	o.strTextureDirectory.resize(256);
	m_spoStream->readMem(&o.strTextureDirectory[0], 256);

	m_spoStream->readMem(&o.uk2_512, 256);

	m_spoStream->readMem(&o.wUK_2, 2);
	m_spoStream->readMem(&o.vPivot, 4 * 3);
	m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);

	m_spoStream->readMem(&o.oValues, sizeof(o.oValues));

	o.vecUnknownDataBuffer.resize(140 - sizeof(o.oValues));
	m_spoStream->readMem(&o.vecUnknownDataBuffer[0], (140) - sizeof(o.oValues));

	auto bytes_read = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_Weighted_Texture_Blend(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	if (m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.RigidMaterialId == ERigidMaterial::tree_billboard_material)
	{
		int debug_2 = 1;
	}

	auto first = m_spoStream->tellp();

	char ch = '\0';
	o.strShaderName.clear();
	do {
		m_spoStream->readMem(&ch, 1);
		o.strShaderName += ch;
	} while (ch);

	//m_spoStream->readMem(&o.strShaderName, 12);
	m_spoStream->readMem(&o.oFlags, 5 * 4);
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));
	m_spoStream->readMem(&o.strMeshName, 32);
	//m_spoStream->readMem(&o.szTextureDirectory, 256);
	//m_spoStream->readMem(&o.uk_256, 256);
	//m_spoStream->readMem(&o.wUK_2, 2);
	//m_spoStream->readMem(&o.vPivot, 4 * 3);
	//m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	size_t header_size = 0;
	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);

	header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize -
		Common::TextureElement::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount -
		Common::AttachmentPointTableEntry::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount
		- 8;

	//sizeof(Common::GroupPreHeader);

	auto bytes_read = m_spoStream->tellp() - first;

	auto bytes_left = ((header_size)-bytes_read) - sizeof(Common::MeshPreHeader);

	o.vecUnknownDataBuffer.resize(bytes_left);

	auto bytes_read1 = m_spoStream->tellp();

	m_spoStream->readMem(&o.vecUnknownDataBuffer[0], bytes_left);

	/*m_spoStream->readMem(&o.oValues, sizeof(o.oValues));

	m_spoStream->readMem(&o.Unknown140, 140 - 32);*/

	auto bytes_read3 = m_spoStream->tellp() - first;

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_AlphaBlend(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	auto header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].getMeshHeaderSize();

	//char ch = '\0';
	//o.strShaderName.clear();
	//do {
	//	m_spoStream->readMem(&ch, 1);
	//	o.strShaderName += ch;
	//} while (ch);

	m_spoStream->readMem_As<std::string>(o.strShaderName);

	//m_spoStream->readMem(&o.strShaderName, 12);
	m_spoStream->readMem(&o.oFlags, 4 * 4);
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));
	m_spoStream->readMem(&o.wUK_2, 2);
	//m_spoStream->readMem(&o.szGroupName, 32);

	//m_spoStream->readMem(&o.szGroupName, 32);

	m_spoStream->readMem(&o.strTextureDirectory, 256);

	/*spoStream->readMem(&o.uk_256, 256);
	m_spoStream->readMem(&o.wUK_2, 2);
	m_spoStream->readMem(&o.vPivot, 4 * 3);
	m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);*/

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_Unknown1(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	auto header_size1 = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].getMeshHeaderSize();

	/*if (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.RigidMaterialId == ERigidMaterial::unknown1)
	{
		int debug_2 = 1;
	}*/

	auto first = m_spoStream->tellp();

	m_spoStream->readMem_As<std::string>(o.strShaderName);
	//m_spoStream->readMem(&o.strShaderName, 12);
	m_spoStream->readMem(&o.oFlags, 4 * 4);
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));

	m_spoStream->readMem(&o.strMeshName, 32);

	//m_spoStream->readMem(&o.szTextureDirectory, 80);

	//m_spoStream->readMem(&o.uk_256, 256);
	m_spoStream->readMem(&o.wUK_2, 2);
	//m_spoStream->readMem(&o.vPivot, 4 * 3);
	/*m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	size_t header_size = 0;

	m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);*/

	auto header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize -
		Common::TextureElement::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwTextureCount -
		Common::AttachmentPointTableEntry::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader.dwAttachmentPointCount
		- 8;

	//sizeof(Common::GroupPreHeader);

	auto bytes_read = m_spoStream->tellp() - first;

	auto bytes_left = ((header_size)-bytes_read) - sizeof(Common::MeshPreHeader);

	o.vecUnknownDataBuffer.resize(bytes_left);

	m_spoStream->readMem(&o.vecUnknownDataBuffer[0], bytes_left);

	return true;
}

bool RigidModelV2::File_Importer_Common::readMeshHeader_CustomTerrain(size_t _lod, size_t _group)
{
	auto& o = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oSubMeshHeader;

	auto header_size1 = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].getMeshHeaderSize();

	/*if (m_spoData->vecLODs[_lod].vecGroups[_group].oPreHeader.RigidMaterialId == ERigidMaterial::unknown1)
	{
		int debug_2 = 1;
	}*/

	auto first = m_spoStream->tellp();

	m_spoStream->readMem_As<std::string>(o.strShaderName);
	//m_spoStream->readMem(&o.strShaderName, 12);
	m_spoStream->readMem(&o.oFlags, 4 * 4);
	m_spoStream->readMem(&o.VertexFormatId, sizeof(o.VertexFormatId));

	//m_spoStream->readMem(&o.szGroupName, 32);

	m_spoStream->readMem(&o.strTextureDirectory, 256);

	//m_spoStream->readMem(&o.uk_256, 256);
	//m_spoStream->readMem(&o.wUK_2, 2);
	//m_spoStream->readMem(&o.vPivot, 4 * 3);
	//m_spoStream->readMem(&o.oTransformation, (4 * 3 * 4) * 3);

	size_t header_size = 0;

	/*m_spoStream->readMem(&o.bone_id_building, 4);
	m_spoStream->readMem(&o.bone_id_unknown, 4);

	m_spoStream->readMem(&o.dwAttachmentPointCount, 4);
	m_spoStream->readMem(&o.dwTextureCount, 4);*/

	header_size = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.uiTextureAndAttchmentBlockSize/* -
		Common::TextureElement::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecGroups[_group].oSubMeshHeader.dwTextureCount -
		Common::AttachmentPointTableEntry::_size_v6_v7_v8 * m_spoData->vecLODs[_lod].vecGroups[_group].oSubMeshHeader.dwAttachmentPointCount
		- 8*/;

		//sizeof(Common::GroupPreHeader);

	long bytes_read = m_spoStream->tellp() - first;

	long bytes_left = ((header_size)-bytes_read) - sizeof(Common::MeshPreHeader);

	o.vecUnknownDataBuffer.resize(bytes_left);

	if (o.vecUnknownDataBuffer.size())
	{
		m_spoStream->readMem(&o.vecUnknownDataBuffer[0], bytes_left);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oAttachBlock.vecAttachmentPoints.resize(oGroup.oSubMeshHeader.dwAttachmentPointCount);

	for (auto& it : oGroup.oAttachBlock.vecAttachmentPoints)
	{
		it.szAttachmentPointName.resize(32);
		m_spoStream->readMem(&it.szAttachmentPointName[0], 32);
		m_spoStream->readMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->readMem(&it.dwBoneId, 4);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readAttachmentPointBlock_V5(size_t _lod, size_t _group)
{
	auto& ref_vecAttachmentPoints = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oAttachBlock.vecAttachmentPoints;

	for (auto& it : ref_vecAttachmentPoints)
	{
		it.wszAttachmentPointName.resize(512);
		m_spoStream->readMem(&it.wszAttachmentPointName[0], 256 * 2);
		m_spoStream->readMem(&it.o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));
		m_spoStream->readMem(&it.dwBoneId, 4);

		// convert the 16 bit char to 8 bit char for internal storage
		it.szAttachmentPointName = tools::wstring_to_string(it.wszAttachmentPointName);
	}

	return true;
}

bool RigidModelV2::File_Importer_Common::readTextureBlock_V6_V7_V8(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		it.szTextureDirAndFileName.resize(256);
		m_spoStream->readMem(&it.Type, 4);
		m_spoStream->readMem((void*)it.szTextureDirAndFileName.data(), 256);
	}

	m_spoStream->readMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->readMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Importer_Common::readTextureBlock_V5(size_t _lod, size_t _group)
{
	auto& oGroup = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	oGroup.oMaterialBlock.vecTextures.resize(oGroup.oSubMeshHeader.dwTextureCount);

	for (auto& it : oGroup.oMaterialBlock.vecTextures)
	{
		it.wszTextureDirAndFileName.resize(512);
		m_spoStream->readMem(&it.Type, 4);
		m_spoStream->readMem(&it.wszTextureDirAndFileName[0], 256 * 2);

		it.szTextureDirAndFileName = tools::wstring_to_string(it.wszTextureDirAndFileName);
	}

	m_spoStream->readMem(&oGroup.oMaterialBlock.dwMaskFlags, 4);
	m_spoStream->readMem(&oGroup.oMaterialBlock.dwAlphaFlags, 4);

	return true;
}

bool RigidModelV2::File_Importer_Common::readVertexBlock_STANDARD_Raw(size_t _lod, size_t _group)
{
	auto& oGroupBlock = m_spoData->vecLODs[_lod].vecMeshBlocks[_group];

	//size_t vertex_data_size = oGroupBlock.getDataSizeVertices();

	size_t vertex_data_size = oGroupBlock.oPreHeader.dwVertexData_TextAndAttach_BlockSize -
		oGroupBlock.oPreHeader.uiTextureAndAttchmentBlockSize;

	if (vertex_data_size == 0)
		return true;

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

	return true;

	// TODO: DO NOT leave as "return" or decoding of mesh will be skipped,
	//return true;

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
	auto& ref_vecIndices = m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oMeshBlock.vecIndices;

	if (m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwIndexCount == 0)
	{
		return true;
	}

	ref_vecIndices.resize(m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwIndexCount);

	m_spoStream->readMem(ref_vecIndices.data(), m_spoData->vecLODs[_lod].vecMeshBlocks[_group].oPreHeader.dwIndexCount * 2);

	return true;
}

bool RigidModelV2::File_Importer_Common::ErrorString(const string& _error)
{
	m_spoData->m_strLastErrorString = _error;
	_log_action_error(_error);

	return false;
}

std::shared_ptr<RigidModelV2::File_Importer_Common> RigidModelV2::File_Importer_Common::create(void* pSrcMem, size_t _size_in_bytes)
{
	auto spoFileImporter = std::shared_ptr < RigidModelV2::File_Importer_Common>(new RigidModelV2::File_Importer_Common);

	spoFileImporter->m_spoStream = std::shared_ptr<SystemLib::DataStream>(new SystemLib::DataStream(pSrcMem, _size_in_bytes));

	spoFileImporter->m_spoData = std::shared_ptr<Common::CommonFile>(new Common::CommonFile);

	if (!spoFileImporter->read())
	{
		spoFileImporter->m_bIsValid = false;
	}

	return spoFileImporter;
}

std::shared_ptr<RigidModelV2::File_Importer_Common> RigidModelV2::File_Importer_Common::create(const std::wstring& _strPath)
{
	size_t file_size = tools::getFileSize(_strPath);

	vector<uint8_t> vecBuffer(file_size);

	ifstream inFile(_strPath, ios::binary);
	if (!inFile.is_open())
	{
		return nullptr;
	}

	inFile.read((char*)&vecBuffer[0], file_size);
	inFile.close();

	// create importer from memory, return it
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