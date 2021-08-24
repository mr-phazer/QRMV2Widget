#pragma once

#include <string>
#include <memory>
#include <vector>

using namespace std;

struct PACK_FILE_HEADER
{
	union _oHeaderId
	{
		char sz[4];  // 0..4
		uint32_t dw; // = 0x12345678;
	}
	oHeaderId;

	uint32_t dwTypeFlags = 0; // 4..8


	// query/set number of contained files

	// 8..12
	uint32_t dwPackileIndexCount = 0;	// query/set names of pack file replaced by this


	// 12..16
	uint32_t dwPackFileIndexSize = 0;	// query/set names of pack file replaced by this


	// 16..	20
	uint32_t uiFileCount = 0;;

	// 24 - 28
	// start of path data
	uint32_t iDataStart = 0;

	//uint32_t Unknown3;	// query/set names of pack file replaced by this

	// 28-32
	/*uint8_t uk1;
	uint8_t uk2;
	uint8_t uk3;
	uint8_t uk4;*/

	uint32_t uk1;

	char filler[100];



};

struct memory_file;

struct pack_file_node
{
	pack_file_node()
	{
		int i = 1;
	}
		
	pack_file_node(const pack_file_node& _pfn)
	{
		strPackFullPath = _pfn.strPackFullPath;
		strInternalPath = _pfn.strInternalPath;
		file_size		= _pfn.file_size;
		data_absolute_offset = _pfn.data_absolute_offset;
		chCompressionFlag = _pfn.chCompressionFlag;
	}

	// deallocates the files extracted to memory
	virtual ~pack_file_node()
	{
		destroy();
	}

	void destroy()
	{
		pBuffer.reset();
	}
	// NOT WRITTEN: opens the parent pack file and seeks to the position of the file, ready to read (into derived types)
	 
	//memory_file* readFile();

	// extracts the file
	bool extractFile(const std::string& dir);
	const uint8_t* extractFileToMemory(bool _bStorePtr = false);
	
	
	//bool comparePath(const std::string& _strPath);

	//string strInternalFileName = "";
	string strPackFullPath = "";
	string strInternalPath = "";
	//string strExtension = "";
	uint32_t file_size = 0;
	uint32_t data_absolute_offset = 0;
	//uint32_t data_relative_offset = 0;

	//string strFormatId = "";
	//string strSecondaryId = "";
	   
	PACK_FILE_HEADER header = {};
	uint32_t uiHeaderLength = 0;
	
	shared_ptr<uint8_t> pBuffer = nullptr;
	
	vector<uint8_t> outBuf;


	uint8_t chCompressionFlag = false;	

	uint8_t chErrorFlags = 0;
	

private:
	vector<const char* > m_vecMemoryExtracts;
};



struct cache_file_header
{
	union {
		char szID[4] = { 'n', 'o' ,'n', 'e' };
		uint32_t uiId;
	};
	
	uint16_t extentension_string_lengtht;
	string extenstion = "";

	uint32_t number_of_files = 0;
	
};

struct cache_file_info
{
	uint16_t internal_path_string_lengtht;
	string strInternalPath = "";
	
	uint16_t pack_file_string_lengtht;
	string strPackFile = "";

	uint32_t absolute_offset = 0;
	uint32_t file_size = 0;
	
};

typedef pack_file_node* ppfn;
