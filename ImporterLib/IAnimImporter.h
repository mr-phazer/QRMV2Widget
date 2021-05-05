#pragma once

// C++ Standard Lib
#include <memory>

// Direct3d
#include <DirectXMath.h>

// Author
#include "..\SystemLib\data_stream.h"
#include "..\ImporterLib\anim_structs.h"

#include "AnimFIle.h"

using namespace DirectX;

namespace TWAnim
{		

	class IAnimImporter
	{
		friend bool	createAnimImporter(SystemLib::DataStream & _oIn, IAnimImporter * *ppImporter);


	public:
		virtual bool read(SystemLib::DataStream& _in) = 0;
		virtual uint32_t getJointCount() = 0;
		virtual bool getTranslation(uint32_t _frame, int32_t _bone, XMFLOAT3& _vTrans) = 0;
		virtual bool getQuaternion(uint32_t _frame, int32_t _bone, XMFLOAT4& _vQuaternion) = 0;
	
		virtual AnimFile* getAnimFile();

	protected:
		virtual bool readHeader(SystemLib::DataStream& _in) = 0;
		virtual bool readBoneData(SystemLib::DataStream& _in) = 0;

		virtual bool readMasks(SystemLib::DataStream& _in) = 0;

		virtual bool readStaticFrameHeader(SystemLib::DataStream& _in) = 0;
		virtual bool readStaticFrame(SystemLib::DataStream& _in) = 0;

		virtual bool readFrameHeader(SystemLib::DataStream& _in) = 0;
		virtual bool readFrames(SystemLib::DataStream& _in) = 0;

		virtual bool readSingleFrame(SystemLib::DataStream& _in, Frame& oFrame, const FrameHeader & oFrameHeader) = 0;

	

	protected:
		SystemLib::DataStream m_oStream;

	protected:
		AnimFile m_oAnimFile;
	
	};


	class AnimImporterCommon : public IAnimImporter
	{
	
	public:		
		virtual bool read(SystemLib::DataStream& _in) override;
		virtual uint32_t getJointCount() override;
		
		virtual bool getTranslation(uint32_t _frame, int32_t _bone, XMFLOAT3& _vTrans) override;
		virtual bool getQuaternion(uint32_t _frame, int32_t _bone, XMFLOAT4& _vQuaternion) override;


	protected:
		virtual bool readHeader(SystemLib::DataStream& _in) override;

	protected:
		virtual bool readBoneData(SystemLib::DataStream& _in) override;
		virtual bool readMasks(SystemLib::DataStream& _in) override;

		virtual bool readStaticFrameHeader(SystemLib::DataStream& _in) override;
		virtual bool readStaticFrame(SystemLib::DataStream& _in) override;

		virtual bool readFrameHeader(SystemLib::DataStream& _in) override;
		virtual bool readFrames(SystemLib::DataStream& _in) override;

		virtual bool readSingleFrame(SystemLib::DataStream& _in, Frame& oFrame, const FrameHeader& oFrameHeader) override;
	


	
	};

	class AnimImporterVersion6 : public AnimImporterCommon
	{
				

	protected:	
		virtual bool readStaticFrameHeader(SystemLib::DataStream& _in) override;
		virtual bool readStaticFrame(SystemLib::DataStream& _in) override;

	
	
	};
	  
	

	class AnimImporterVersion7 : public AnimImporterCommon
	{	
	
		
		


		//vector<RenderLib::pose> m_vecExtraFrame; // 1 pose = 1 frame (without non-moving bone-info)
		//vector<RenderLib::pose> m_vecFrames; // 1 pose = 1 frame (without non-moving bone-info)

	};

	extern bool createAnimImporter(SystemLib::DataStream & _oIn, IAnimImporter * *ppImporter);
	extern std::shared_ptr<IAnimImporter> getAnimImporter(SystemLib::DataStream& _oInr);
}