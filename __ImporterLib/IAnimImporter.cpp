#include "pch.h"
#include "IAnimImporter.h"

//using namespace TWAnim;
//bool TWAnim::createAnimImporter(SystemLib::DataStream& _oIn, TWAnim::IAnimExporter** ppImporter)

bool TWAnim::createAnimImporter(SystemLib::DataStream& _oIn, IAnimImporter** ppImporter)
{
	uint32_t version;
	_oIn.readMem(&version, 4);
	_oIn.seek_absolute(0);

	*ppImporter = nullptr;

	switch (version)
	{
	case 6:	*ppImporter = new AnimImporterVersion6;
		break;

	case 7:	*ppImporter = new AnimImporterVersion7;
		break;

	default:
		break;
	}

	if ((*ppImporter) == nullptr)
		return false;

	// read the .ANIM file
	return  (*ppImporter)->read(_oIn);
}

std::shared_ptr<TWAnim::IAnimImporter> TWAnim::getAnimImporter(SystemLib::DataStream& _oIn)
{
	TWAnim::IAnimImporter* poImporter;

	if (!createAnimImporter(_oIn, &poImporter))
		return nullptr;

	return std::shared_ptr<TWAnim::IAnimImporter>(poImporter);
}

//std::shared_ptr<TWAnim::IAnimExporter> TWAnim::getAnimImporter(SystemLib::DataStream& _oIn)
//{
//	IAnimExporter* poImporter;
//
//	if (!createAnimImporter(_oIn, &poImporter))
//		return nullptr;
//
//	return shared_ptr<IAnimExporter>(poImporter);
//}

//bool importer::AnimImporterCommon::readBoneData()
//{
//ool anim_file::readJointDataSkeleton()
//{
//
//	if (m_file.oMetaData.oHeader.uiVersion == 7 || m_file.oMetaData.oHeader.uiVersion == 5)
//	{
//		/*if (_file.header.uiVersion == 7)
//			read(_file.header.uk2, 8);
//		*/
//		streamoff o = m_fsFile.tellg();
//
//		// read the number
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			//ANIM_JOINT_DATA* poTemp = new ANIM_JOINT_DATA;
//			read(&m_file.PJoints[i].wNameLength, 2);
//			read(m_file.PJoints[i].szName, m_file.PJoints[i].wNameLength);
//
//			read(&m_file.PJoints[i].iParentID, sizeof(uint32_t ));
//			m_file.PJoints[i].iID = i; // the is same as the current bone counter value
//
//
//
//			_BREAK_POINT;
//		}
//
//		// For skeleton this is used to define the position of the joint
//		// For an animation only file, this is mask to indicated what joint should be rotated before an�mation starts
//
//
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			read(&m_file.PIDMaskTranslation[i], sizeof(int32_t ));
//
//
//		}
//
//
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			read(&m_file.PIDMaskRotation[i], sizeof(int32_t ));
//		}
//
//		if (m_file.oMetaData.oHeader.uiVersion == 7)
//		{
//			uint32_tu1 = 0, u2 = 0;
//			read(&u1, 4);
//			read(&u2, 4);
//		}
//
//		read(&m_file.dwTranslationCount, sizeof(uint32_t ));
//		read(&m_file.dwRotationCount, sizeof(uint32_t ));
//
//		read(&m_file.dwFrameCount, sizeof(uint32_t ));
//
//		return true;
//	}
//
//	return false;
//}

bool TWAnim::AnimImporterCommon::read(SystemLib::DataStream& input)
{
	if (!readHeader(input))
		return false;

	if (!readBoneData(input))
		return false;

	if (!readMasks(input))
		return false;

	if (!readStaticFrameHeader(input))
		return false;

	if (!readStaticFrame(input))
		return false;

	if (!readFrameHeader(input))
		return false;

	if (!readFrames(input))
		return false;

	return true;
}

