#include "pch.h"
#include "anim_structs.h"

SystemLib::return_value string_with_16_bit_length::read(SystemLib::DataStream& _in)
{
	uint16_t wLength = 0;
	_in.readMem(&wLength, 2);

	strText.clear();
	strText.resize(wLength);
	_in.readMem(&strText[0], wLength);

	return true;
}

SystemLib::return_value string_with_16_bit_length::write(SystemLib::DataStream& _out)
{
	uint16_t len = (uint16_t)strText.size();
	_out.writeMem(&len, 2);
	_out.writeMem(&strText[0], strText.size());

	return true;
}