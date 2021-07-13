#pragma once

#include <DirectXMath.h>
#include <vector>

#include "RigidModelV2_Enums.h"
#include "RigidModelV2_MeshStructs.h"
//#include "..\RenderLib\vertex_layouts.h"
#include "string_constants.h"

namespace RigidModelV2
{
	constexpr static uint32_t RMV2_SIGNATURE = 0x32564D52;

	struct FileInfoHeader_V5_V6_V7_V8
	{
		union _Signature {
			char szSignature[4];		// Should always be "RMV2"
			uint32_t dwSignature = 0;	// the 32 bit numberical eqauivalent for "RV2" = 0x32564D52
		} Signature = { 0 };

		uint32_t dwModelVersion = 0;		// 6 for Rome2/Atilla/Thrones, 7 for WH/WH2, 8 for 3K
	};

	struct FileHeader_V6_V7_V8
	{
		FileInfoHeader_V5_V6_V7_V8 oFileInfoHeader;

		// count is split up into two 16 biLPARAM lParams and the lower 16 bit is used,
		// because in some rare models the upper 16 bits ("G") is filled with some info
		union
		{
			uint16_t wLod, UK_16Bit;
			uint32_t dwLodCount = 0;
		};

		char szSkeletonId[128] = { 0 };
	};

	struct LodQualityLevel_V7_V8
	{
		uint8_t quality_level = 0;
		uint8_t flag1 = 125;
		uint8_t flag2 = 136;
		uint8_t flag3 = 174;
	};

	struct LodHeaderElement_V5_V6
	{
		uint32_t dwGroupCount = 0;
		uint32_t dwVerticesDataLength = 0;
		uint32_t dwIndicesDataLength = 0;
		uint32_t dwStartOffset = 0;
		float fVisibilityDistance = 100.0f;		// distance when engine switches to next LOD
	};

	struct LodHeaderElement_V7_V8
	{
		uint32_t dwGroupCount = 0;
		uint32_t dwVerticesDataLength = 0;
		uint32_t dwIndicesDataLength = 0;
		uint32_t dwStartOffset = 0;
		float fVisibilityDistance = 100.0f;		// distance when engine switches to next LOD

		uint32_t dwAuthoredLodNumber = 0;					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that
		LodQualityLevel_V7_V8 oQualityLevel; // v7/v8 related to graphics setting "mesh quality"
	};

	namespace Common
	{
		struct FileHeader
		{
			static size_t size_v6_v7_v8() { return 8 + 4 + 128; };

			FileInfoHeader_V5_V6_V7_V8 oFileInfoHeader;

			// count is split up into two 16 biLPARAM lParams and the lower 16 bit is used,
			// because in some rare models the upper 16 bits ("G") is filled with some info
			union
			{
				uint16_t wLod, UK_16Bit;
				uint32_t dwLodCount = 0;
			};

			char szSkeletonId[128] = "";
			wchar_t wszSkeletonId[128] = L"";
		};

		// TODO: finish this
		static constexpr bool isKnownMaterial(ERigidMaterial _MaterialID)
		{
			switch (_MaterialID)
			{
			case ERigidMaterial::tree_billboard_material:
				return false;

			case ERigidMaterial::alpha_blend:
				return false;

			case ERigidMaterial::weighted_texture_blend:
				return false;

			case ERigidMaterial::unknown1:
				return false;

				// TODO: 8 bytes (mask and alpha, maybe?) are read too much in one of these, masking it crash, find out which one,
				// and/or is it a another material not yet in switch block
			case ERigidMaterial::custom_terrain:
				return false;

			case ERigidMaterial::weighted:
				return true;

			case ERigidMaterial::weighted_dirtmap:
				return true;

			case ERigidMaterial::weighted_decal:
				return true;

			case ERigidMaterial::weighted_decal_dirtmap:
				return true;

			case ERigidMaterial::weighted_skin:
				return true;

			case ERigidMaterial::weighted_skin_decal:
				return true;

			case ERigidMaterial::weighted_skin_dirtmap:
				return true;

			case ERigidMaterial::weighted_skin_decal_dirtmap:
				return true;

			case ERigidMaterial::decal_dirtmap:
				return true;
			case ERigidMaterial::default_material:
				return true;

			default:
				return false;
			}
		}

		struct LodHeaderElementCommon
		{
			static constexpr size_t size_v5_v6 = 5 * 4;
			static constexpr size_t size_v7_v8 = 7 * 4;

			uint32_t dwMeshCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 100.0f;		// distance when engine switches to next LOD

			uint32_t dwAuthoredLodNumber = 0;					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that

			//LodQualityLevel_V7_V8 oQualityLevel; // v7/v8 related to graphics setting "mesh quality"
			uint32_t oQualityLevel; // v7/v8 related to graphics setting "mesh quality"
		};

