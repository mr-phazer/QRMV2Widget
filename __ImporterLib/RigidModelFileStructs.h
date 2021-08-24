#pragma once
#pragma warning( disable : 26812)

//#include "pch.h"

#include "..\SystemLib\data_stream.h"
#include "..\SystemLib\ISerializeabe.h"
#include "..\SystemLib\SerializeVector.h"

#include "RigidModelConst.h"
#include "LODHeader.h"

namespace ImporterLib
{
	namespace RigidModel
	{
		struct RigidMaterial
		{
			enum ETypes : uint32_t
			{
				bow_wave = 22,
				non_renderable = 26,
				texture_combo_vertex_wind = 29,
				texture_combo = 30,
				decal_waterfall = 31,
				standard_simple = 32,
				campaign_trees = 34,
				point_light = 38,
				static_point_light = 45,
				debug_geometry = 46,
				custom_terrain = 49,
				weighted_cloth = 58,
				cloth = 60,
				collision = 61,
				collision_shape = 62,
				tiled_dirtmap = 63,
				ship_ambientmap = 64,
				weighted = 65,
				projected_decal = 67,
				default_material = 68,
				grass = 69,
				weighted_skin = 70,
				decal = 71,
				decal_dirtmap = 72,
				dirtmap = 73,
				tree = 74,
				tree_leaf = 75,
				weighted_decal = 77,
				weighted_decal_dirtmap = 78,
				weighted_dirtmap = 79,
				weighted_skin_decal = 80,
				weighted_skin_decal_dirtmap = 81,
				weighted_skin_dirtmap = 82,
				water = 83,
				unlit = 84,
				weighted_unlit = 85,
				terrain_blend = 86,
				projected_decal_v2 = 87,
				ignore = 88,
				tree_billboard_material = 89,
				water_displace_volume = 91,
				rope = 93,
				campaign_vegetation = 94,
				projected_decal_v3 = 95,
				weighted_texture_blend = 96,
				projected_decal_v4 = 97,
				global_terrain = 98,
				decal_overlay = 99,
				alpha_blend = 100,

				unknown = 20000
			};

			static std::string getStringFromId(ETypes ID);
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Texture data struct
		//
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct ITextureData : SystemLib::ISerializable { };

		struct TextureData : SystemLib::ISerializable
		{
			TextureData()
			{
				//memset(this, 0, sizeof(TextureData));
				szTextureDirAndFileName = R"(test_folder\place_holder.dds)";
				szTextureDirAndFileName.resize(256);

				wcscpy_s(szwTextureDirAndFileName, LR"(test_folder\place_holder.dds)");

				Type = TextureType::EValues::eEmpty;
			}

			size_t static constexpr _size()
			{
				return sizeof(Type) + 256;
			}

			/*virtual size_t size() override
			{
				return sizeof(uiTextureType) + 256;
			}*/

			/// <summary>
			///  Serializes object, writes to disk
			/// </summary>
			/// <param name="oOut">ofstream, opened for binary read</param>
			/// <returns></returns>

			SystemLib::return_value write(SystemLib::DataStream& _out)
			{
				_out.writeMem((char*)&Type, sizeof(Type));
				szTextureDirAndFileName.resize(256);
				_out.writeMem(&szTextureDirAndFileName[0], 256);

				return true;
			}

			SystemLib::return_value read(SystemLib::DataStream& _In)
			{
				_In.readMem((char*)&Type, sizeof(Type));
				szTextureDirAndFileName.resize(256);
				_In.readMem(&szTextureDirAndFileName[0], 256);

				return true;
			}

			TextureType::EValues Type; // texture type. types defines in texture_type
			std::string szTextureDirAndFileName = std::string(256, 0);
			//char szTextureDirAndFileName[256]; // Internal texture folder + filname.dds  - Zero-terminated and padded string
			wchar_t szwTextureDirAndFileName[256]; // Internal texture folder + filname.dds  - Zero-terminated and padded string
		}; // END: texture data

		struct IMaterial : SystemLib::ISerializable { };

		struct Material_Version7 : public IMaterial
		{
			virtual SystemLib::return_value read(SystemLib::DataStream& _oIn) override
			{
				for (auto& itTex : vecTextures)
					itTex.read(_oIn);
				_oIn.readMem(&mask, 4);
				_oIn.readMem(&alpha, 4);

				return true;
			};

			virtual SystemLib::return_value write(SystemLib::DataStream& _oOut)
			{
				for (auto& itTex : vecTextures)
					itTex.write(_oOut);

				_oOut.writeMem(&mask, 4);
				_oOut.writeMem(&alpha, 4);

				return true;
			};

		public:
			std::vector<TextureData> vecTextures;
			uint32_t mask = 0;
			uint32_t alpha = 0;
		};

		struct AttachmentPointData
		{
			AttachmentPointData(const std::string& _strBoneName, uint32_t uiID)
			{
				set(_strBoneName, uiID);
			}

