#include "data_stream.h"
#include "pch.h"
#include "data_stream.h"
#include "..\ImporterLib\anim_structs.h"
#include "..\ImporterLib\pack_file_structs.h"
#include "..\SystemLib\tools.h"

SystemLib::DataStream::DataStream()
{
}

SystemLib::DataStream::DataStream(const void* _pSrcMem, size_t _elements)
{
	open_from_memory(_pSrcMem, _elements);
}

SystemLib::DataStream::DataStream(pack_file_node* _ppfn, size_t _elements)
{
	open_from_memory((uint8_t*)_ppfn->extractFileToMemory(), (size_t)_ppfn->file_size);
	//open(_ppfn->strExternalPath, _elements == 0 ? _ppfn->file_size : _elements);
}

SystemLib::DataStream::DataStream(const char* szStr)
{
	open(szStr);
}

SystemLib::DataStream::DataStream(std::string& _strFilePath)
{
	open(_strFilePath);
}

SystemLib::DataStream::DataStream(std::string&& _strFilePath)
{
	open(_strFilePath);
}

SystemLib::DataStream::DataStream(std::wstring& _strFilePath)
{
	open(_strFilePath);
}

//SystemLib::DataStream::DataStream(const std::string& _strFilePath)
//{
//	open(_strFilePath);
//}
//
//SystemLib::DataStream::DataStream(void* _pSrcMem, size_t _elements)
//{
//	open(_pSrcMem, _elements);
//
//}