//bool importer::AnimImporterCommon::readBoneData()
//{
//
//
//		// read the number
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			//ANIM_JOINT_DATA* poTemp = new ANIM_JOINT_DATA;
//			read(&m_file.PJoints[i].wNameLength, 2);
//			read(m_file.PJoints[i].szName, m_file.PJoints[i].wNameLength);
//
//			read(&m_file.PJoints[i].iParentID, sizeof(uint32_t ));
//			m_file.PJoints[i].iID = i; // the is same as the current bone counter value
//
//
//
//			_BREAK_POINT;
//		}
//
//		// For skeleton this is used to define the position of the joint
//		// For an animation only file, this is mask to indicated what joint should be rotated before an�mation starts
//
//		// if version 7 get the trans/tor data differntly
//		if (m_file.oMetaData.oHeader.uiVersion == 7)
//			m_file.dwTranslationCount = 0;
//		uint32_ttrans = 0;
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			read(&m_file.PIDMaskTranslation[i], sizeof(int32_t ));
//
//			// version 7 uses this way of counting transformations/rotations
//			/*if (m_file.oMetaData.oHeader.uiVersion == 7)
//			{
//				if (m_file.PIDMaskTranslation[i] != -1 && m_file.PIDMaskTranslation[i] < 9000)
//					m_file.dwTranslationCount++;
//
//				if (m_file.PIDMaskTranslation[i] > 9000)
//					trans++;
//			}*/
//		}
//
//		// For skeleton this is used to define the translation of the joing
//		// For an animation only file, this is mask to indicated what joints should be tranlated before an�mation starts
//		// if version 7 get the trans/tor data differntly
//		if (m_file.oMetaData.oHeader.uiVersion == 7)
//			m_file.dwRotationCount = 0;
//		uint32_trot = 0;
//		for (uint32_ti = 0; i < m_file.oMetaData.oHeader.dwJointCount; i++)
//		{
//			read(&m_file.PIDMaskRotation[i], sizeof(int32_t ));
//			// version 7 uses this way of counting transformations/rotations
//			/*if (m_file.oMetaData.oHeader.uiVersion == 7)
//			{
//				if (m_file.PIDMaskRotation[i] != -1 && m_file.PIDMaskRotation[i] < 9000)
//					m_file.dwRotationCount++;
//
//				if (m_file.PIDMaskRotation[i] > 9000)
//					rot++;
//			}*/
//		}
//
//		bool b = false;
//		if (m_file.oMetaData.oHeader.uiVersion == 7)
//			b = read_v7_ExtraHeaderInfo();
//
//
//		read(&m_file.dwTranslationCount, sizeof(uint32_t ));
//		read(&m_file.dwRotationCount, sizeof(uint32_t ));
//		read(&m_file.dwFrameCount, sizeof(uint32_t ));
//
//
//		return true;
//
//
//
//	return true;
//
//}

bool TWAnim::AnimImporterCommon::readHeader(SystemLib::DataStream& _in)
{
	// messes up the objects in the class
	//memset(&oHeader, 0, sizeof(oHeader));

	_in.readMem(&m_oAnimFile.oFileHeader.dwVersion, sizeof(uint32_t));
	_in.readMem(&m_oAnimFile.oFileHeader.dwUnknown, 4);
	_in.readMem(&m_oAnimFile.oFileHeader.fFrameRate, 4);

	// read skeleton ID string

	string_with_16_bit_length _16str;
	_16str.read(_in);
	m_oAnimFile.oFileHeader.strSkeletonId = _16str.strText;

	_in.readMem(&m_oAnimFile.oFileHeader.v7_sub_header.flag_count, 4);

	if (m_oAnimFile.oFileHeader.v7_sub_header.flag_count)
	{
		_16str.read(_in);
		m_oAnimFile.oFileHeader.v7_sub_header.strText = _16str.strText;
	}

	_in.readMem(&m_oAnimFile.oFileHeader.fEndTime, 4);
	_in.readMem(&m_oAnimFile.oFileHeader.dwBoneCount, 4);

	return true;
}

uint32_t TWAnim::AnimImporterCommon::getJointCount()
{
	return m_oAnimFile.oFileHeader.dwBoneCount;
}

bool TWAnim::AnimImporterCommon::getTranslation(uint32_t _frame, int32_t _bone, XMFLOAT3& _vTrans)
{
	if (_frame >= m_oAnimFile.vecFrames.size())
		return false;

	if (m_oAnimFile.vecMaskTranslations[_bone] == -1)
		return false;

	_vTrans = m_oAnimFile.vecFrames[_frame].vecTranslations[_bone];

	return true;
}

bool TWAnim::AnimImporterCommon::getQuaternion(uint32_t _frame, int32_t _bone, XMFLOAT4& _vQuaternion)
{
	if (_frame >= m_oAnimFile.vecFrames.size())
		return false;

	if (m_oAnimFile.vecMaskRotations[_bone] == -1)
		return false;

	_vQuaternion = m_oAnimFile.vecFrames[_frame].vecQuaternions[_bone];

	return true;
}

TWAnim::AnimFile* TWAnim::IAnimImporter::getAnimFile()
{
	return &m_oAnimFile;
}

