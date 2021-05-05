#pragma once

#include <string>
#include "..\SystemLib\data_stream.h"
#include "..\SystemLib\ISerializeabe.h"


#include <directxpackedvector.h>
#include <DirectXMath.h>


//
//namespace TWAnim
//{
//	/*class IAnimExporter;
//	class AnimImporterCommon;
//	class AnimImporterVersion7;
//	class AnimImporterVersion5;*/
//
//	int i = 1;
//
//};




namespace TWAnim
{
	struct BoneData
	{
		int32_t id = 0;
		int32_t parent_id = 0;
		std::string strName = "empty";
	};



	struct AnimHeaderVersion7
	{
		uint32_t dwVersion = 7;
		uint32_t dwUnknown = 1;
		float fFrameRate = 20.f;

		std::string strSkeletonId = "no skeleton set";

		uint32_t dwBoneCount = 0;;

		/*uint32_tuk1;
		uint32_tuk2;
		uint32_tuk3;
		uint32_tuk4;*/

		//6 uk3_4[2];
		struct  _v7_sub_header
		{
			uint32_t flag_count = 0;;
			std::string strText = "";
		} v7_sub_header;


		float fEndTime = 0.1f;
	};

	/*struct AnimHeaderAndBoneTableVersion7
	{
		AnimHeaderVersion7  oHeader;
		RenderLib::Skeleton m_vecBoneData;
	};*/




	struct FrameHeader
	{
		uint32_t dwTranslationsCount = 0;
		uint32_t dwRotationCount = 0;
	};

	struct Frame
	{
		std::vector <DirectX::XMFLOAT3> vecTranslations;
		std::vector <DirectX::XMFLOAT4> vecQuaternions;
		//vector <DirectX::PackedVector::XMSHORT4> vecRotations;
	};

} // END: namespace TWAnim;

struct string_with_16_bit_length : public SystemLib::ISerializable
{
	//friend class TWAnim::IAnimExporter;
	//friend class TWAnim::AnimImporterCommon;
	//friend class TWAnim::AnimImporterVersion7;
	//friend class TWAnim::AnimImporterVersion5;

public:
	//virtual bool read(SystemLib::DataStream& _in) override;
	//virtual bool write(SystemLib::DataStream& _in) override;

	virtual SystemLib::return_value read(SystemLib::DataStream& _in) override;
	virtual SystemLib::return_value write(SystemLib::DataStream& _in) override;
	//virtual SystemLib::return_value write(SystemLib::DataStream& _in, std::string _strBoneName) override;

public:
	std::string strText;
};

