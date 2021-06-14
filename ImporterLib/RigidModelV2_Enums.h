#pragma once

#include <cstdint>
#include <map>
#include <string>

enum EVertexFormat : uint16_t
{
	eDefaultFormat = 0,
	collision_format = 1,
	eWeightedFormat = 3,
	eCinematicFormat = 4,
	grass_format = 5,

	eERROR_NOT_SET = 1000
};

enum  ETextureType : uint32_t {
	eDiffuse = 0,
	eNormal = 1,
	eMask = 3,
	eAmbientOcclusion = 5,
	eTilingDirtUV2 = 7,
	eDirtAlphaMask = 8,
	eSkinMask = 10,
	eSpecular = 11,
	eGlossMap = 12,
	eDecalDirtmap = 13,
	eDecalDirtmask = 14,
	eDecalMask = 15,
	eDiffuseDamage = 17,

	// for model verion 8
	eBaseColor = 27,
	eMaterialMap = 29,

	// empty
	eTEX_ERROR_NOT_SET = 200,
	eEmpty = 400
};

enum  ERigidMaterial : uint32_t
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
	
	unknown1 = 101,

	eMAT_ERROR_NOT_SET = 20000
};

struct MaterialIdToHeaderSize
{
private:
	std::map<ERigidMaterial, size_t> m_map =
	{
		{ERigidMaterial::weighted, 940},
		{ERigidMaterial::weighted_dirtmap, 940}
	};
};

static inline std::string getStringFrom_RigidMaterialId(ERigidMaterial ID)
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
	case ERigidMaterial::ignore:									return "ignore";
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

		//case ERigidMaterial::unknown:
	default:
		return "uknown material!";
	}
}
