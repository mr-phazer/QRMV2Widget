#pragma once

#include "..\QtTestRMV2\QConsole.h"

namespace RigidModelV2
{
	namespace Error
	{
		bool lodOffset_Check(uint32_t _file_pos, uint32_t _lod_offset)
		{
			if (_file_pos != _lod_offset)
			{
				// _log_action_error("Fatal Error: LOD Offset / File position mismatch. Skipping.");

				return false;
			}

			return true;
		}
	}
}