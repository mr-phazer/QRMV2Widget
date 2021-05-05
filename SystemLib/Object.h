#pragma once

#include <string>

struct Object
{
public:
	Object();



private:
	uint32_t m_id = 0;
	static uint32_t sm_next_id;

};

