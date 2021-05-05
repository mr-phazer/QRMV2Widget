#include "Object.h"

uint32_t Object::sm_next_id = 0;

Object::Object()
{
	m_id = this->sm_next_id++;

}