			void set(const std::string& _strBoneName, uint32_t uiID, DirectX::XMFLOAT3X4 _3x4matrix =
				DirectX::XMFLOAT3X4(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0)
			)
			{
				memset(this, 0, sizeof(AttachmentPointData));

				// zero pad string
				memset(szAttachmentPointName, 0, 32);

				// copy string, if string longer than 32 char, only copy 32 char.
				memcpy(szAttachmentPointName, _strBoneName.c_str(), _strBoneName.size() > 32 ? 32 : _strBoneName.size());

				// set matrix to "indentity
				o3x4Matrix = DirectX::XMFLOAT3X4(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0);

				// DEBUG CODE
				/*DirectX::XMMATRIX _m = XMLoadFloat3x4(&o3x4Matrix);

				_m =_m* XMMatrixRotationY(XM_PI);
				XMStoreFloat3x4(&o3x4Matrix, _m);*/

				// bone id - corresponds to the bone ids used in the skeletons
				uiBoneId = uiID;
			}

			AttachmentPointData()
			{
				set("", 0);
			}

			static size_t constexpr size()
			{
				return (32 +
					sizeof(DirectX::XMFLOAT3X4) +
					sizeof(uiBoneId));
			}

			/// <summary>
			///  Serializes object, writes to disk
			/// </summary>-
			/// <param name="oOut">ofstream, opened for binary read</param>
			/// <returns></returns>
			bool write(SystemLib::DataStream& _oOut)
			{
				_oOut.writeMem(szAttachmentPointName, 32);   // bone_name
				_oOut.writeMem((char*)&o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));   // 3x4 indentity matricxx
				_oOut.writeMem((char*)&uiBoneId, sizeof(uiBoneId)); // bone id (32 bit)

				return true;
			}

			bool read(SystemLib::DataStream& _oIn)
			{
				_oIn.readMem(szAttachmentPointName, 32);   // bone_name
				_oIn.readMem((char*)&o3x4Matrix, sizeof(DirectX::XMFLOAT3X4));   // 3x4 indentity matricxx
				_oIn.readMem((char*)&uiBoneId, sizeof(uiBoneId)); // bone id (32 bit)

				return true;
			}

			//32 uint8_ts[0 - Padded String] - SupplementarBoneName
			char szAttachmentPointName[32];
			wchar_t wszAttachmentPointName[32]; // for wide char / unicode, not yes supported

			// 48 uint8_ts - ? // Probably position, rotation and other things.
			//char unkownn1[48];

			DirectX::XMFLOAT3X4 o3x4Matrix =
				DirectX::XMFLOAT3X4(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0);

			// 4 uint8_ts[UInt32] - SupplementarBoneID
			uint32_t uiBoneId = 0;// 4 uint8_ts[UInt32] - SupplementarBoneID
		}; // END: RMV2_AttachmentPoint_Data

#pragma region ImporterLIb::RigidModel::FileHeader
		struct FileHeader : public SystemLib::ISerializable
		{
			// works for RMV2 version 6, 7, 8
			virtual const char* getSignatureSZ() { return Signature.szSignature; };
			virtual uint32_t getSignatureDWORD() { return Signature.dwSignature; };
			virtual uint32_t getLODCount() { return Count.wLod; };
			virtual const char* getSkeletonSZ() { return szSkeletonId; };

			constexpr static uint32_t RMV2_SIGNATURE = 0x32564D52;

			union _Signature {
				char szSignature[4];		// Should always be "RMV2"
				uint32_t dwSignature = 0;	// the 32 bit numberical eqauivalent for "RV2" = 0x32564D52
			} Signature = { 0 };

			uint32_t ModelVersion = 0;		// 6 for Rome2/Atilla/Thrones, 7 for WH/WH2, 8 for 3K

			// count is split up into two 16 biLPARAM lParams and the lower 16 bit is used,
			// because in some rare models the upper 16 bits ("G") is filled with some info
			struct _Count
			{
				uint16_t wLod = 0, G = 0;
			} Count = { 0 };

			//uint32_t		LodsCount = 0;		// LOD count
			char szSkeletonId[128] = { 0 };
			wchar_t wszSkeletonId[128] = { 0 };

			size_t size() override
			{
				return sizeof(Signature.dwSignature) + sizeof(Count) + sizeof(szSkeletonId) + sizeof(ModelVersion);
			};

			static size_t _size()
			{
				return sizeof(Signature.dwSignature) + sizeof(Count) + sizeof(szSkeletonId) + sizeof(ModelVersion);
			};

			/// <summary>
			/// Write the File Header
			/// </summary>
			/// <param name="oOut">ofstreanm, opened for binary writing</param>
			/// <returns>bool</returns>
			SystemLib::return_value write(SystemLib::DataStream& oOut) override
			{
				oOut.writeMem(Signature.szSignature, 4); // "RMV2"
				oOut.writeMem(&ModelVersion, 4); // Version Number
				oOut.writeMem(&Count.wLod, 2); // write lod count
				oOut.writeMem(&Count.G, 2);	// upper bits, unknown
				oOut.writeMem(szSkeletonId, 128);	// write the skeleton idstd::string,
				return true;
			}

