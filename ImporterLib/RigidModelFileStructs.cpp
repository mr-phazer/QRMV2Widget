#include "RigidModelFileStructs.h"

std::string ImporterLib::RigidModel::RigidMaterial::getStringFromId(ETypes ID)
{
	switch (ID)
	{
	case bow_wave:										return "bow_wave";
	case non_renderable:									return "non_renderable";
	case texture_combo_vertex_wind:										return "texture_combo_vertex_wind";
	case texture_combo:										return "texture_combo";
	case decal_waterfall:									return "decal_waterfall";
	case standard_simple:									return "standard_simple";
	case campaign_trees:									return "campaign_trees";
	case point_light:									return "point_light";
	case static_point_light:									return "static_point_light";
	case custom_terrain:									return "custom_terrain";
	case weighted_cloth:									return "weighted_cloth";
	case cloth:										return "cloth";
	case collision:										return "collision";
	case collision_shape:									return "collision_shape";
	case tiled_dirtmap:										return "tiled_dirtmap";
	case ship_ambientmap:									return "ship_ambientmap";
	case weighted:										return "weighted";
	case projected_decal:									return "projected_decal";
	case default_material:						       			return "default_material";
	case grass:										return "grass";
	case weighted_skin:										return "weighted_skin";
	case decal:										return "decal";
	case decal_dirtmap:										return "decal_dirtmap";
	case dirtmap:									return "dirtmap";
	case tree:										return "tree";
	case tree_leaf:										return "tree_leaf";
	case weighted_decal:									return "weighted_decal";
	case weighted_decal_dirtmap:									return "weighted_decal_dirtmap";
	case weighted_dirtmap:										return "weighted_dirtmap";
	case weighted_skin_decal:									return "weighted_skin_decal";
	case weighted_skin_decal_dirtmap:									return "weighted_skin_decal_dirtmap";
	case weighted_skin_dirtmap:										return "weighted_skin_dirtmap";
	case water:										return "water";
	case unlit:										return "unlit";
	case weighted_unlit:									return "weighted_unlit";
	case terrain_blend:										return "terrain_blend";
	case projected_decal_v2:									return "projected_decal_v2";
	case ignore:									return "ignore";
	case tree_billboard_material:									return "tree_billboard_material";
	case water_displace_volume:										return "water_displace_volume";
	case rope:										return "rope";
	case campaign_vegetation:									return "campaign_vegetation";
	case projected_decal_v3:									return "projected_decal_v3";
	case weighted_texture_blend:									return "weighted_texture_blend";
	case projected_decal_v4:									return "projected_decal_v4";
	case global_terrain:									return "global_terrain";
	case decal_overlay:										return "decal_overlay";
	case alpha_blend:										return "alpha_blend";

	case unknown:
	default:
		return "uknown material!";
	}
}

const char* TextureType::getStringFromId(uint32_t uiValue)
{
#define S(value, string) case value:	return string;

	switch (uiValue)
	{
	case values1::uiDiffuse:			return "Diffuse";
	case values1::uiSpecular:		return "Specular";

	case values1::uiNormal:			return "Normal Map";

	case values1::uiGlossMap:		return "Gloss Map";
	case values1::uiAmbientOcclusion: return "Ambient Acclusion";

	case values1::uiDecalDirtmap:	return "Decal Dirt Map";

	case values1::uiDecalDirtmask:	return "Decal Dirt Mask";

	case values1::uiDecalMask:		return "Decal Mask";

	case values1::uiDiffuseDamage:	return "Diffuse Damage";

	case values1::uiDirtAlphaMask:	return "Dirt Alpha Mask";

	case values1::uiMask:			return "Mask";

	case values1::uiSkinMask:		return "Skin Mask";

	case values1::uiTilingDirtUV2:	return "Tiling Dirt UV2";

		// Version 8, 3K
	case values1::uiBaseColor:		return "Base Colour";
	case values1::uiMaterialMap:		return "Material Map";

	case values1::uiEmpty:			return "...";

	default:
		return "[unknown type]";
	}
}

TextureType::EValues TextureType::getIdFromString(const std::string& _str½)
{
	//std::string str = tools::lower(_str);

	//if (str == "diffuse")
	//	return TextureType::EValues::uiDiffuse;

	//if (str == "normal")
	//	return TextureType::EValues::uiNormal;

	//if (str == "specular")
	//	return TextureType::EValues::uiSpecular;

	//if (str == "gloss_map")
	//	return TextureType::EValues::uiGlossMap;

	//if (str == "mask")
	//	return TextureType::EValues::uiMask;

	//if (str == "material_map")
	//	return TextureType::EValues::uiMaterialMap;

	//if (str == "base_colour")
	//	return TextureType::EValues::uiBaseColor;

	return TextureType::EValues::eEmpty;
};

SystemLib::return_value ImporterLib::RigidModel::createRidigModFile(SystemLib::DataStream& _DataStream, ImporterLib::RigidModel::RigidModelFileCommon** ppDest)
{
	ImporterLib::RigidModel::FileHeader oHeader;

	oHeader.read(_DataStream);
	_DataStream.seek_absolute(0);

	switch (oHeader.ModelVersion)
	{
	case 6:
		*ppDest = new  ImporterLib::RigidModel::RigidModelFileCommon();
		//	(*ppDest)->strFileName = _DataStream.getFileName();
		return (*ppDest)->read(_DataStream);

	case 7:
	case 8:
		*ppDest = new  ImporterLib::RigidModel::RigidModelFile_Version7();
		//	(*ppDest)->strFileName = tools:: _DataStream.m_wstrDiskFileName;
		return (*ppDest)->read(_DataStream);

	default:
		return { -1, "RMV2 version: " + std::to_string(oHeader.ModelVersion) + ", is not yet supported" };
	}
}

SystemLib::return_value ImporterLib::RigidModel::createRidigModFile(SystemLib::DataStream&& _DataStream, RigidModelFileCommon** ppDest)
{
	SystemLib::DataStream tempStream = _DataStream;
	return ImporterLib::RigidModel::createRidigModFile(tempStream, ppDest);
}