#pragma once


#include "RigidModelFileStructs.h"

using namespace std;

namespace ImporterLib
{
	namespace RigidModel
	{
		class RigidModelFileHeader {};
		class RigidModelLodHeader {};

		struct RigidModelTextureItem
		{
			// TODO: check the order of these fields
			uint32_t type;
			std::string strPath;
		};

		struct RigidModelMaterial
		{
			std::vector<RigidModelTextureItem> vecTextures;

			// TODO: check the order of these fields
			uint32_t mask = 0;
			uint32_t alpha = 0;
		};



		struct RigidModelGroupHeader
		{

		};

		struct RigidModelSubMesh
		{
			SubMeshHeader oHeader;
			std::vector<AttachmentPointData> vecAttachMentPoints;
			RigidModelMaterial Material;
			std::vector<uint8_t> vecVertexDataRAW;
			std::vector<uint16_t> vecIndexData16;
		};

		struct RigidModelLodData
		{ 
			std::vector<RigidModelSubMesh> vecSubMeshes;
		};

		//struct RigidModelFile
		//{
		//	//RigidModelSubMesh* GetSubMesh(size_t lod, size_t group);


		//	RigidModelFileHeader oFileHeader;
		//	std::vector<RigidModelLodHeader> vecLodHeaders;
		//	std::vector<RigidModelLodData> vecLodData;

		//};


		class IRigidModelImporter
		{
		public:
			virtual bool createNew(SystemLib::DataStream& _input, IRigidModelFile** ppRigidModelFile)
			{
				spoPointer.reset();
				RigidModel::FileHeader oFileHeader;

				oFileHeader.read(_input);

				switch (oFileHeader.ModelVersion)
				{
				case 6: 					
						
				break;

				case 7: break;
//					*ppRigidModelFile = new RigidModel::RigidModelFile_Version7;

				case 8: break;

				}
			}



		public:

			/*ImportLib::RigideModelHeader oFileHeader;
			vector<RigidModelLODHeader> vecLodHeader;
			vector < vector<RigidModelGroup> oGroups;*/

		private:
			std::unique_ptr<IRigidModelFile> spoPointer;

		}; // END: class IRigidModelImporter;


		extern bool createRigidModelFileImporter(SystemLib::DataStream& _oIn, IRigidModelImporter** _ppFile);



		extern std::shared_ptr<IRigidModelImporter> GetRigidModelFile(SystemLib::DataStream& _oInr);
	}  // END: namespace RigidModel
} 