			SystemLib::return_value read(SystemLib::DataStream& _in) override
			{
				_in.readMem(Signature.szSignature, 4); // "RMV2"
				_in.readMem(&ModelVersion, 4); // Version Number
				_in.readMem(&Count.wLod, 2); // write lod count
				_in.readMem(&Count.G, 2);	// upper bits, unknown
				_in.readMem(szSkeletonId, 128);	// write the skeleton idstd::string,

				if (Signature.dwSignature != RMV2_SIGNATURE)
					return last_error("File is not a valid RMV2 File.");

				return true;
			}
		}; // END File Struct Header;
#pragma endregion END: ImporterLIb::RigidModel::FileHeader

#pragma region SubMesheader

		struct SubMeshHeader : public SystemLib::ISerializable
		{
			SubMeshHeader()
			{
				memset(szShaderName, 0, 255);
				memcpy(szShaderName, "default_dry", 11);

				memset(szGroupName, 0, 32);
				memcpy(szGroupName, "Box_01", 6);
			}

			void setName(const std::string& _strName)
			{
				memset(szGroupName, 0, 32);
				memcpy(szGroupName, _strName.c_str(), _strName.size() > 31 ? 31 : _strName.size());
			}

			RigidMaterial::ETypes RigidMaterialId =
				RigidMaterial::ETypes::default_material;		// Indicates the material of the group, some material make the group chunk larger, which merits further discovery

			// number of bytes from the start of this header to the begining of the next group (starting at the header)
			uint32_t dwGroupSize = 0;	// Size of the entire group (header + attachment blocks + texture blocks + vertices + indices)
										// Absolute offset of next grouop = (sum of preverious group sizes) + current group size + LodHeaders[lod].offset

			uint32_t uiTextureAndAttchmentBlockSize = 0; // The combined size of all attachment blocks and all texture blocks in current group
			uint32_t uiVertextCount = 0;		// Count of vertices in group

			uint32_t uiVertexData_TextAndAttach_BlockTotalSize = 0;
			uint32_t uiIndexCount = 0;		// 	Number of indices

			// offset 24
			// the bounding box of the max extend of the vertices in the following vertex chunk
			float fGroupMinimumX = 0.0f;;
			float fGroupMinimumY = 0.0f;;
			float fGroupMinimumZ = 0.0f;
			float fGroupMaximumX = 1.7f;;
			float fGroupMaximumY = 1.7f;;
			float fGroupMaximumZ = 1.7f;

			// offset 48
			char  szShaderName[255];		// shader name is zero terminated, 256 just convient array size
			wchar_t  szwShaderName[256];	// utf-16 version, used in RMV2 v5 (see hehader)

			// Unknown structure 16 and 20 bytes long have been encountered, 20 for most cases, only terrain meshes seem to be 16, unsure on how to detect size
			// These values change when loading different types of model "weighted model" "weapon", "building"
			// but I dont know what it means exactly yet
			struct _flags
			{
				// not clear on what these bytes mean, hence the test union, they have some relative no vertex type.
				// Though seems not be vertex size, even if that fits for two vertex types

				//uint8_t flags1[4] = { 129, 5, 0, 0 }; // 32512; // 32512  for WH character model / weapons
				//
				//uint8_t flags2[4] = { 73, 0, 78, 0 }; // 32; // 32  for WH character model / weapons
				//	uint32_t flags3 = 51;
				//	uint32_t flags4 = 0;
				//	uint32_t flags5 = 0;

				//char str[5 * 4];

				/*uint32_t flags1;
				uint32_t flags2;
				uint32_t flags3;
				uint32_t flags4;
				uint32_t flags5;*/

				char data[40];

				/*uint8_t flags1[4] = { 0 };
				uint8_t flags2[4] = { 0 };
				uint8_t flags3[4] = { 0 };
				uint8_t flags4[4] = { 0 };
				uint8_t flags5[4] = { 0 };
				*/
			} oFlags;

			//offset 80
			VertexFormat::EVertexType VertexFormatId = VertexFormat::EVertexType::eDefaultFormat;

			char szGroupName[32] = "Box_01";		// First believed to be zero padded 32 char, but is not present at all in certain "obscure" RMV2 "sub formats"
			wchar_t szwGroupName[32];	// utf-16 version of abovestd::string, seems only to be used in RMV2 version 5, (see File header)

			// !!NOT!! part of file format

			char szTextureDirectory[256] = "variantmeshes/_variantmodels/man/tex/";		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory
			wchar_t wszTextureDirecory[256];		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory

			// !!NOT!! part of file format

			// in building models it seems like this structs contains values with this types 8 times
			struct _S2
			{
				_S2()
				{
					memset(this, 0, sizeof(_S2));
				}

				// this type patterns repeats itself, but only when offset "lines up", further investigation.....
				uint32_t dw0;//float f1;
				uint32_t dw1;
				uint32_t dw2;
				uint32_t dw3;
				uint32_t dw4;// float f2;
				uint32_t dw5;
				uint32_t dw6;
				uint32_t dw7;
			};
			_S2 Unknown3_256[8];

