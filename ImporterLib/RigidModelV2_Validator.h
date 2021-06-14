#pragma once

#include <map>

#include "RigidModelV2_Data_Structs.h"
#include "string_constants.h"

namespace RigidModelV2
{
	class RMV2_Validator
	{
	public:
		bool validateFileHeader(const Common::FileHeader* _poFileHeader);

		bool validateLodHeader(const Common::LodHeaderElementCommon* _poFileHeader);

		bool validateSubMeshPreHeader(const RigidModelV2::Common::GroupPreHeader* _poFileHeader);

		//bool validateSubMeshHeader(const RigidModelV2::Common::GroupHeader_Default_Weighted* _poFileHeader);

		std::string strLastErrorString = "not set";

		std::map<ERigidMaterial, bool> mapMaterialsLoaded;
	};
}
