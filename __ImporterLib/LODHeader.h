#pragma once

//#include "pch.h"
#include <DirectXMath.h>

#include "..\SystemLib\data_stream.h"
#include "..\SystemLib\ISerializeabe.h"
#include "..\SystemLib\SerializeVector.h"
#include "RigidModelConst.h"

namespace ImporterLib
{
	namespace RigidModel
	{
		struct LodQualityLevel
		{
			uint8_t quality_level = 0;
			uint8_t flag1 = 125;
			uint8_t flag2 = 136;
			uint8_t flag3 = 174;
		};

		struct LODHeaderCommon : public SystemLib::ISerializable
		{
			void reset()
			{
				dwGroupCount = 0;
				dwVerticesDataLength = 0;
				dwIndicesDataLength = 0;
				dwStartOffset = 0;
				fVisibilityDistance = 100.0f; // Assemued to indicate when to switch to another LOD, standard values for 4 LODs are usually 100.0f, 200.0f, 400.0f, 500.0f.
				dwAuthoredLodNumber = 0;					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that
				oQualityLevel.quality_level = 0;
				oQualityLevel.flag1 = 0;
				oQualityLevel.flag2 = 0;
				oQualityLevel.flag3 = 0;
			};

			virtual uint32_t getGroupCount() { return dwGroupCount; };
			virtual uint32_t getVertexDataSize() { return dwVerticesDataLength; };
			virtual uint32_t getIndexDataSize() { return dwIndicesDataLength; };
			virtual uint32_t getStartOffset() { return dwStartOffset; };
			virtual float getVisibilityDistance() { return fVisibilityDistance; };
			virtual uint32_t getAuthoredLodNumber() { return dwAuthoredLodNumber; };					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that
			virtual LodQualityLevel getQualityLevelAndFlags() { return oQualityLevel; };

			uint32_t dwGroupCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 100.0f; // Assemued to indicate when to switch to another LOD, standard values for 4 LODs are usually 100.0f, 200.0f, 400.0f, 500.0f.

			uint32_t dwAuthoredLodNumber = 0;					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that

			LodQualityLevel oQualityLevel;

			/// <summary>
			/// <c>virtual SystemLib::return_value read</c> Write the LOD Header Data that is common to all RMV2 versions
			/// </summary>
			/// <param name="_oInputDataStream"> : DataStream for reading</param>
			/// <returns>class SystemLib::return_value with proper values set</returns>
			virtual SystemLib::return_value read(SystemLib::DataStream& _oInputDataStream) override
			{
				_oInputDataStream.readMem((char*)&dwGroupCount, 4);
				_oInputDataStream.readMem((char*)&dwVerticesDataLength, 4);
				_oInputDataStream.readMem((char*)&dwIndicesDataLength, 4);

				_oInputDataStream.readMem((char*)&dwStartOffset, 4);
				_oInputDataStream.readMem((char*)&fVisibilityDistance, 4);

				return true;
			}

			/// <summary>
			/// <c>virtual SystemLib::return_value write()</c>
			/// >Write the LOD Header Data that is common to all RMV2 versions
			/// </summary>
			/// <param name="_oInputDataStream"> : DataStream for reading</param>
			/// <returns>class SystemLib::return_value with proper values set</returns>
			virtual SystemLib::return_value write(SystemLib::DataStream& _oOutPutDataStream) override
			{
				_oOutPutDataStream.writeMem((char*)&dwGroupCount, 4);
				_oOutPutDataStream.writeMem((char*)&dwVerticesDataLength, 4);
				_oOutPutDataStream.writeMem((char*)&dwIndicesDataLength, 4);

				_oOutPutDataStream.writeMem((char*)&dwStartOffset, 4);
				_oOutPutDataStream.writeMem((char*)&fVisibilityDistance, 4);

				return true;
			}
		};

		struct LODHeaderV7 : public LODHeaderCommon
		{
			uint32_t getGroupCount() { return dwGroupCount; };
			uint32_t getVertexDataSize() { return dwVerticesDataLength; };
			uint32_t getIndexDataSize() { return dwIndicesDataLength; };
			uint32_t getStartOffset() { return dwStartOffset; };
			float getVisibilityDistance() { return fVisibilityDistance; };
			uint32_t getAuthoredLodNumber() { return dwAuthoredLodNumber; };					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that
			LodQualityLevel getQualityLevelAndFlags() { return oQualityLevel; };

			static constexpr size_t _size(int version)
			{
				return sizeof(LODHeaderV7);
			}

			virtual SystemLib::return_value read(SystemLib::DataStream& _in) override
			{
				// read the LODHeader data common to all RMV2 versions
				LODHeaderCommon::read(_in);

				//dwAuthoredLodNumber = _lod_number;
				_in.readMem(&dwAuthoredLodNumber, 4);
				_in.readMem(&oQualityLevel, 4);

				return true;
			}

			SystemLib::return_value write(SystemLib::DataStream& _oOut) override
			{
				/*oOut.read((char*)&dwGroupCount, 4);
				oOut.read((char*)&dwVerticesDataLength, 4);
				oOut.read((char*)&dwIndicesDataLength, 4);

				oOut.read((char*)&dwStartOffset, 4);
				oOut.read((char*)&fVisibilityDistance, 4);*/

				// read the LODHeader data common to all RMV2 versions
				LODHeaderCommon::write(_oOut);

				// Verison 7 and 8, 2 x 4 extra bytes
				_oOut.writeMem((char*)&dwAuthoredLodNumber, 4);
				_oOut.writeMem((char*)&oQualityLevel, 4);

				return true;
			};
		}; //  END LodHeader
	}; // END: namespace RigidModel
}; // END: namespace ImporterLib
