//#pragma once
//#include <algorithm>
//#include <Windows.h>
//#include "pack_file_structs.h"
////#include "anim_file.h"
////#include <SystemLib::DataStream.h>
////#include <thread.h>
//#include "..\SystemLib\path_node.h"
////
////#include "..\7z\lzma.h"
//
//
//#define PACK_ERROR_EOF 0b1
//#define PACK_ERROR_UNKNOWn 0b10000000
//
//#include "..\SystemLib\data_stream.h"
//
//extern path_node* g_poTreeAll;
//
//using namespace std;
//
//struct pack_flags {
//
//	/// This represents the bitmasks a PackFile can have applied to his type.
//	///
//	/// Keep in mind that this lib supports decoding PackFiles with any of these flags enabled,
//	/// but it only supports enconding for the `HAS_INDEX_WITH_TIMESTAMPS` flag.
//	struct PFHFlags {
//
//		/// Used to specify that the header of the PackFile is extended by 20 bytes. Used in Arena.
//		static constexpr uint32_t HAS_EXTENDED_HEADER = 0b0000000100000000;
//
//		/// Used to specify that the PackedFile Index is encrypted. Used in Arena.
//		static constexpr uint32_t HAS_ENCRYPTED_INDEX = 0b0000000010000000;
//
//		/// Used to specify that the PackedFile Index contains a timestamp of every PackFile.
//		static constexpr uint32_t HAS_INDEX_WITH_TIMESTAMPS = 0b0000000001000000;
//
//		/// Used to specify that the PackedFile's data is encrypted. Seen in `music.pack` PackFiles and in Arena.
//		static constexpr uint32_t HAS_ENCRYPTED_DATA = 0b0000000000010000;
//	};
//};
//
//
//
//
//enum pack_type : uint32_t
//{
//	boot = 0,
//	release = 1,
//	patch = 2,
//	mod = 3,
//	movie = 4
//};
//
//
//
//class pack_file;
//
//
//
//struct packed_file_info_node
//{
//	string strParentPack;
//	string strInternalPath;
//	uint32_t size;
//	uint32_t offset;
//};
//
//
//class test : public vector<pack_file_node*>
//{
//public:
//	bool extractAll(const std::string& _strPath);
//
//
//};
//
//
//struct pack_flaghs {
//
//	/// Used to specify that the header of the PackFile is extended by 20 bytes. Used in Arena.
//	static constexpr uint32_t HAS_EXTENDED_HEADER = 0b0000000100000000;
//
//	/// Used to specify that the PackedFile Index is encrypted. Used in Arena.
//	static constexpr uint32_t HAS_ENCRYPTED_INDEX = 0b0000000010000000;
//
//	/// Used to specify that the PackedFile Index contains a timestamp of every PackFile.
//	static constexpr uint32_t HAS_INDEX_WITH_TIMESTAMPS = 0b0000000001000000;
//
//	/// Used to specify that the PackedFile's data is encrypted. Seen in `music.pack` PackFiles and in Arena.
//	static constexpr uint32_t HAS_ENCRYPTED_DATA = 0b0000000000010000;
//};
//
//
//
//
//#define  MAKE_ID(ch0, ch1, ch2, ch3)                              \
//                ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |       \
//                ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
//
//
//struct PACK_FILE_INO
//{
//
//	uint32_t uiPackId = 0;
//	uint32_t uiFileCount = 0;
//	uint32_t uiVersion = 0;
//
//
//};
//
//
//
//struct PACK_FILE_INFO1
//{
//	/*
//	bool copyDataToFile(void* pvSrc, uint32_t len)
//	{
//		if (len > uiFileSize || pFileData != nullptr )
//			return false;
//
//		asm_mem_copy(pFileData, pvSrc, len);
//
//		return true;
//
//	}*/
//
//	char szPath[MAX_PATH];
//	uint32_t uiFileSize;
//	uint32_t uiDataStartAbs;
//	uint32_t uiDataStartRelative;
//
//
//};
//
//
////struct
////{
////	pack_file_node RMV2_File;
////	pack_file_node ANIM_SKeleton_File¨;
////	pack_file_node ANIM_Animation_File¨;
////	pack_file_node MAtrices_File;
////
////
////
////
////
////
////};
////
//
//struct pack_file_data
//{
//	uint32_t uiHeaderLength = 0;
//	PACK_FILE_HEADER oHeader = { 0 };
//
//	vector<string> m_strIndexStrings;
//
//};
//
//struct pack_file_package
//{
//	void setFolder(const std::string&);
//	void addPackFile(const std::string&);
//
//
//	string strFolder;
//	vector<string> vecPackFile;
//};
//
//
//struct game_pack_list {
//
//	string strFolder = "";
//	vector<string> vecPackFiles;
//};
//
//struct pack_file_path_block
//{
//	uint32_t file_size = 0;
//	string strPath;
//};
//
//struct pack_file_data_block
//{
//	vector<uint8_t> vecData;
//
//};
//
//struct pack_file_block
//{
//	PACK_FILE_HEADER oHeader;
//	vector<string> vedIndexString;
//	vector<pack_file_path_block> vecSizeAndPath;
//	vector<pack_file_data_block> vecData;
//
//	void open_from_memory(SystemLib::DataStream& _in);
//	void save(SystemLib::DataStream& _out);
//	void insertFile(const std::string&, uint8_t* _pBuffer, uint32_t file_size);
//};
//
//
//class pack_file :
//	public SystemLib::DataStream
//{
//public:
//	static vector<packed_file_info_node*> sm_vecFilesInAllPacks;
//
//	static void createAll();
//	static void destrouyAll();
//	static pack_file* sm_poAll;
//
//	static path_node* sm_poPathTree; // TODO re-enable
//
//
//	pack_file();
//	virtual ~pack_file();
//
//	PACK_FILE_HEADER header;
//	uint32_t uiHeaderLength = 0;
//
//	vector<string> m_strIndexStrings;
//
//	vector<uint8_t> decrypt_packed_file(vector<uint8_t>& ciphertext);
//
//	//	virtual ~pack_file();
////protected:
//	bool readPackFile(const std::string&);
//	//virtual bool clear();
//
//	//bool getNextFile_INIT(vector<string> _strDataDir, const std::string& _strExtension = "", bool bEqualToExension = false,
//	//	vector<string> _vecSubstr = { "" }, bool bEqualToSubstrict = false, const std::string& _strFormatId = "", const std::string& _strSecondryId = "");
//
//	//bool m_nitHadRun = false;
//	//bool getNextPackFiles(uint32_t num);
//	//bool getNexAnimFile(const std::string& _strFomratId, const std::string& _strSecondaryId, uint32_t num = 1);
//
//	//// allocated new mememory, loads the files into it and return a vector pair of size and pointer
//	//const vector<pair<uint32_t, char*>>& loadFilesIntoMemory(vector<pack_file_node*>& _vecFiles);
//
//	//bool makeFileSystemInMemory();
//	//// Opens pack file and returns type bits
//	//uint32_t getReadHeaderAndGetPackType(const std::string& _strFileName);
//
//	//// Retrurn type bits on already open file
//	//uint32_t getReadHeaderAndGetPackType();
//
//	//bool writeCacheFile(const std::string& extension, const std::string& strFolder);
//
//	//bool writeCacheFileFolder();
//
//	//bool writeCacheFileAll();
//
//
//
//
//	//const SystemLib::DataStream* readFile(const std::string& _strFileName);
//
//	//const pack_file_node* getFileInfo(const std::string& _strFileName);
//
//	//bool readAllCache(const std::string& _strDataDir, const vector<string>& _strSubString,
//	//	const std::string& _strFormat, const std::string& _strSecondaryId);
//
//	////bool readAll(vector<string> _strDataDir, const std::string& _strExtension, vector<string> _strSubString, const std::string& _strFormatId, const std::string& _strSecondryId);
//	//bool isFinished();
//
//	//window* pWin;
//
//	////virtual bool readPackInfo(vector<string>& _veconst stringPaths, const std::string&);
//	////virtual bool readPackInfoCache2(const std::string&);
//
//	//static game_pack_list sm_packfilesRome2;
//
//	//string m_message = "";
//
//
//
//	//bool readPackFile(const std::string&);
//
//
//
//
//	//bool readPackFileCache();
//
//
//	//virtual bool close() override;
//
//	//virtual uint32_t getNumberOfElements() override;
//
//	//const vector<pack_file_node* >& searchList(const vector<pack_file_node* >& _vecSrc,
//	//	const std::string& _strSearchString);
//
//	//// searches for file name in all the fles in the string vector, returns an empty vector if one found
//	//// more than one file can be found, and will be stored in the return vector
//	//const vector<pack_file_node>& searchForPath(const std::string& _strFileName, const vector<string>& _vecPacksToSearch);
//
//	//const vector<pack_file_node* >& getMatchingFiles(const std::string& _strFomratId, const std::string& _SecondaryId);
//
//
//	//const vector<pair<string, uint32_t>>& getAllFileNamesAndSizes();
//
//	//bool readAllFileHeaders(const std::string&, const std::string&);
//
//
//	//thread_class* getThread();
//	//bool m_bFinished = false;
//
//
//
//	//uint32_t getNumberOfFIles(const vector<string>& _vecPacksToSearch);
//
//	//bool extractFiles(const std::string& _strPath);
//
//protected:
//	virtual bool readHeader();
//	//virtual bool readHeaderCache();
//
//	//// reads all the string and their offset info, if substring is set it will only include files with that substring
//	//bool readFileList(const std::string& _strFileName);
//	bool readFileListQuick(const std::string& _strFileName);
//
//
//	/*bool readFileListCache(const std::string& _strFileName);
//
//
//	bool readFileList_PFH5(const std::string& _strFileName);*/
//
//public:
//	bool readAllPackFilesInPath(const wstring& _strFileName);
//
//	bool readAllPackFilesInConfigPaths();
//
////public:
////	vector<pack_file_node*>& searchForPath(const std::string& _strFilePath);
////	vector<pack_file_node*>& searchForFile(const std::string& _strFileName);
////	vector<pack_file_node*>& searchForExt(const std::string& _strExt);
////	vector<pack_file_node*>& searchForPartialPath(const std::string& _strExt);
////	vector<pack_file_node*>& searchForSubStringAndExtention(const std::string& _strSubString, const std::string& _strExtention);
////
////
//protected:
//	uint32_t m_vector_index = 0;
//	uint32_t m_header_length = 0;
//	ifstream ifsInputFile;
//	DataStream oInputFile;
//
//
////	bool readPackFileListCache(const std::string& _const stringFileName);
////
////	const PACK_FILE_HEADER& getHeader(const std::string& strPath);
////public:
////	mutex _getCurrentPackFileName_Mutex;
////	const std::string& getCurrentPackFileName();
//
//	string m_strSearchExtension = "";
//
//	string m_strExtension = "";
//	string m_strPackFileFolder = "";
//	string m_strFileName = "";
//	//vector<string> m_veconst stringTempSubstr;
//	bool m_bEqualToSubstring = false;
//	bool m_bEqualToExtension = false;
//
//	bool findValidPackFiles(const wstring& _strDir, const vector<uint32_t>& m_vecPackTypes = { pack_type::release });
//
//public:
//	static constexpr uint32_t makeID(const char _csz[4]);
//
//	static constexpr uint32_t getHeaderLength(const char _szID[4]);
//protected:
//
//	/*thread_class* m_thread_task = nullptr;*/
//
//
//	vector<pack_file_node*> m_vecInternalFileList;
//
//	vector<pack_file_node*> vecFileInfoReturn;
//
//
//	vector<pack_file_node> m_vecTempPACK_FILE_INFO;
//
//	vector<pack_file_node*> m_vecSearchFileList;
//
//	vector<pack_file_node*> m_vecPackFileList;
//
//	PACK_FILE_HEADER m_oTempHeader;
//
//	pack_file_data m_oAnimFile;
//	string m_strFileNameTemp = "";
//
//	pack_file_node* m_poNodeTemp = nullptr;
//
//	std::string m_strSearchSubString = "";
//	uint32_t m_dwMemoryUsed;
//
//
//
//	uint32_t m_index = 0;
//	uint32_t m_i = 0;
//
//
//	uint32_t m_file_list_index = 0;
//
//
//	uint32_t m_packed_files_total = 0;
//	uint32_t m_files_completed = 0;
//	string m_strCurrentFile = "";
//
//
//	uint32_t m_pack_files_total = 0;
//	uint32_t m_pack_files_completed = 0;
//	string m_strCurrentPackFileTemp = "";
//
//	bool m_bFileReadingComplete = false;
//	bool m_bPackReadingComplete = false;
//	bool m_bFileFound = false;
//
//
//
//	vector<pack_file_node* > m_vecTempList;
//};
//
//
