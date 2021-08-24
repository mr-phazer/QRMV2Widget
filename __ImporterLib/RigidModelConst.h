#pragma once

#include <string>

#pragma warning( disable : 26812)

struct TextureType
{

	struct values1 {
		static constexpr uint32_t	uiDiffuse = 0;
		static constexpr uint32_t	uiNormal = 1;
		static constexpr uint32_t	uiMask = 3;
		static constexpr uint32_t	uiAmbientOcclusion = 5;
		static constexpr uint32_t	uiTilingDirtUV2 = 7;
		static constexpr uint32_t	uiDirtAlphaMask = 8;
		static constexpr uint32_t	uiSkinMask = 10;
		static constexpr uint32_t	uiSpecular = 11;
		static constexpr uint32_t	uiGlossMap = 12;
		static constexpr uint32_t	uiDecalDirtmap = 13;
		static constexpr uint32_t	uiDecalDirtmask = 14;
		static constexpr uint32_t	uiDecalMask = 15;
		static constexpr uint32_t	uiDiffuseDamage = 17;

		// for model verion 8
		static constexpr uint32_t	uiBaseColor = 27;
		static constexpr uint32_t	uiMaterialMap = 29;

		// empty
		static constexpr uint32_t	uiEmpty = 200;
	};

	enum  EValues  : uint32_t {
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
		eEmpty = 200
	};

	TextureType()
	{

	}
 



	//	using _value::operator=;

public:
	static const char* getString(TextureType::EValues uiValue);
	static const char* getStringFromId(uint32_t uiValue);
	static TextureType::EValues getIdFromString(const std::string&  _str);

};




struct VertexFormat
{
	enum EVertexType : uint16_t
	{
		eDefaultFormat = 0,
	collision_format = 1,
	eWeightedFormat = 3,
	eCinematicFormat = 4,
	grass_format = 5
	};

	static constexpr uint32_t getSizeFromFormatAndVersion(EVertexType _eType, uint32_t _version)
	{

		if (_version == 8)
		{
			switch (_eType)
			{
			case eDefaultFormat: return 32;
			case collision_format: return 0;
			case eWeightedFormat: return 28;
			case eCinematicFormat: return 36;
			case grass_format: return 0;
			}
		}
		switch (_eType)
		{
		case eDefaultFormat: return 32;
		case collision_format: return 0;
		case eWeightedFormat: return 28;
		case eCinematicFormat: return 32;
		case grass_format: return 0;
		}

	}

};



