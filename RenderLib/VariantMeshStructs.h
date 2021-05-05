#pragma once

#include "..\ImporterLib\RigidModelFileStructs.h"
#include "..\ImporterLib\IRigidModelmporter.h"

namespace RenderLib
{
	struct MaterialInfo
	{
		bool parseXMLMaterialFile_Internal();

		vector<ImporterLib::RigidModel::RigidModelTextureItem> vecTextureItems;
		wstring strPathToXmlMatrial = L"";

		struct _ShaderInfo
		{
			wstring name = L"";
			wstring shader = L"";
		} oShaderInfo;

		bool bAlpha = false;
	};

	struct VariantMeshInfo
	{
		bool parseWSModelInternal();

		//bool bOnDisk = false;
		std::string strModelPath = ""; // TODO: change to wstring, for disk loading
		vector<vector<MaterialInfo>> vecLodMaterials;
		int32_t attch_bone = -1;
		string strAttachBoneName = "";
	};
}