			// Value changes also with different model types.
			//struct U
			//{ // Uknown
			//	uint32_tui;
			//};

			struct _ui
			{
				uint16_t value;
			} ui;
			//
			//uint16_t uiUK_2;
			// the new origion
			// the models new pivov point. For weapons etc. Usually/aways? 0 for rigged models.

			DirectX::XMFLOAT3 pivot = { 0,0,0 };

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
			uint32_t uiAttachmentPointCount = 0;	// Count of weapon attachment points blocks
			uint32_t dwTextureCount = 0;			// Count of texture info blocks in group

			uint32_t value1 = 0;	// 0
			uint32_t value2 = 0;	// 0
			uint32_t value3 = 1;	// 1
			uint32_t value4 = 0;	// 0
			uint32_t value5 = 0;	// 0
			uint32_t value6 = 0;	// 0
			uint32_t value7 = 1;	//  1
			uint32_t value8 = 0;	// 0

			uint32_t Unknown140[27] = { 0 };	/// <value>Name accesses the value of the name data member</value>
			//140 uint8_ts - ? // No idea.
			//uint32_t dwTransparent = 0;

			/// <summary>
			/// Write the Group Data HEADER
			/// </summary>
			/// <param name="oOut">ofstreanm, opened for binary writing</param>
			/// <returns>bool</returns>
			SystemLib::return_value write(SystemLib::DataStream& _Out)
			{
				size_t _offHeader1 = _Out.tellp(); // DEBUGGING CODE

				// write ID and group
				_Out.writeMem((char*)&RigidMaterialId, 4);

				size_t off2 = _Out.tellp();
				_Out.writeMem((char*)&dwGroupSize, 4);

				// write block size fields
				size_t off1 = _Out.tellp();
				_Out.writeMem((char*)&uiTextureAndAttchmentBlockSize, 4);

				_Out.writeMem((char*)&uiVertextCount, 4);

				size_t off4 = _Out.tellp();
				_Out.writeMem((char*)&uiVertexData_TextAndAttach_BlockTotalSize, 4);
				_Out.writeMem((char*)&uiIndexCount, 4);

				// write bounding box
				_Out.writeMem((char*)&fGroupMinimumX, 4);
				_Out.writeMem((char*)&fGroupMinimumY, 4);
				_Out.writeMem((char*)&fGroupMinimumZ, 4);

				_Out.writeMem((char*)&fGroupMaximumX, 4);
				_Out.writeMem((char*)&fGroupMaximumY, 4);
				_Out.writeMem((char*)&fGroupMaximumZ, 4);

				size_t debug_size = strlen(szShaderName); // DEBUGGIN CODE

				char null = '\0';
				_Out.writeMem(szShaderName, strlen(szShaderName));
				_Out.writeMem(&null, 1); // write null terminator

				// write group header flags

				/*char a = 'v';
				oOut.writeMem((char*)&a, 5 * 4);*/

				_Out.writeMem((char*)&oFlags, 5 * 4);

				_Out.writeMem((char*)&VertexFormatId, 2);
				_Out.writeMem(szGroupName, 32);
				//oOut.writeMem(&null, 1); // write null terminator

				_Out.writeMem(szTextureDirectory, 256);

				// write unknown block
				int size = sizeof(Unknown3_256);
				_Out.writeMem((char*)&Unknown3_256, 256);

				_Out.writeMem((char*)&ui, 2);

				_Out.writeMem((char*)&pivot.x, 4);
				_Out.writeMem((char*)&pivot.y, 4);
				_Out.writeMem((char*)&pivot.z, 4);

				// write the indentity matrices
				_Out.writeMem((char*)&oTransformation, sizeof(oTransformation));
				_Out.writeMem((char*)&bone_id_building, 4);
				_Out.writeMem((char*)&bone_id_unknown, 4);

				_Out.writeMem((char*)&uiAttachmentPointCount, 4);
				_Out.writeMem((char*)&dwTextureCount, 4);

				_Out.writeMem((char*)&value1, 4);
				_Out.writeMem((char*)&value2, 4);
				_Out.writeMem((char*)&value3, 4);
				_Out.writeMem((char*)&value4, 4);
				_Out.writeMem((char*)&value5, 4);
				_Out.writeMem((char*)&value6, 4);
				_Out.writeMem((char*)&value7, 4);
				_Out.writeMem((char*)&value8, 4);
				_Out.writeMem((char*)&Unknown140, 27 * 4);

				size_t _offHeader2 = _Out.tellp();
				size_t _deltaHeader = _offHeader2 - _offHeader1;

				return true;
			}

