#include "RigidModelV2_Validator.h"
#include "..\QtTestRMV2\QConsole.h"

bool RigidModelV2::RMV2_Validator::validateFileHeader(const Common::FileHeader* _poFileHeader)
{
#ifdef _DEBUG

	char szTempRMV2Signature[5] = { 0 };

	memcpy(szTempRMV2Signature, _poFileHeader->oFileInfoHeader.Signature.szSignature, 4);

	_log_action("File Signature: " + std::string(szTempRMV2Signature));
	_log_action("File LODs: " + std::to_string(_poFileHeader->dwLodCount));
	_log_action("File Skeleton: " + std::string(_poFileHeader->szSkeletonId));

#endif // DEBUG

	if (_poFileHeader->oFileInfoHeader.Signature.dwSignature != RMV2_SIGNATURE)
	{
		strLastErrorString = ErrorStrings::NotValidRMV2File;
		return false;
	}

	if (_poFileHeader->dwLodCount > 100)
	{
		strLastErrorString = ErrorStrings::InvalidLOdCount + std::to_string(_poFileHeader->dwLodCount);
		return false;
	}

	strLastErrorString = ErrorStrings::Sucess;
	return true;
}

bool RigidModelV2::RMV2_Validator::validateLodHeader(const Common::LodHeaderElementCommon* _LodX)
{
	_log_action("SubMesh Count: " + std::to_string(_LodX->dwGroupCount));
	_log_action("Lod Range: " + std::to_string(_LodX->fVisibilityDistance));

	if (isnan(_LodX->fVisibilityDistance))
	{
		strLastErrorString = "Lod range = invalid float (NaN) .";
		return false;
	}

	strLastErrorString = ErrorStrings::Sucess;

	return true;
}

bool RigidModelV2::RMV2_Validator::validateSubMeshPreHeader(const RigidModelV2::Common::GroupPreHeader* _poGroupPreHeader)
{
	_log_action("SubGroup Material: " + getStringFrom_RigidMaterialId(_poGroupPreHeader->RigidMaterialId));
	_log_action("Subgroup Material (numerical value): " + std::to_string(_poGroupPreHeader->RigidMaterialId));

	if ((uint32_t)_poGroupPreHeader->RigidMaterialId > 200)
	{
		_log_action_error("Invalid material id");
		strLastErrorString = "Invalid material id";
		return false;
	}

	if (_poGroupPreHeader->dwVertexCount > 0xFFFF)
	{
		strLastErrorString = "Invalid vertex count value: " + _poGroupPreHeader->dwVertexCount;
		_log_action_error(strLastErrorString);

		return false;
	}

	return true;
}