		struct LodHeaderBlock
		{
			std::vector<Common::LodHeaderElementCommon> vecElements;
		};

		struct MeshPreHeader
		{
			ERigidMaterial RigidMaterialId = ERigidMaterial::eMAT_ERROR_NOT_SET;		// Indicates the material of the group, some material make the group chunk larger, which merits further discovery
		// number of bytes from the start of this header to the begining of the next group (starting at the header)
			uint32_t dwMeshBlockSize = 0;	// Size of the entire group (header + attachment blocks + texture blocks + vertices + indices)

			uint32_t uiTextureAndAttchmentBlockSize = 0; // The combined size of all attachment blocks and all texture blocks in current group
			uint32_t dwVertexCount = 0;		// Count of vertices in group

			uint32_t dwVertexData_TextAndAttach_BlockSize = 0; // size of vertex data + attachment point table
			uint32_t dwIndexCount = 0;		// 	Number of indices// Absolute offset of next grouop = (sum of preverious group sizes) + current group size + LodHeaders[lod].offset

			DirectX::XMFLOAT3 vMinBB;
			DirectX::XMFLOAT3 vMaxBB;
		};

		struct MeshHeader
		{
			/*float fGroupMinimumX = 0.0f;;
			float fGroupMinimumY = 0.0f;;
			float fGroupMinimumZ = 0.0f;
			float fGroupMaximumX = 1.7f;;
			float fGroupMaximumY = 1.7f;;
			float fGroupMaximumZ = 1.7f;*/

			// offset 48
			std::string  strShaderName = "123456789123";		// shader name is zero terminated, paddded
			std::wstring  wstrShaderName = L"123456789123";		// shader name is zero terminated, paddded

			// Unknown structure 16 and 20 bytes long have been encountered, 20 for most cases, only terrain meshes seem to be 16, unsure on how to detect size
			// These values change when loading different types of model "weighted model" "weapon", "building"
			// but I dont know what it means exactly yet
			struct _flags
			{
				// not clear on what these bytes mean, hence the test union, they have some r____elative no vertex type.
				// Though seems not be vertex size, even if that fits for two vertex types

				//uint8_t flags1[4] = { 129, 5, 0, 0 }; // 32512; // 32512  for WH character model / weapons
				//
				//uint8_t flags2[4] = { 73, 0, 78, 0 }; // 32; // 32  for WH character model / weapons
				//	uint32_t flags3 = 51;
				//	uint32_t flags4 = 0;
				//	uint32_t flags5 = 0;

				//char str[5 * 4];

				char szFlags[60];
				/*uint8_t flags1[4] = { 0 };
				uint8_t flags2[4] = { 0 };
				uint8_t flags3[4] = { 0 };
				uint8_t flags4[4] = { 0 };
				uint8_t flags5[4] = { 0 };
				*/
			} oFlags;

			//offset 80
			EVertexFormat VertexFormatId = EVertexFormat::eERROR_NOT_SET;

			std::string strMeshName = "";		// First believed to be zero padded 32 char, but is not present at all in certain "obscure" RMV2 "sub formats"
			std::wstring wstrMeshName = L"";		// First believed to be zero padded 32 char, but is not present at all in certain "obscure" RMV2 "sub formats"
			//wchar_t szwGroupName[32];	// utf-16 version of abovestd::string, seems only to be used in RMV2 version 5, (see File header)

			// !!NOT!! part of file format

			std::string strTextureDirectory = "";		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory
			std::wstring wstrTextureDirectory = L"";		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory
			//wchar_t wszTextureDirecory[256];		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory

			// !!NOT!! part of file format
			char uk2_512[512];
			// in building models it seems like this structs contains values with this types 8 times
			//struct _S2
			//{
			//	_S2()
			//	{
			//		memset(this, 0, sizeof(_S2));
			//	}

			//	// this type patterns repeats itself, but only when offset "lines up", further investigation.....
			//	uint32_t dw0;//float f1;
			//	uint32_t dw1;
			//	uint32_t dw2;
			//	uint32_t dw3;
			//	uint32_t dw4;// float f2;
			//	uint32_t dw5;
			//	uint32_t dw6;
			//	uint32_t dw7;
			//};
			//_S2 Unknown3_256[8];

			// Value changes also with different model types.
			//struct U
			//{ // Uknown
			//	uint32_tui;
			//};

			uint16_t wUK_2;

			/*struct _ui
			{
				uint16_t value;
			} ui;*/
			//
			//uint16_t uiUK_2;
			// the new origin
			// the model's pivov point. For weapons etc. Usually/aways? 0 for rigged models.

			DirectX::XMFLOAT3 vPivot = { 0,0,0 };