			SystemLib::return_value read(SystemLib::DataStream& _Out) override
			{
				size_t _offHeader1 = _Out.tellp(); // DEBUGGING CODE

				// write ID and group
				_Out.readMem(&RigidMaterialId, 4);

				size_t off2 = _Out.tellp();
				_Out.readMem(&dwGroupSize, 4);

				// write block size fields
				size_t off1 = _Out.tellp();
				_Out.readMem(&uiTextureAndAttchmentBlockSize, 4);

				_Out.readMem(&uiVertextCount, 4);

				size_t off4 = _Out.tellp();
				_Out.readMem(&uiVertexData_TextAndAttach_BlockTotalSize, 4);
				_Out.readMem(&uiIndexCount, 4);

				// write bounding box
				_Out.readMem(&fGroupMinimumX, 4);
				_Out.readMem(&fGroupMinimumY, 4);
				_Out.readMem(&fGroupMinimumZ, 4);

				_Out.readMem(&fGroupMaximumX, 4);
				_Out.readMem(&fGroupMaximumY, 4);
				_Out.readMem(&fGroupMaximumZ, 4);

				//size_t debug_size = strlen(szShaderName); // DEBUGGIN CODE

				char null = '\0';
				_Out.readMem(szShaderName, 12);
				//_Out.readMem(&null, 1); // write null terminator

				// write group header flags

				/*char a = 'v';
				oOut.readMem(&a, 5 * 4);*/

				_Out.readMem((char*)&oFlags, 5 * 4);

				_Out.readMem(&VertexFormatId, 2);
				_Out.readMem(szGroupName, 32);
				//oOut.writeMem(&null, 1); // write null terminator

				_Out.readMem(szTextureDirectory, 256);

				// write unknown block
				int size = sizeof(Unknown3_256);
				_Out.readMem(&Unknown3_256, 256);

				_Out.readMem(&ui, 2);

				_Out.readMem(&pivot.x, 4);
				_Out.readMem(&pivot.y, 4);
				_Out.readMem(&pivot.z, 4);

				// write the indentity matrices
				_Out.readMem(&oTransformation, sizeof(oTransformation));
				_Out.readMem(&bone_id_building, 4);
				_Out.readMem(&bone_id_unknown, 4);

				_Out.readMem(&uiAttachmentPointCount, 4);
				_Out.readMem(&dwTextureCount, 4);

				_Out.readMem(&value1, 4);
				_Out.readMem(&value2, 4);
				_Out.readMem(&value3, 4);
				_Out.readMem(&value4, 4);
				_Out.readMem(&value5, 4);
				_Out.readMem(&value6, 4);
				_Out.readMem(&value7, 4);
				_Out.readMem(&value8, 4);
				_Out.readMem(&Unknown140, 27 * 4);

				size_t _offHeader2 = _Out.tellp();
				size_t _deltaHeader = _offHeader2 - _offHeader1;

				return true;
			}

			size_t size() override
			{
				size_t total = 0;
				// write ID and group
				//oOut.writeMem((char*)&uiMaterialId, 4);
				//oOut.writeMem((char*)&uiGroupSize, 4);
				total += 8;

				// write block size fields
				//oOut.writeMem((char*)&uiTextureAndAttchmentBlockSize, 4);
				//oOut.writeMem((char*)&uiVertextCount, 4);
				//oOut.writeMem((char*)&uiVertexDataPlusBlockTotalSize, 4);
				//oOut.writeMem((char*)&uiIndexCount, 4);

				total += 16;

				// write bounding box
				//oOut.writeMem((char*)&fGroupMinimumX, 4);
				//oOut.writeMem((char*)&fGroupMinimumY, 4);
				//oOut.writeMem((char*)&fGroupMinimumZ, 4);
				//oOut.writeMem((char*)&fGroupMaximumX, 4);
				//oOut.writeMem((char*)&fGroupMaximumY, 4);
				//oOut.writeMem((char*)&fGroupMaximumZ, 4);

				total += 24;

				//char null = '\0';
				////oOut.writeMem(szShaderName, strlen(szShaderName) );
				//oOut.writeMem(szShaderName, 32 );
				//oOut.writeMem(&null, 1); // write null terminator

				size_t DEBUG_strlen = strlen(szShaderName); // DEBUGGIN CODE
				total += strlen(szShaderName) + 1;

				// write group header flags
				//oOut.writeMem((char*)&oFlags.flags1, 4);
				//oOut.writeMem((char*)&oFlags.flags2, 4);
				//oOut.writeMem((char*)&oFlags.flags3, 4);
				//oOut.writeMem((char*)&oFlags.flags4, 4);
				//oOut.writeMem((char*)&oFlags.flags4, 4);
				total += 20;

				//oOut.writeMem((char*)&wVertexFormat, 2);
				//oOut.writeMem(szGroupName, 32);
				total += 34;

				//oOut.writeMem(&null, 1); // write null terminator

		//		oOut.writeMem(szTextureDirectory, 258);

				total += 258;

				// write unknown block
				//int size = sizeof(Unknown3_252);
				//oOut.writeMem((char*)&Unknown3_252, 256);

				//oOut.writeMem((char*)&ui, 4);

				total += 256;

				/*oOut.writeMem((char*)&pivot.x, 4);
				oOut.writeMem((char*)&pivot.y, 4);
				oOut.writeMem((char*)&pivot.z, 4);*/

				total += 12;

				// write the indentity matrices
				//oOut.writeMem((char*)&o3x4IndentityMatrices, sizeof(o3x4IndentityMatrices));
				//oOut.writeMem((char*)&n1, 4);
				//oOut.writeMem((char*)&n2, 4);

				total += sizeof(oTransformation) + 4 + 4;

				//oOut.writeMem((char*)&uiAttachmentPointCount, 4);
				//oOut.writeMem((char*)&uiTextureCount, 4);
				total += 8;

				/*oOut.writeMem((char*)& value1, 4);
				oOut.writeMem((char*)& value2, 4);
				oOut.writeMem((char*)& value3, 4);
				oOut.writeMem((char*)& value4, 4);
				oOut.writeMem((char*)& value5, 4);
				oOut.writeMem((char*)& value6, 4);
				oOut.writeMem((char*)& value7, 4);
				oOut.writeMem((char*)& value8, 4);
				oOut.writeMem((char*)&Unknown140, 27*4);*/

				total += 32 + (27 * 4);

				return total;
			}

