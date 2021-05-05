#include "IAnimExporter.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "..\DirectXTK\Inc\SimpleMath.h"
#include "..\ImporterLib\AnimFIle.h"
#include "..\SystemLib\system_clock.h"

using namespace DirectX;

shared_ptr<TWAnim::IAnimExporter> TWAnim::createAnimExporter(SystemLib::DataStream& _oIn)
{
	return shared_ptr<TWAnim::IAnimExporter>(new TWAnim::AnimExporterVersion7);
}

bool TWAnim::AnimExporterVersion7::write(SystemLib::DataStream& _out)
{
	writeHeader(_out);
	writeBoneData(_out);
	writeMasks(_out);

	writeStaticFrameHeader(_out);
	writeStaticFrame(_out);

	writeFrameHeader(_out);
	writeFrames(_out);

	return true;
}

bool TWAnim::AnimExporterVersion7::writeHeader(SystemLib::DataStream& _in)
{
	_in.writeMem(&oAnimFile.oFileHeader.dwVersion, sizeof(uint32_t));
	_in.writeMem(&oAnimFile.oFileHeader.dwUnknown, 4);
	_in.writeMem(&oAnimFile.oFileHeader.fFrameRate, 4);

	// write skeleton ID string
	uint16_t len = (uint16_t)oAnimFile.oFileHeader.strSkeletonId.size();
	_in.writeMem(&len, 2);
	_in.writeMem((void*)oAnimFile.oFileHeader.strSkeletonId.data(), oAnimFile.oFileHeader.strSkeletonId.size());

	_in.writeMem(&oAnimFile.oFileHeader.v7_sub_header.flag_count, 4);

	if (oAnimFile.oFileHeader.v7_sub_header.flag_count)
	{
		// if flag/count set, write extra ID string
		len = (uint16_t)oAnimFile.oFileHeader.v7_sub_header.strText.size();
		_in.writeMem(&len, 2);
		_in.writeMem((void*)oAnimFile.oFileHeader.v7_sub_header.strText.data(),
			oAnimFile.oFileHeader.v7_sub_header.strText.size());
	}

	_in.writeMem(&oAnimFile.oFileHeader.fEndTime, 4);
	_in.writeMem(&oAnimFile.oFileHeader.dwBoneCount, 4);

	return true;
}

bool TWAnim::AnimExporterVersion7::writeBoneData(SystemLib::DataStream& _out)
{
	for (const auto& it : oAnimFile.vecBoneData)
	{
		uint16_t len = (uint16_t)it.strName.length();
		_out.writeMem(&len, 2); // write string len in 16 bits
		_out.writeMem((void*)it.strName.data(), len); // write string

		_out.writeMem((void*)&it.parent_id, 4); // write parent id
	}

	return true;
}

bool TWAnim::AnimExporterVersion7::writeMasks(SystemLib::DataStream& _out)
{
	for (const auto& it : oAnimFile.vecMaskTranslations)
	{
		_out.writeMem((void*)&it, 4);
	}

	for (const auto& it : oAnimFile.vecMaskRotations)
	{
		_out.writeMem((void*)&it, 4);
	}

	return true;
}

bool TWAnim::AnimExporterVersion7::writeStaticFrameHeader(SystemLib::DataStream& _o)
{
	_o.writeMem(&oAnimFile.oStaticFrameHeader.dwTranslationsCount, 4);
	_o.writeMem(&oAnimFile.oStaticFrameHeader.dwRotationCount, 4);
	return true;
}

bool TWAnim::AnimExporterVersion7::writeStaticFrame(SystemLib::DataStream& _o)
{
	return false;
}

bool TWAnim::AnimExporterVersion7::writeFrameHeader(SystemLib::DataStream& o)
{
	o.writeMem(&oAnimFile.oFrameHeader.dwTranslationsCount, 4);
	o.writeMem(&oAnimFile.oFrameHeader.dwRotationCount, 4);
	o.writeMem(&oAnimFile.dwFrameCount, 4);
	return true;
}

bool TWAnim::AnimExporterVersion7::writeFrames(SystemLib::DataStream& _o)
{
	for (const auto& it : oAnimFile.vecFrames)
		writeSingleFrame(_o, it);

	return true;
}

bool TWAnim::AnimExporterVersion7::writeSingleFrame(SystemLib::DataStream& _o, const Frame& oFrame)
{
	// write translations
	for (const auto& it : oFrame.vecTranslations)
	{
		_o.writeMem((void*)&it, 3 * 4);
	}

	const float _divider = 32767.0f;
	// write rotations
	for (const auto& it : oFrame.vecQuaternions)
	{
		PackedVector::XMSHORT4 q;

		q.x = it.x * _divider;
		q.y = it.y * _divider;
		q.z = it.z * _divider;
		q.w = it.w * _divider;

		_o.writeMem((void*)&q, 4 * 2);
	}

	return true;
}