bool TWAnim::AnimImporterCommon::readBoneData(SystemLib::DataStream& _in)
{
	// read the skelelton, bone name and parent ids
	m_oAnimFile.vecBoneData.resize(m_oAnimFile.oFileHeader.dwBoneCount);
	for (uint32_t i = 0; i < m_oAnimFile.oFileHeader.dwBoneCount; i++)
	{
		m_oAnimFile.vecBoneData[i].id = i;

		string_with_16_bit_length _16str;
		_16str.read(_in);

		m_oAnimFile.vecBoneData[i].strName = _16str.strText;
		_in.readMem(&m_oAnimFile.vecBoneData[i].parent_id, sizeof(int32_t));
	}

	return true;
}

bool TWAnim::AnimImporterCommon::readMasks(SystemLib::DataStream& _in)
{
	m_oAnimFile.vecMaskTranslations.resize(m_oAnimFile.oFileHeader.dwBoneCount);
	m_oAnimFile.vecMaskRotations.resize(m_oAnimFile.oFileHeader.dwBoneCount);

	for (const auto& it : m_oAnimFile.vecMaskTranslations)
	{
		_in.readMem((void*)&it, 4);
	}

	for (const auto& it : m_oAnimFile.vecMaskRotations)
	{
		_in.readMem((void*)&it, 4);
	}

	return true;
}

bool TWAnim::AnimImporterCommon::readStaticFrameHeader(SystemLib::DataStream& _in)
{
	_in.readMem(&m_oAnimFile.oStaticFrameHeader.dwTranslationsCount, 4);
	_in.readMem(&m_oAnimFile.oStaticFrameHeader.dwRotationCount, 4);
	return true;
}

bool TWAnim::AnimImporterCommon::readStaticFrame(SystemLib::DataStream& _in)
{
	return readSingleFrame(_in, m_oAnimFile.oStaticFrame, m_oAnimFile.oStaticFrameHeader);
}

bool TWAnim::AnimImporterCommon::readFrameHeader(SystemLib::DataStream& _in)
{
	_in.readMem(&m_oAnimFile.oFrameHeader.dwTranslationsCount, 4);
	_in.readMem(&m_oAnimFile.oFrameHeader.dwRotationCount, 4);
	_in.readMem(&m_oAnimFile.dwFrameCount, 4);
	return true;
}

bool TWAnim::AnimImporterCommon::readFrames(SystemLib::DataStream& _in)
{
	m_oAnimFile.vecFrames.resize(m_oAnimFile.dwFrameCount);
	for (auto& it : m_oAnimFile.vecFrames)
	{
		readSingleFrame(_in, it, m_oAnimFile.oFrameHeader);
	}

	return true;
}

bool TWAnim::AnimImporterCommon::readSingleFrame(SystemLib::DataStream& _in, Frame& _oFrame, const FrameHeader& _oFrameHeader)
{
	_oFrame.vecTranslations.resize(m_oAnimFile.oFileHeader.dwBoneCount);
	_oFrame.vecQuaternions.resize(m_oAnimFile.oFileHeader.dwBoneCount);

	// read translations
	for (size_t t = 0; t < _oFrameHeader.dwTranslationsCount; t++)
	{
		if (m_oAnimFile.vecMaskTranslations[t] != -1)
		{
			int storage_index = m_oAnimFile.vecMaskTranslations[t];  // get the actual index from the "mask"
			_in.readMem((void*)&_oFrame.vecTranslations[storage_index], 3 * 4);
		}
		else
		{
			_oFrame.vecTranslations[t] = { 0,0,0 };
		}
	}

	const float _divider = 32767.0f;
	// read rotations
	for (size_t q = 0; q < _oFrameHeader.dwRotationCount; q++)
	{
		if (m_oAnimFile.vecMaskRotations[q] != -1)
		{
			int storage_index = m_oAnimFile.vecMaskRotations[q];  // get the actual index from the "mask"
			PackedVector::XMSHORT4 raw;

			_in.readMem((void*)&raw, 4 * 2);

			_oFrame.vecQuaternions[storage_index].x = ((float)raw.x) / _divider;
			_oFrame.vecQuaternions[storage_index].y = ((float)raw.y) / _divider;
			_oFrame.vecQuaternions[storage_index].z = ((float)raw.z) / _divider;
			_oFrame.vecQuaternions[storage_index].w = ((float)raw.w) / _divider;
		}
		else
		{
			_oFrame.vecQuaternions[q] = { 0,0,0,1 };
		}
	}

	return true;
}

bool TWAnim::AnimImporterVersion6::readStaticFrameHeader(SystemLib::DataStream& _in)
{
	// Do nothing as ANIM v6 does not háve a "static frame"
	return true;
}

bool TWAnim::AnimImporterVersion6::readStaticFrame(SystemLib::DataStream& _in)
{
	// Do nothing as ANIM v6 does not háve a "static frame"
	return true;
}