			void setattachCount(uint32_t count)
			{
				uiAttachmentPointCount = count;
			}

			void setTextureCount(uint32_t count)
			{
				dwTextureCount = count;
			}

			void setVertexCount(uint32_t count, uint32_t vertex_size)
			{
				uiVertextCount = count;
			}

			void setIndexCount(uint32_t count)
			{
				uiIndexCount = count;
			}

			constexpr static uint32_t group_header_size = 940;

			static size_t _size()
			{
				return group_header_size;
			}

			uint32_t calculate_group_size(uint32_t _rmv2_version)
			{
				uint32_t vertex_size = VertexFormat::getSizeFromFormatAndVersion(VertexFormatId, _rmv2_version);

				uint32_t group_header_size = 940;

				uint32_t group_header_size_DEBUG = (uint32_t)size();

				uint32_t alpha_and_mask_size = 8;

				uiTextureAndAttchmentBlockSize = (uint32_t)
					uiAttachmentPointCount * (uint32_t)AttachmentPointData::size() + dwTextureCount * (uint32_t)TextureData::_size() +
					((uint32_t)group_header_size + (uint32_t)alpha_and_mask_size);

				uiVertexData_TextAndAttach_BlockTotalSize =
					(uiVertextCount * vertex_size) + uiTextureAndAttchmentBlockSize;

				dwGroupSize =
					//o.size() +
					uiVertexData_TextAndAttach_BlockTotalSize +
					(uiIndexCount * 2);

				return 0;
			}
		}; // EMD: SubMeshHeader

#pragma endregion : END SubMeshHeader;

		struct RMV2MeshData : SystemLib::ISerializable
		{
			friend struct ISerializable;
			friend struct SubMeshBlock;

		public:
			std::vector<uint8_t> vecVertexDataRAW;
			std::vector<uint16_t> vecIndexData16;

			uint32_t vertex_size = 0;

			template <class VERTEX_TYPE>
			VERTEX_TYPE* getVertexAs(size_t _index)
			{
				return reinterpret_cast<VERTEX_TYPE*>(&vecVertexDataRAW[_index]);
			}

			const void* vertex_data()
			{
				return &vecVertexDataRAW[0];
			};

			size_t vertex_data_size()
			{
				return vecVertexDataRAW.size();
			};

			size_t vertex_count()
			{
				return vecVertexDataRAW.size() / vertex_size;
			};

			const void* index_data16()
			{
				return &vecIndexData16[0];
			};

			size_t index_count()
			{
				return vecIndexData16.size();
			};

			size_t getVertexCount();

		protected:
			SystemLib::return_value read(SystemLib::DataStream& _DataStream,
				size_t vertex_buffer_size,
				size_t index_buffer_size)
			{
				vecVertexDataRAW.clear();
				vecIndexData16.clear();

				vecVertexDataRAW.resize(vertex_buffer_size);
				vecIndexData16.resize(index_buffer_size / 2); // divide by 2 as it is a 16 bit buffer

				_DataStream.readMem(&vecVertexDataRAW[0], vertex_buffer_size);
				_DataStream.readMem(&vecIndexData16[0], index_buffer_size);

				return true;
			};

			SystemLib::return_value write(SystemLib::DataStream& _DataStream,
				size_t vertex_buffer_size,
				size_t index_buffer_size)
			{
				_DataStream.writeMem(&vecVertexDataRAW[0], vertex_buffer_size);
				_DataStream.writeMem(&vecIndexData16[0], index_buffer_size);

				return true;
			};

			virtual SystemLib::return_value read(SystemLib::DataStream& _DataStream) override
			{
				return false;
			}

			virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream) override
			{
				return false;
			}
			//virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream) override;;
		};

