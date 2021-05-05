#include "tools.h"
#include "tools.h"

std::wstring tools::string_to_wstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string tools::wstring_to_string(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::string tools::GetDirFromFullPath(const std::string& _str)
{
	string strTemp = _str;

	if (strTemp == "")
		return "";

	size_t i = 0;
	int i1 = 0;
	size_t index = strTemp.size();
	size_t cut = 0;;

	while (strTemp[index] != 92 && strTemp[index] != '/' && index > 0)
	{
		index--;
		cut++;
	}
	index--;

	if (index == 0)
		return strTemp;

	strTemp.erase(index + 2, cut - 1);

	return strTemp;
}

std::wstring tools::GetDirFromFullPath(const std::wstring& _str)
{
	wstring strTemp = _str;

	if (strTemp == L"")
		return L"";

	size_t i = 0;
	int i1 = 0;
	size_t index = strTemp.size();
	size_t cut = 0;;

	while (strTemp[index] != L'\\' && strTemp[index] != L'/' && index > 0)
	{
		index--;
		cut++;
	}
	index--;

	if (index == 0)
		return strTemp;

	strTemp.erase(index + 2, cut - 1);

	return strTemp;
}

std::string tools::GetExtensionFromFullPath(const std::string& _str)
{
	if (_str == "")
		return "";

	bool bFounDot = false;
	size_t len = _str.length();
	size_t index = len - 1;

	string strTemp = "";

	// move the index back to where the extension starts
	while (_str[index] != '.' && index > 0)
	{
		index--;
	}

	if (index <= 0)
		return "";

	strTemp = &_str[index + 1];

	return strTemp;
}

std::wstring tools::GetExtensionFromFullPath(const std::wstring& _str)
{
	if (_str == L"")
		return L"";

	bool bFounDot = false;
	size_t len = _str.length();
	size_t index = len - 1;

	wstring strTemp = L"";

	// move the index back to where the extension starts
	while (_str[index] != L'.' && index > 0)
	{
		index--;
	}

	if (index <= 0)
		return L"";

	strTemp = &_str[index + 1];

	return strTemp;
}

std::string tools::GetFileFromFullPath(const std::string& _strInput)
{
	const char* _csz = _strInput.c_str();
	size_t len = strlen(_csz);

	if (len == 0)
	{
		return "";
	}

	size_t index = len - 1;

	// move the index back to where the extension starts
	while (_csz[index] != '\\' && _csz[index] != '/' && index > 0)
	{
		index--;
	}

	if (index < 0)
		return "";

	if (index == 0) // if there is no path in front of file name½
		return &_csz[index];

	return &_csz[index + 1];
}

std::wstring tools::GetFileFromFullPath(const std::wstring& _strInput)
{
	const wchar_t* _csz = _strInput.c_str();
	size_t len = _strInput.size();
	size_t index = len - 1;

	// move the index back to where the extension starts
	while (_csz[index] != L'\\' && _csz[index] != L'/' && index > 0)
	{
		index--;
	}

	if (index < 0)
		return L"";

	if (index == 0) // if there is no path in front of file name½
		return &_csz[index];

	return &_csz[index + 1];
}

string tools::lower(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

string tools::upper(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

wstring tools::lower(const std::wstring& _strInput)
{
	std::wstring strOut;
	strOut.resize(_strInput.size(), L' ');

	for (size_t i = 0; i < _strInput.size(); i++)
	{
		strOut[i] = tolower(_strInput[i]);
	}

	return strOut;
}

wstring tools::upper(const std::wstring& _strInput)
{
	std::wstring strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

string tools::getLowerAndBackSlash(const std::string& _strInput)
{
	std::string strOut = _strInput;

	for (auto& it : strOut)
	{
		it = tolower(it);
		if (it == '/')
			it = '\\';
	}

	//transform(strOut.begin(), strOut.end(), strOut.begin(), tolower);

	return strOut;
}

size_t tools::getFileSize(const std::string& strFileName)
{
	struct _stat stat_buf {};
	int rc = _stat(strFileName.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : 0;

	std::ifstream in(strFileName, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();

	streampos begin, end;
	ifstream ifsFile(strFileName, ios::in | ios::binary);
	/* if ( ifsFile.fail() ) {
		 this->m_error_code = predefined_error_codes::FILE_NOT_FOUND;
		 if (window::getMainWindow())
			 window::getOKErrorBox(window::getMainWindow(), "Error!", "File not opened.");
		 else
			 window::getOKErrorBox(NULL, "Error!", "File not opened.");

		 writeDebugMessage("rmv2_file::_getFileSize: File not opened");

		 return false;
	}*/

	begin = ifsFile.tellg();
	ifsFile.seekg(0, ios::end);
	end = ifsFile.tellg();
	ifsFile.close();

	size_t rEnd = end;
	size_t rBegin = begin;

	//if (rEnd - rBegin >= 0) {
	//	this->m_error_code = predefined_error_codes::FILE_SIZE_ZERO;
	//	if (window::getMainWindow())
	//		window::getOKErrorBox(window::getMainWindow(), "Error!", "File size = 0!");
	//	else
	//		window::getOKErrorBox(NULL, "Error!", "File size = 0!");

	//	writeDebugMessage("rmv2_file::_getFileSize: File size = 0!");

	//	return false;
	//}

	return (rEnd - rBegin);
}

size_t tools::getFileSize(const std::wstring& strFileName)
{
	struct _stat stat_buf {};
	int rc = _wstat(strFileName.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : 0;
}