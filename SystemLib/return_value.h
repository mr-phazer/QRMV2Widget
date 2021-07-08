#pragma once
#include <d3d11.h>

#include <map>

#include "Object.h"
#include "data_stream.h"
#include <comdef.h>
#include <QtCore\qstring.h>

using namespace std;

namespace SystemLib
{
	struct return_value
	{
		static return_value Error(int code, const std::string& strMsg)
		{
			return return_value(code, strMsg);
		}

		return_value() { pairValue = { 0, "" }; }

		return_value(bool b, const std::string& strMsg)
		{
			pairValue = { b ? 1 : -1, strMsg == "" ? "Operation Succeded." : strMsg };
		}

		return_value(const std::string& strMsg)
		{
			pairValue = { 0, strMsg == "" ? "Operation Succeded." : strMsg };
		}

		return_value(int code, const std::string& strMsg = "")
		{
			pairValue = { code, strMsg == "" ? "Operation Succeded." : strMsg };
		}

		return_value(bool _bState) {
			if (_bState)
				pairValue = { 0, "Operation Succeded." };
			else
				pairValue = { -1, "Error happened!" };
		}

		return_value(HRESULT _hrResult) {
			_com_error err(_hrResult);
			//std::wstring wstrErrorMsg = err.ErrorMessage();
			//std::string strErrorMsg = QString::fromStdWString(wstrErrorMsg).toStdString();
			std::string strErrorMsg = std::string();

			if (SUCCEEDED(_hrResult))
				pairValue = { 0, strErrorMsg.c_str() };
			else
				pairValue = { -1, strErrorMsg.c_str() };
		}

		operator bool()
		{
			return (pairValue.first > -1);
		}

		operator const char* ()
		{
			return pairValue.second.c_str();
		}

		operator const std::string& ()
		{
			return pairValue.second;
		}

		operator int()
		{
			return pairValue.first;
		}

		std::pair<int, std::string> pairValue;
	};
}