#pragma once

#include <vector>

#include "..\ImporterLib\anim_structs.h"

using namespace std;
namespace TWAnim
{
	class AnimFile
	{
	public:
		AnimHeaderVersion7 oFileHeader;
		std::vector<BoneData> vecBoneData;
		vector<int32_t> vecMaskTranslations;
		vector<int32_t> vecMaskRotations;

		FrameHeader oStaticFrameHeader;
		Frame oStaticFrame;

		FrameHeader oFrameHeader;
		uint32_t dwFrameCount = 0;
		vector<Frame> vecFrames;
	};
}