			// 3x4 indentity matrices. They are stored like CA stores the bind pose matrices for skeletons.
			// So the last row is implicit = 0, 0, 0, 1 (you have to put it in yourself)
			struct _Transfomation
			{
				DirectX::XMFLOAT3X4 matrix1 =
				{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0
				};

				DirectX::XMFLOAT3X4 matrix2 =
				{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0
				};
				DirectX::XMFLOAT3X4 matrix3 =
				{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0
				};;
			} oTransformation;

			int32_t bone_id_building = -1;
			int32_t bone_id_unknown = -1;

			//I scale the whole model so it's probably not a single block!
			uint32_t dwAttachmentPointCount = 0;	// Count of weapon attachment points blocks
			uint32_t dwTextureCount = 0;			// Count of texture info blocks in group

			struct  _values {
				uint32_t value1 = 0;	// 0
				uint32_t value2 = 0;	// 0
				uint32_t value3 = 1;	// 1
				uint32_t value4 = 0;	// 0
				uint32_t value5 = 0;	// 0
				uint32_t value6 = 0;	// 0
				uint32_t value7 = 1;	//  1
				uint32_t value8 = 0;	// 0
			} oValues;

			static constexpr size_t UnknownDataSize = 140 - sizeof(_values);
			std::vector<uint8_t> vecUnknownDataBuffer;
			//140 uint8_ts - ? // No idea.¨'
		};

		struct AttachmentPointTableEntry
		{
			static size_t constexpr _size_v5 = (2 * 32) + (3 * 4 * 4) + 4;
			static size_t constexpr _size_v6_v7_v8 = 84; // (32) + (3 * 4 * 4) + 4;

			// name of attachment point bone, has to be a bone from the skeleton referred to in the file header
			std::string szAttachmentPointName;
			std::wstring wszAttachmentPointName; // for wide char / unicode, not yes supported

			// transformtion, scale, rotation, translation are used by the game, but vanilla model always (?) have this to indenity
			DirectX::XMFLOAT3X4 o3x4Matrix =
				DirectX::XMFLOAT3X4(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0);

			uint32_t dwBoneId = 0; // attachment point bne id, referes to a bone in the skeleton that something (props) can be attached to.
		};

		struct TextureElement
		{
			static constexpr size_t _size_v6_v7_v8 = 256 + 4;
			static constexpr size_t _size_v5 = (2 * 256) + 4;

			ETextureType Type = ETextureType::eTEX_ERROR_NOT_SET; // texture type. types defines in texture_type
			std::string szTextureDirAndFileName = ""; // Internal (pack) texture path, zero terminated, and zero-padded
			std::wstring wszTextureDirAndFileName = L""; // Internal (pack) texture path, zero terminated, and zero-padded
		}; // END: texture data

		struct ModelVertex
		{
			DirectX::XMFLOAT4 position = { 0, 0, 0, 0 };

			DirectX::XMFLOAT2 uv = { 0, 0 };
			//DirectX::XMFLOAT2 tex2 = { 0, 0 };

			DirectX::XMFLOAT4 normal = { 0, 0, 0, 0 };
			DirectX::XMFLOAT4 tangent = { 0, 0, 0, 0 };
			DirectX::XMFLOAT4 bitangent = { 0, 0, 0, 0 };

			DirectX::XMFLOAT4 bone_weights = { 1.0f, 0, 0, 0 };
			DirectX::XMUINT4 bone_indices = { 0, 0, 0, 0 };

			DirectX::XMFLOAT4 color = { 0, 0, 0, 0 };
		};

		struct MaterialBlock
		{
			size_t size_v6_v7_v8()
			{
				return (vecTextures.size() * TextureElement::_size_v6_v7_v8) + 8;
			}

			std::vector<TextureElement> vecTextures;
			uint32_t dwMaskFlags = 0;
			uint32_t dwAlphaFlags = 0;
		};

		struct AttachBlock
		{
			size_t size_v6_v7_v8()
			{
				return vecAttachmentPoints.size() * AttachmentPointTableEntry::_size_v6_v7_v8;
			}

			std::vector<AttachmentPointTableEntry> vecAttachmentPoints;
		};

		struct MeshGeometryData
		{
			void packMesh();

			void unPackMesh();

			// unpacks the float16 and 8 bit unorm based vertices into float based vertices
			void unpackAs_Default(size_t _vertex_size,
				DirectX::SimpleMath::Vector3 _vPivot = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f),
				int32_t _bone_index = -1
			); // treats as static "default" data
			void unpackAs_Weighted2_V5_V6_V7(size_t _vertex_size); // treats the vertex data as cinematic vertices
			void unpackAs_Weighted4_V5_V6_V7(size_t _vertex_size); // treats the vertex data as cinematic vertices

