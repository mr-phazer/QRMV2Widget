#pragma once

#include "..\SystemLib\data_stream.h"

#include "RigidModelV2_Data_Structs.h"

#include <string>
#include "..\QtTestRMV2\QConsole.h"

namespace RigidModelV2

{
	class File_Exporter_Common
	{
	public:
		//static std::shared_ptr<RigidModelV2::File_Exporter_Common> create(void* pSrcMem, size_t _size_in_bytes);
		static std::shared_ptr<RigidModelV2::File_Exporter_Common> create();

		size_t write_file(const std::wstring& _strPath);

		std::shared_ptr <Common::CommonFile> getFile();
		bool setFile(std::shared_ptr <Common::CommonFile> m_spoFile);

	private:
		File_Exporter_Common() {};
		bool write();
		/************************************************************************************
		File Headers
		*************************************************************************************/
		//bool create()

		bool writeFileHeader_V5();
		bool writeFileHeader_V5_V6_V7_V8();

		/************************************************************************************
			LOD Headers
		*************************************************************************************/
		bool writeLodHeaderBlock_V5_V6();
		bool writeLodHeaderBlock_V7_V8();

		/**********************************************************************************-----**
			LODs + Sub Meshes
		*************************************************************************************/
		bool writeLods();

		// BEGIN: group write methods

		bool writeGroupBlock(size_t _lod, size_t _group);

		bool writeGroupHeader(size_t _lod, size_t _group);

		bool writeGroupPreHeader(size_t _lod, size_t _group);

		bool writeGroupHeaderDefaultWeighted(size_t _lod, size_t _group);

		bool writeAttachmentPointBlock_V6_V7_V8(size_t _lod, size_t _group);
		bool writeAttachmentPointBlock_V5(size_t _lod, size_t _group);

		bool writeTextureBlock_V6_V7_V8(size_t _lod, size_t _group);
		bool writeTextureBlock_V5(size_t _lod, size_t _group);

		bool writeVertexBlock_Raw(size_t _lod, size_t _group);

		bool writeVertices_Static(size_t _lod, size_t _group);
		bool writeVertices_Weighted(size_t _lod, size_t _group);
		bool writeVertices_Cinematic(size_t _lod, size_t _group);

		bool processVertices_To_Static_V5_V6_V7(size_t _lod, size_t _group);
		bool processVertices_To_Weighted_V5_V6_V7(size_t _lod, size_t _group);
		bool processVertices_To_Cinematic_V5_V6_V7(size_t _lod, size_t _group);

		bool processVertices_To_Static_V8(size_t _lod, size_t _group);
		bool processVertices_To_Weighted_V8(size_t _lod, size_t _group);
		bool processVertices_To_Cinematic_V8(size_t _lod, size_t _group);

		bool writeIndices(size_t _lod, size_t _group);
	public:
		std::shared_ptr<SystemLib::DataStream> m_spoStream;
		std::shared_ptr <Common::CommonFile> m_spoFile;
	};
}