#include "system_clock.h"

void SystemLib::system_clock::initController()
{
	// get the frequence of clock
	LARGE_INTEGER liTemp;
	QueryPerformanceFrequency(&liTemp);
	sm_qwFrencuency = liTemp.QuadPart;

	// get current clock tick value
	QueryPerformanceCounter(&liTemp);

	sm_qwAppStartValue = liTemp.QuadPart;
}

float SystemLib::system_clock::getSeconds()
{
	LARGE_INTEGER liTemp;
	QueryPerformanceCounter(&liTemp);

	uint64_t qw = liTemp.QuadPart;

	float dRet = (float)qw / float(sm_qwFrencuency);

	return dRet;
}

float SystemLib::system_clock::getSecondsSinceAppStart()
{
	LARGE_INTEGER liTemp;
	QueryPerformanceCounter(&liTemp);

	uint64_t qw = liTemp.QuadPart - sm_qwAppStartValue;

	float dRet = (float)qw / float(sm_qwFrencuency);

	return dRet;
}

uint64_t SystemLib::system_clock::sm_qwFrencuency;
uint64_t SystemLib::system_clock::sm_qwAppStartValue;