			// pack the vertex data from float32 based to float16 and 8 bit unorm based
			void makePacked_Default_V5_V6_V7(size_t _vertex_size);
			void makePacked_Weighted2_V5_V6_V7(size_t _vertex_size);
			void makePacked_Weighted4_V5_V6_V7(size_t _vertex_size);

			std::vector<ModelVertex> vecVertices;
			std::vector<uint8_t> vecVertexDataRawBuffer;
			std::vector<uint16_t> vecIndices;
		};

		struct ExtraMaterialDataBlock
		{
			static size_t sizeFromMaterialId(ERigidMaterial Material);

			std::vector<uint8_t> vecExtraMaterialData;
		};

		struct MeshBlock
		{
			void copy(const MeshBlock&);

			/*
			sets a new mesh for this group, and recalculates the group's header fields
			One also needs to call the appropiate CommonFile::updateFields_() to update the file's header
			 */
			void setMesh_v6_v7_v8(
				std::vector <ModelVertex>& _vecVertices,
				std::vector <uint16_t>& _vecIndices
			);

			void setTextures(const std::vector<TextureElement>& _vecTextures =
				{
					{ETextureType::eDiffuse, "commontextures/default_grey.dds", L"commontextures/default_grey.dds"},

					{ETextureType::eNormal, "commontextures/default_normal.dds", L"commontextures/default_normal.dds"},

					{ETextureType::eSpecular, "commontextures/default_grey.dds", L"commontextures/default_grey.dds"},

					{ETextureType::eGlossMap, "commontextures/default_grey.dds", L"commontextures/default_grey.dds"}
				}
			);

			void setDefaultTextures();

			// get an already set/calculate vertex size
			size_t getVertexSize();

			// calculates the vertex size from the block size fieds
			size_t calculateVertexSize();

			// set vertex size
			void setVertexSize(size_t size);

			size_t getDataSizeVertices();
			size_t getDataSizeIndices();

			size_t getMeshHeaderSize();

			// updates the groupsize field, and returns new value
			size_t updateGroupFields_v6_v7_v8();

		public:
			void simplifyGroup(float reduce_rate = 1.0, float _error = 0.1, bool _bUseSloppy = false);
			void cleanUpMesh();

		public:
			MeshPreHeader oPreHeader;
			MeshHeader oSubMeshHeader;

			AttachBlock oAttachBlock;
			MaterialBlock oMaterialBlock;
			ExtraMaterialDataBlock oExtraMaterialBlock;
			MeshGeometryData oMeshBlock;

		private:
			size_t vertex_size = 0;
		};

		struct LodBLock
		{
			void simplifyLod(float reduce_rate = 1.0, float _error = 0.1, bool _bUseSloppy = false);
			void setDefaultTextures();

			uint32_t updateSize();

			uint32_t getVertexDataSize();
			uint32_t getIndexDataSize();

			MeshBlock& operator[](size_t index)
			{
				return vecMeshBlocks[index];
			}

			std::vector<MeshBlock> vecMeshBlocks;
			float fLodDistance = -1;
		};

		class Validator
		{
		public:
			bool validateFileHeader(const Common::FileHeader* _poFileHeader);

			bool validateLodHeader(const Common::LodHeaderElementCommon* _poFileHeader);

			std::string validateMeshPreHeader(const RigidModelV2::Common::MeshPreHeader* _poFileHeader);
			//bool validateSubMeshHeader(const RigidModelV2::Common::GroupHeader_Default_Weighted* _poFileHeader);

			std::string strLastErrorString = "";

			std::map<ERigidMaterial, bool> mapMaterialsLoaded;
		};

		struct CommonFile
		{
			// updates counter and block size fields
			void updateFileFields()
			{
				if (oFileHeader.oFileInfoHeader.dwModelVersion < 7)
				{
					updateFileFields_v5_v6();
				}
				else
				{
					updateFileFields_v7_v8();
				}
			}
		private:
			void updateFileFields_v5_v6();
			void updateFileFields_v7_v8();

		public:
			MeshBlock* getGroup(size_t _lod, size_t _group);

			// makes it position to access lod/group byt [lod][group] as "group" alkso has an operator like this
			LodBLock& operator[](size_t index)
			{
				return vecLODs[index];
			}

			FileHeader oFileHeader;
			LodHeaderBlock oLodHeaderBlock;

			std::vector<LodBLock> vecLODs;

			bool isValid();
			std::string getLastErrorString();

			static bool ErrorString(std::shared_ptr<CommonFile> file, const std::string& _error);
			Validator oValidator;

			//private:
			bool m_bIsValid = false;
			std::string m_strLastErrorString = "";
		};
	};
};