		struct AttachmentPointBlock : SystemLib::ISerializable
		{
			SystemLib::return_value read(SystemLib::DataStream& _DataStream, size_t _elements)
			{
				vecData.clear();
				vecData.resize(_elements);

				return this->read(_DataStream);
			}

			virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream) override
			{
				bool bResult = true;
				for (auto& it : vecData)
					if (!it.write(_DataStream))
						bResult = false;

				return bResult;
			};

			std::vector<AttachmentPointData> vecData;

		private:
			virtual SystemLib::return_value read(SystemLib::DataStream& _DataStream)
			{
				for (auto& it : vecData)
					it.read(_DataStream);

				return true;
			};
		};

		struct SubMeshBlock : SystemLib::ISerializable
		{
			size_t calculateVertexSize() // TODO: Finish this, use the RME code
			{
				uint32_t vertex_size = 0;
				uint32_t indice_size = 6;
				float fVertexCount = (float)oHeader.uiVertextCount;
				float fVertexPlusBlockSize = (float)oHeader.uiVertexData_TextAndAttach_BlockTotalSize;
				float fBlockSize = (float)oHeader.uiTextureAndAttchmentBlockSize;
				float fVertexBlockSize = (float)fVertexPlusBlockSize - fBlockSize;
				float fVertexSize = (float)(fVertexPlusBlockSize - fBlockSize) / fVertexCount;

				return (size_t)fVertexSize;
				//float vertex_data_size += (fVertexPlusBlockSize - fBlockSize);
			}

			virtual SystemLib::return_value read(SystemLib::DataStream& _DataStream) {
				auto ret = oHeader.read(_DataStream);

				size_t calc_offset = _DataStream.tellp() + oHeader.dwGroupSize - SubMeshHeader::_size();
				//_DataStream.seek_relative(oHeader.dwGroupSize - SubMeshHeader::_size());

				oAttachmentPoints.read(_DataStream, oHeader.uiAttachmentPointCount);

				oMaterial.vecTextures.resize(oHeader.dwTextureCount);
				oMaterial.read(_DataStream); // TODO: Finish this

				float fvertex_size =
					((float)oHeader.uiVertexData_TextAndAttach_BlockTotalSize -
						(float)oHeader.uiTextureAndAttchmentBlockSize)
					/ oHeader.uiVertextCount;

				oMesh.vertex_size = fvertex_size;

				oMesh.read(
					_DataStream,
					oHeader.uiVertexData_TextAndAttach_BlockTotalSize - oHeader.uiTextureAndAttchmentBlockSize,
					(size_t)(uint32_t)2 * oHeader.uiIndexCount
				); // TODO: Finish this, calculate the vertex buffer size, preferably with using vertex_form::getsize()

				size_t actual_offset = _DataStream.tellp(); // TODO: Use this in integrity check

				if (calc_offset != actual_offset)
					__debugbreak();

				return true;
			};

			virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream) {
				auto ret = oHeader.write(_DataStream);

				size_t calc_offset = _DataStream.tellp() + oHeader.dwGroupSize - SubMeshHeader::_size();
				//_DataStream.seek_relative(oHeader.dwGroupSize - SubMeshHeader::_size());

				oAttachmentPoints.write(_DataStream);
				oMaterial.write(_DataStream);

				float fvertex_size =
					((float)oHeader.uiVertexData_TextAndAttach_BlockTotalSize -
						(float)oHeader.uiTextureAndAttchmentBlockSize)
					/ oHeader.uiVertextCount;

				float fVertexSizeCorrectlyCalculate = fvertex_size - ((size_t)fvertex_size);
				assert(fVertexSizeCorrectlyCalculate == 0.0f);

				oMesh.write(
					_DataStream,
					oHeader.uiVertexData_TextAndAttach_BlockTotalSize - oHeader.uiTextureAndAttchmentBlockSize,
					(size_t)(uint32_t)2 * oHeader.uiIndexCount
				); // TODO: Finish this, calculate the vertex buffer size, preferably with using vertex_form::getsize()

				oMesh.vertex_size = fvertex_size;
				size_t actual_offset = _DataStream.tellp(); // TODO: Use this in integrity check

				if (calc_offset != actual_offset)
					__debugbreak();

				return true;
			};

		public:
			SubMeshHeader oHeader;
			AttachmentPointBlock oAttachmentPoints;
			Material_Version7 oMaterial;
			RMV2MeshData oMesh;
		};

		struct LODData // : SystemLib::ISerializable
		{
			std::vector<SubMeshBlock> vecGroups;
		};

		struct IRigidModelFile : public SystemLib::ISerializable {
			FileHeader oFileHeader;
			std::vector<LODData> vecLodData;
			std::vector<std::shared_ptr<LODHeaderCommon>> spovecLODHeaders;
			std::string strFileName;
		};

		struct RigidModelFileCommon : public IRigidModelFile {
			virtual RMV2MeshData* getMeshData(size_t _lod, size_t _group)
			{
				if (_lod >= oFileHeader.Count.wLod)
					return nullptr;

				if (_lod >= vecLodData.size())
					return nullptr;

				if (_group >= vecLodData[_lod].vecGroups.size())
					return nullptr;

				return &vecLodData[_lod].vecGroups[_group].oMesh;
			}

			virtual SubMeshBlock* getSubMeshBlock(size_t _lod, size_t _group)
			{
				if (_lod >= oFileHeader.Count.wLod)
					return nullptr;

				if (_lod >= vecLodData.size())
					return nullptr;

				if (_group >= vecLodData[_lod].vecGroups.size())
					return nullptr;

				return &vecLodData[_lod].vecGroups[_group];
			}

			virtual const FileHeader* getFileHeader()
			{
				return &oFileHeader;
			}

			virtual SystemLib::return_value read(SystemLib::DataStream& _DataStream)
			{
				if (!oFileHeader.read(_DataStream))
					return false;

				// Read LOD HEADER
				spovecLODHeaders.resize(oFileHeader.Count.wLod);
				for (auto& itLod : spovecLODHeaders)
				{
					itLod = std::shared_ptr<LODHeaderCommon>(new LODHeaderCommon);

					itLod->read(_DataStream);
				}

				// Read LOD DATA
				vecLodData.resize(oFileHeader.Count.wLod);

				return readCommon(_DataStream);
			}

			virtual SystemLib::return_value readCommon(SystemLib::DataStream& _DataStream)
			{
				for (size_t lod_index = 0; lod_index < oFileHeader.Count.wLod; lod_index++)
				{
					// allocate space for groups (submeshes) in this LOD
					vecLodData[lod_index].vecGroups.resize(spovecLODHeaders[lod_index]->dwGroupCount);

					_DataStream.seek_absolute(spovecLODHeaders[lod_index]->dwStartOffset);
					for (size_t group_index = 0; group_index < spovecLODHeaders[lod_index]->dwGroupCount; group_index++)
					{
						vecLodData[lod_index].vecGroups[group_index].read(_DataStream);

						int debug_break_1 = 1;
					}
				}

				return true;
			};

			virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream)
			{
				if (!oFileHeader.write(_DataStream))
					return false;

				spovecLODHeaders.resize(oFileHeader.Count.wLod);
				for (auto& itLod : spovecLODHeaders)
				{
					//itLod = std::shared_ptr<LODHeaderCommon>(new LODHeaderV7);

					itLod->write(_DataStream);
				}

				return writeCommon(_DataStream);
			}

			virtual SystemLib::return_value writeCommon(SystemLib::DataStream& _DataStream)
			{
				for (size_t lod_index = 0; lod_index < oFileHeader.Count.wLod; lod_index++)
				{
					// allocate space for groups (submeshes) in this LOD
					vecLodData[lod_index].vecGroups.resize(spovecLODHeaders[lod_index]->dwGroupCount);

					if (spovecLODHeaders[lod_index]->dwStartOffset != _DataStream.tellp())
						__debugbreak();
					//ataStream.seek_absolute(spovecLODHeaders[lod_index]->dwStartOffset);
					for (size_t group_index = 0; group_index < spovecLODHeaders[lod_index]->dwGroupCount; group_index++)
					{
						vecLodData[lod_index].vecGroups[group_index].write(_DataStream);

						int debug_break_1 = 1;
					}
				}

				return true;
			}
		};

		struct RigidModelFile_Version7 : public RigidModelFileCommon
		{
			virtual SystemLib::return_value read(SystemLib::DataStream& _DataStream) override
			{
				if (!oFileHeader.read(_DataStream))
					return false;

				spovecLODHeaders.resize(oFileHeader.Count.wLod);
				for (auto& itLod : spovecLODHeaders)
				{
					itLod = std::shared_ptr<LODHeaderCommon>(new LODHeaderV7);

					itLod->read(_DataStream);
				}

				//// Read LOD HEADER
				//spovecLODHeader.resize(oFileHeader.Count.wLod, nullptr);
				//for (size_t i = 0; i < spovecLODHeader.size(); i++)
				//{
				//	spovecLODHeader[i] = std::shared_ptr<LODHeaderCommon>(new LODHeaderV7);
				//	spovecLODHeader[i]->read(_DataStream);
				//}

				// Read LOD DATA
				vecLodData.resize(oFileHeader.Count.wLod);

				return readCommon(_DataStream);
			}

			virtual SystemLib::return_value write(SystemLib::DataStream& _DataStream) override
			{
				if (!oFileHeader.write(_DataStream))
					return false;

				spovecLODHeaders.resize(oFileHeader.Count.wLod);
				for (auto& itLod : spovecLODHeaders)
				{
					//itLod = std::shared_ptr<LODHeaderCommon>(new LODHeaderV7);

					itLod->write(_DataStream);
				}

				return writeCommon(_DataStream);
			}
		};

		SystemLib::return_value createRidigModFile(SystemLib::DataStream& _DataStream, RigidModelFileCommon** ppDest);
		SystemLib::return_value createRidigModFile(SystemLib::DataStream&& _DataStream, RigidModelFileCommon** ppDest);
	}
}