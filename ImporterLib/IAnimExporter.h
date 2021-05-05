#pragma once

#include <memory>

#include "..\SystemLib\data_stream.h"
//#include "..\__RenderLib\skeleton.h"
#include "..\ImporterLib\AnimFIle.h"

namespace TWAnim
{
	class IAnimExporter
	{
		//friend std::shared_ptr<IAnimExporter> createAnimExporter(SystemLib::DataStream& _oIn);


	public:
		virtual bool write(SystemLib::DataStream& _out) = 0;

	protected:
		virtual bool writeHeader(SystemLib::DataStream& _out) = 0;
		virtual bool writeBoneData(SystemLib::DataStream& _out) = 0;

		virtual bool writeMasks(SystemLib::DataStream& _out) = 0;

		virtual bool writeStaticFrameHeader(SystemLib::DataStream& _out) = 0;
		virtual bool writeStaticFrame(SystemLib::DataStream& _out) = 0;

		virtual bool writeFrameHeader(SystemLib::DataStream& _out) = 0;
		virtual bool writeFrames(SystemLib::DataStream& _out) = 0;

		virtual bool writeInverseBindPoseMatrices(SystemLib::DataStream& _out) = 0;


	public:
		AnimFile oAnimFile;

	//public:
	//	
	//	AnimHeaderVersion7  oHeader;
	//	std::vector<pair<std::string, int32_t>> vecBoneData;

	//	vector<int32_t> vecMaskTranslations;
	//	vector<int32_t> vecMaskRotations;

	//	FrameHeader oStaticFrameHeader;
	//	Frame oStaticFrame;

	//	FrameHeader oFrameHeader;
	//	uint32_t dwFrameCount = 0;
	//	vector<Frame> m_vecFrames;



	};
	
	class AnimExporterVersion7 : public IAnimExporter
	{
	public:
		
		virtual bool write(SystemLib::DataStream& _out) override;
		

	protected:
		virtual bool writeHeader(SystemLib::DataStream& _out) override;
		virtual bool writeBoneData(SystemLib::DataStream& _out) override;

		virtual bool writeMasks(SystemLib::DataStream& _out) override;

		virtual bool writeStaticFrameHeader(SystemLib::DataStream& _out);
		virtual bool writeStaticFrame(SystemLib::DataStream& _out);

		virtual bool writeFrameHeader(SystemLib::DataStream& _out);
		virtual bool writeFrames(SystemLib::DataStream& _out);

		virtual bool writeSingleFrame(SystemLib::DataStream& _out, const Frame& oFrame);
	
	protected:
		
		
		virtual bool writeInverseBindPoseMatrices(SystemLib::DataStream& _out) override
		{
			return false;
		}

	




	};

	extern std::shared_ptr<IAnimExporter> createAnimExporter(SystemLib::DataStream& _oIn);

	//extern std::shared_ptr<IAnimExporter> getAnimImporter(SystemLib::DataStream& _oInr);
};