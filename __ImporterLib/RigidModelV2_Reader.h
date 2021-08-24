#pragma once

#include "..\SystemLib\data_stream.h"

#include "RigidModelV2_Data_Structs.h"

#include <string>

namespace RigidModelV2
{
	// flags (in 32 bit "alpha" field)
	const uint32_t SKIN_FALG = 1 << 30;

	// interfaces
	/*class IFile_Header_Reader
	{
		virtual bool read(RigidModelV2::FileHeader_V6_V7_V8*, SystemLib::DataStream&) = 0;
	};*/

	class File_Importer_Common
	{
		// can only be called by the class itself
		bool ErrorString(const std::string& _error);
	public:		

		static std::shared_ptr<RigidModelV2::File_Importer_Common> create(void* pSrcMem, size_t _size_in_bytes);
		static std::shared_ptr<RigidModelV2::File_Importer_Common> create(const std::wstring& _strPath);

		std::shared_ptr <Common::CommonFile> getFile();

		bool isValid() {
			return m_bIsValid;
		};

	private:
		bool m_bIsValid = false;
		//File_Importer_Common() {};
		bool read();

		bool checkEntireFile();

		bool readFileHeaderAndLodBlocks();

		/************************************************************************************
			File Headers
		*************************************************************************************/
		bool readFileInfoHeader_V5_V6_V7_V8();
		bool readFileHeader_V5();
		bool readFileHeader_V6_V7_V8();

		/************************************************************************************
			LOD Headers
		*************************************************************************************/
		bool readLodHeaderBlock_V5_V6();
		bool readLodHeaderBlock_V7_V8();

		/************************************************************************************
			LODs + Sub Meshes
		*************************************************************************************/
		bool readLods();

		// BEGIN: group read methods

		bool readMeshBlock(size_t _lod, size_t _group);

		bool readMeshHeader(size_t _lod, size_t _group);

		bool readMeshPreHeader(size_t _lod, size_t _group);
		bool readMeshHeader(ERigidMaterial _eRigidMaterial, size_t _lod, size_t _group);

		bool readMeshHeader_Default_Weighted_v5(size_t _lod, size_t _group);
		bool readMeshHeader_Default_Weighted_v6_v7_v8(size_t _lod, size_t _group);

		bool readMeshHeader_Weighted_Texture_Blend(size_t _lod, size_t _group);
		bool readMeshHeader_AlphaBlend(size_t _lod, size_t _group);
		bool readMeshHeader_Unknown1(size_t _lod, size_t _group);
		bool readMeshHeader_CustomTerrain(size_t _lod, size_t _group);

		bool readAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group);
		bool readAttachmentPointBlock_V5(size_t _lod, size_t _group);

		bool readTextureBlock_V6_V7_V8(size_t _lod, size_t _group);
		bool readTextureBlock_V5(size_t _lod, size_t _group);

		bool readVertexBlock_STANDARD_Raw(size_t _lod, size_t _group);

		//bool readVertices_Static(size_t _lod, size_t _group);
		//bool readVertices_Weighted(size_t _lod, size_t _group);
		//bool readVertices_Cinematic(size_t _lod, size_t _group);

		//bool processVertices_As_Static_V5_V6_V7(size_t _lod, size_t _group);
		//bool processVertices_As_Weighted_V5_V6_V7(size_t _lod, size_t _group);
		//bool processVertices_As_Cinematic_V5_V6_V7(size_t _lod, size_t _group);

		//bool processVertices_As_Static_V8(size_t _lod, size_t _group);
		//bool processVertices_As_Weighted_V8(size_t _lod, size_t _group);
		//bool processVertices_As_Cinematic_V8(size_t _lod, size_t _group);

		bool readIndices(size_t _lod, size_t _group);
		// END: group read methods

		std::shared_ptr<SystemLib::DataStream> m_spoStream;

		std::shared_ptr <Common::CommonFile> m_spoData;
	};

	static constexpr size_t _mesh_header_default_weighted_v6_v7_v8_length = 940;
	static constexpr size_t _mesh_header_default_weighted_v5_length = 1468 + 48;
	//class File_Exporter_Common
	//{
	//	/************************************************************************************
	//	File Headers
	//	*************************************************************************************/
	//	//bool create()

	//	bool readFileHeader_V5();
	//	bool readFileHeader_V5_V6_V7_V8();

	//	/************************************************************************************
	//		LOD Headers
	//	*************************************************************************************/
	//	bool writeLodHeaderBlock_V5_V6();
	//	bool writeLodHeaderBlock_V7_V8();

	//	/************************************************************************************
	//		LODs + Sub Meshes
	//	*************************************************************************************/
	//	bool writeLods();

	//	// BEGIN: group write methods

	//	bool writeGroupBlock(size_t _lod, size_t _group);

	//	bool writeGroupHeader(size_t _lod, size_t _group);

	//	bool writeGroupPreHeader(size_t _lod, size_t _group);

	//	bool writeGroupHeaderDefaultWeighted(size_t _lod, size_t _group);

	//	bool writeAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group);
	//	bool writeAttachmentPointBlock_V5(size_t _lod, size_t _group);

	//	bool writeTextureBlock_V6_V7_V8(size_t _lod, size_t _group);
	//	bool writeTextureBlock_V5(size_t _lod, size_t _group);

	//	bool writeVertexBlock_Raw(size_t _lod, size_t _group);

	//	bool writeVertices_Static(size_t _lod, size_t _group);
	//	bool writeVertices_Weighted(size_t _lod, size_t _group);
	//	bool writeVertices_Cinematic(size_t _lod, size_t _group);

	//	bool processVertices_To_Static_V5_V6_V7(size_t _lod, size_t _group);
	//	bool processVertices_To_Weighted_V5_V6_V7(size_t _lod, size_t _group);
	//	bool processVertices_To_Cinematic_V5_V6_V7(size_t _lod, size_t _group);

	//	bool processVertices_To_Static_V8(size_t _lod, size_t _group);
	//	bool processVertices_To_Weighted_V8(size_t _lod, size_t _group);
	//	bool processVertices_To_Cinematic_V8(size_t _lod, size_t _group);

	//	bool writeIndices(size_t _lod, size_t _group);

	//	std::shared_ptr<SystemLib::DataStream> m_spoStream;

	//	std::shared_ptr <Common::CommonFile> m_spoData;
	//};

	class File_Processor_Commmon
	{
		static bool cleanupMesh(Common::MeshGeometryData* _poMeshBlock);
		static bool reduceMesh(
			Common::MeshGeometryData* _poMeshBlock,
			float _ratio = 1.0,
			bool _bUseBrute = false,
			float _error = 0.003);
	};
};