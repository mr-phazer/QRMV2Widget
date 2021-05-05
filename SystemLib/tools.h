#pragma once

#include <codecvt>
#include <string>
#include <algorithm>
#include <fstream>
//#include <locale> 


using namespace std;

namespace tools
{

	extern std::wstring string_to_wstring(const std::string& str);

	extern std::string wstring_to_string(const std::wstring& wstr);



	template <class T>
	static bool compare(T _1, T _2);


	template <>
	static bool compare(string _1, string _2);



	std::string GetDirFromFullPath(const std::string& _str);
	std::wstring GetDirFromFullPath(const std::wstring& _str);

	std::string GetExtensionFromFullPath(const std::string& _str);
	std::wstring GetExtensionFromFullPath(const std::wstring& _str);
		
	std::string GetFileFromFullPath(const std::string& _strInput);	
	std::wstring GetFileFromFullPath(const std::wstring& _strInput);


	
	std::string lower(const std::string& _strInput);	
	std::string upper(const std::string& _strInput);	
	
	std::wstring lower(const std::wstring& _strInput);	
	std::wstring upper(const std::wstring& _strInput);	
	
	
	std::string getLowerAndBackSlash(const std::string& _strInput);



	size_t getFileSize(const std::string& strFileName);
	size_t getFileSize(const std::wstring& strFileName);





	template<class T>
	bool compare(T _1, T _2)
	{				
		return ( _1 == _2 );		
	}
	

	template<>
	bool compare(string _1, string _2)
	{		
		auto s1 = lower(_1);
		auto s2 = lower(_2);

		return ( s1 == s2 );
	}

}