bool SystemLib::DataStream::open(const std::string& _strFilePath)
{
	// read entire file into buffer
	m_wstrDiskFileName = tools::string_to_wstring(_strFilePath);

	// open file and place file index at the end of file
	std::ifstream file(_strFilePath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return false;

	std::streamsize size = file.tellg(); // end of file index = size of file
	file.seekg(0, std::ios::beg); // go back to the beginning

	// clear buffer
	m_vecBuffer.clear();

	// allocate
	m_vecBuffer.resize(size);

	file.read((char*)&m_vecBuffer[0], size);

	if (file.good())
	{
		file.close();
		return true;
	}

	setCurrentAddress();
	file.close();
	return false;
}

bool SystemLib::DataStream::open(const std::wstring& _strFilePath)
{
	// read entire file into buffer
	m_wstrDiskFileName = _strFilePath;

	// open file and place file index at the end of file
	std::ifstream file(_strFilePath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		return false;

	std::streamsize size = file.tellg(); // end of file index = size of file
	file.seekg(0, std::ios::beg); // go back to the beginning

	// clear buffer
	m_vecBuffer.clear();

	// allocate
	m_vecBuffer.resize(size);

	file.read((char*)&m_vecBuffer[0], size);

	if (file.good())
	{
		file.close();
		return true;
	}

	setCurrentAddress();
	file.close();
	return false;
}

bool SystemLib::DataStream::open(const std::string& _strFilePath, size_t _bytes)
{
	// read entire file into buffer

	// open file and place file index at the end of file
	std::ifstream file(_strFilePath, std::ios::binary);
	if (!file.is_open())
	{
		return m_bisValid = false;
	}

	// clear buffer
	m_vecBuffer.clear();

	// allocate
	m_vecBuffer.resize(_bytes);

	file.read((char*)&m_vecBuffer[0], _bytes);

	if (file.good())
	{
		file.close();
		return true;
	}

	setCurrentAddress();

	file.close();
	return false;
}

bool SystemLib::DataStream::open(const std::wstring& _strFilePath, size_t _bytes)
{
	// read entire file into buffer

	// open file and place file index at the end of file
	std::ifstream file(_strFilePath, std::ios::binary);
	if (!file.is_open())
	{
		return m_bisValid = false;
	}

	// clear buffer
	m_vecBuffer.clear();

	// allocate
	m_vecBuffer.resize(_bytes);

	file.read((char*)&m_vecBuffer[0], _bytes);

	if (file.good())
	{
		file.close();
		return true;
	}

	setCurrentAddress();

	file.close();
	return false;
}

bool SystemLib::DataStream::open_from_memory(const void* _pSrcMem, size_t _elements)
{
	if (!_pSrcMem)
	{
		m_bisValid = false;
		return m_bisValid;
	}
	// clear buffer
	m_vecBuffer.clear();

	// allocate
	m_vecBuffer.resize(_elements);

	// copy memory
	memcpy(&m_vecBuffer[0], _pSrcMem, _elements);

	m_index = 0;

	m_bisValid = true;

	setCurrentAddress();
	return m_bisValid;
}

bool SystemLib::DataStream::clearAndReset()
{
	m_vecBuffer.clear();
	m_index = 0;

	setCurrentAddress();
	return true;
}

bool SystemLib::DataStream::readMem(void* _pDestMem, size_t _elements)
{
	if (_elements == 0)
	{
		return false;
	}

	if (_elements + m_index > m_vecBuffer.size())
	{
		return false;
	}

	memcpy(_pDestMem, &m_vecBuffer[m_index], _elements);

	m_index += _elements;

	setCurrentAddress();
	return true;
}

bool SystemLib::DataStream::readSZ(std::string& _strOut)
{
	// clear the input string,
	_strOut.clear();

	// locl vars
	char ch = '\0';
	bool bResult = true;
	do {
		bResult = readMem(&ch, 1);
		_strOut += ch;

		if (!bResult)
			return bResult;
	} while (ch != '\0'); // break if EOF or NULL char encounterd

	return true;
}

bool SystemLib::DataStream::readWSZ(std::wstring& _strOut)
{
	// clear the input string,
	_strOut.clear();

	// locl vars
	wchar_t wch = L'\0';
	bool bResult = true;
	do {
		bResult = readMem(&wch, 2);
		_strOut += wch;

		if (!bResult)
			return bResult;
	} while (wch != L'\0'); // break if EOF o,,r NULL char encounterd

	return true;
}

bool SystemLib::DataStream::writeWSZ(std::wstring& _strOut)
{
	for (auto& wch : _strOut)
	{
		writeMem(&wch, 2);
	}

	auto wch_null = L'\0';
	writeMem(&wch_null, 2);
	return false;
}

bool SystemLib::DataStream::writeMem(void* _pSrcMem, size_t _elements)
{
	size_t bytes_to_read = _elements;

	if (_elements + m_index >= m_vecBuffer.size())
	{
		m_vecBuffer.resize(_elements + m_vecBuffer.size());

		//size_t bytes_to_read = _elements - m_vecBuffer.size();/
	}

	//m_vecBuffer.resize(_elements + m_vecBuffer.size());

	memcpy(&m_vecBuffer[m_index], _pSrcMem, _elements);
	m_index += _elements;

	setCurrentAddress();
	return true;
}

size_t SystemLib::DataStream::write_file(const std::wstring& _strFilePath)
{
	auto strPath = (_strFilePath != L"") ? _strFilePath : m_wstrDiskFileName;

	if (m_vecBuffer.size() == 0)
		return 0;

	std::ofstream file(strPath, std::ios::binary);

	if (!file.is_open())
		return 0;

	file.write((char*)m_vecBuffer.data(), m_vecBuffer.size());

	setCurrentAddress();
	return m_vecBuffer.size();
}

SystemLib::EStreamFlags SystemLib::DataStream::seek_relative(int _offset)
{
	if (m_index == 0 && _offset < 0)
		return EStreamFlags::eInvalidOffset;

	m_index += _offset;

	if (m_index >= m_vecBuffer.size())
		return EStreamFlags::eEndOfStream;

	setCurrentAddress();
	return EStreamFlags::eSuccess;
}

SystemLib::EStreamFlags SystemLib::DataStream::seek_absolute(size_t _offset)
{
	if (_offset < 0)
		return EStreamFlags::eInvalidOffset;

	if (_offset >= m_vecBuffer.size())
		return EStreamFlags::eEndOfStream;

	m_index = _offset;

	setCurrentAddress();
	return EStreamFlags::eSuccess;
}

//bool SystemLib::DataStream::isValid()
//{
//	return (m_vecBuffer.size() != 0 && b);
//}

const uint8_t* SystemLib::DataStream::data() const
{
	return &m_vecBuffer[0];
}

uint8_t* SystemLib::DataStream::data()
{
	return &m_vecBuffer[0];
}

size_t SystemLib::DataStream::tellp()
{
	return m_index;
}