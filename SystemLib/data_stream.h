#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include <assert.h>

struct pack_file_node;

namespace SystemLib
{
	enum EStreamFlags : int
	{
		eSuccess = 0,
		eEndOfStream = 1,
		eInvalidOffset = 2
	};

	class DataStream
	{
	public:
		DataStream();

		DataStream(const char*);
		DataStream(std::string& _strFilePath);
		DataStream(std::string&& _strFilePath);

		DataStream(std::wstring& _strFilePath);

		/*DataStream(const std::string& _strFilePath);
		DataStream(const std::string _strFilePath);
		*/

		DataStream(const void* _pSrcMem, size_t _elements);

		DataStream(pack_file_node* _ppfn, size_t _elements = 0);

		DataStream(DataStream& copy)
		{
			assert("copy constructor not implmented" == "");
		}

		bool isValid() { return m_bisValid; };
		size_t getSize() { return m_vecBuffer.size(); };

		operator bool() { return m_bisValid; };

	public:
		bool open(const std::string& _strFilePath);
		bool open(const std::wstring& _strFilePath);

		/// <summary>
		/// Only reads the specific part of the file
		/// </summary>
		/// <param name="_strFilePath">File Path</param>
		/// <param name="_bytes">Bytes to read</param>
		/// <returns>true/false</returns>
		bool open(const std::string& _strFilePath, size_t _bytes);
		bool open(const std::wstring& _strFilePath, size_t _bytes);

		bool open_from_memory(const void* _pSrcMem, size_t _elements);

		bool clearAndReset();

		bool readMem(void* _pDestMem, size_t _elements);
		//bool readMem(void* _pDestMem, size_t _elements);

		// read null terminated string
		bool readSZ(std::string& _strOut);

		// read null terminated wide string
		bool readWSZ(std::wstring& _strOut);

		// write null terminated string
		bool writeSZ(std::string& _strOut);

		// write null terminated wide string
		bool writeWSZ(std::wstring& _strOut);

		template <typename T>
		bool readMem_As(T& _str);

		bool writeMem(void* _pSourceMem, size_t _elements);

		template <typename T>
		bool read(T* _poT);

		// TODO: make this return the number of bytes _actual_ written
		// return the size of the buffer being written
		size_t write_file(const std::wstring& _strFilePath = L"");

		size_t tellp();

		SystemLib::EStreamFlags seek_relative(int _offset);
		SystemLib::EStreamFlags seek_absolute(size_t _offset);

		const uint8_t* data() const;
		uint8_t* data();

		uint8_t* pCurrentAddress = nullptr;

	private:
		void setCurrentAddress()
		{
			if (m_vecBuffer.size() == 0)
			{
				pCurrentAddress = nullptr;
				return;
			}

			if (m_index < m_vecBuffer.size())
			{
				// if NOT after end of index set pointer
				pCurrentAddress = &m_vecBuffer[m_index];
			}
			else
			{// IF after end of index set pointer = END
				pCurrentAddress = &m_vecBuffer[m_vecBuffer.size() - 1];
			}
		};

	public:
		std::vector<uint8_t> m_vecBuffer;
		size_t m_index = 0;
		std::wstring m_wstrDiskFileName = L"";

		bool m_bisValid = false;
	};

	template<>
	inline bool DataStream::readMem_As(std::string& _str)
	{
		char ch = '\0';
		_str.clear();
		do {
			readMem(&ch, 1);
			_str += ch;
		} while (ch);

		return true;
	}

	template<typename T>
	inline bool DataStream::read(T* _pot)
	{
		return DataStream::read(_pot, sizeof(T));
	};

	//// explicit instantion
	//template <> bool SystemLib::data_stream::read<string_with_16_bit_length>(string_with_16_bit_length* _poString)
	//{
	//	return _poString->read(*this);
	//}

	//template bool data_stream::read<double>(double*);
};