#pragma once

#include <string>

class IAbstractObject
{
public:
	virtual bool destroy() { return true; };

protected:
	std::string m_strName = "";
};

