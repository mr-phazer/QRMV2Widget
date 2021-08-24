//#include "RigidModelV2_Validator.h"
#include "..\QtTestRMV2\QConsole.h"

#include "RigidModelV2_Data_Structs.h"
bool RigidModelV2::Common::Validator::validateFileHeader(const Common::FileHeader* _poFileHeader)
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

bool RigidModelV2::Common::Validator::validateLodHeader(const Common::LodHeaderElementCommon* _LodX)
{
	_log_action("SubMesh Count: " + std::to_string(_LodX->dwMeshCount));
	_log_action("Lod Range: " + std::to_string(_LodX->fVisibilityDistance));

	if (isnan(_LodX->fVisibilityDistance))
	{
		strLastErrorString = "Lod range = invalid float (NaN) .";
		return false;
	}

	strLastErrorString = ErrorStrings::Sucess;

	return true;
}

std::string RigidModelV2::Common::Validator::validateMeshPreHeader(const RigidModelV2::Common::MeshPreHeader* _poGroupPreHeader)
{
	/*return true;
	_log_action("SubGroup Material: " + getStringFrom_RigidMaterialId(_poGroupPreHeader->RigidMaterialId));
	_log_action("Subgroup Material (numerical value): " + std::to_string(_poGroupPreHeader->RigidMaterialId));*/

	if (!isRigidMaterialValid(_poGroupPreHeader->RigidMaterialId) && _poGroupPreHeader->dwVertexCount > 0xFFFF)
	{
		return "Error:"
			"\r\n - Unknown Rigid_Material Id."
			"\r\n - Invalid vertex count value: " + std::to_string(_poGroupPreHeader->dwVertexCount) + " max is: " + std::to_string(0xFFFF) +
			"\r\nFile possibly corrupted";
	}

	if (!isRigidMaterialValid(_poGroupPreHeader->RigidMaterialId))
	{
		return "Unknown Rigid_Material Id: " + getStringFrom_RigidMaterialId(_poGroupPreHeader->RigidMaterialId);
	}

	if (_poGroupPreHeader->dwVertexCount > 0xFFFF)
	{
		return "Invalid vertex count value: " + std::to_string(_poGroupPreHeader->dwVertexCount) + " max is: " + std::to_string(0xFFFF);
	}

	return "";
}