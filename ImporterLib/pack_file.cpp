#include "pack_file.h"
#include "pack_file_structs.h"

#include <assert.h>

#include <QtCore\qstring.h>

#include <windows.h>
#include <algorithm>

//#include "..\QtRMV2Widget\QConsole.h"
#include "..\SystemLib\tools.h"
#include "..\SystemLib\system_clock.h"

//#define CondRet(func) if (!##func())	return false;

#include "lzma_file.h"

void pack_file::createAll()
{
	// already created;
	if (sm_poAll)
	{
		delete sm_poAll;
	}
	sm_poAll = new pack_file;

	sm_poAll->readAllPackFilesInConfigPaths();
}

void pack_file::destrouyAll()
{
	// TODO: re-enable
	if (sm_poAll)
	{
		for (auto& it : sm_poAll->m_vecInternalFileList)
		{
			delete it;
		}
		sm_poAll->sm_vecFilesInAllPacks.clear();

		delete sm_poAll;
		sm_poAll = nullptr;
	}
}

pack_file::pack_file()
{
}

pack_file::~pack_file()
{
	for (auto& it : m_vecInternalFileList)
	{
		delete it;
	}
	m_vecInternalFileList.clear();
}

bool comp(pack_file_node* i, pack_file_node* j) { return (i->strInternalPath > j->strInternalPath); }

bool pack_file::readAllPackFilesInConfigPaths()
{
	///*system_clock::init();
	//system_clock clock;
	//clock.resetLocalTime();*/

	//vector<string> vecPaths;
	//if (main_process::sm_oConfigFile.getValueCount("data_paths") == 0)
	//	return false;

	vector<wstring> vecPaths = { LR"(F:\SteamLibrary\steamapps\common\Total War WARHAMMER II\data)" };

	//find pack files
	for (size_t i = 0; i < vecPaths.size(); i++)
	{
		if (!findValidPackFiles(vecPaths[i]))
			return false;
	}

	//for (auto it = m_vecPackFileList.rbegin(); it != m_vecPackFileList.rend(); ++it)
	//{
	//	this->readPackFileQuick((*it)->strPackFullPath);

	//}
	// _log_action(".pack files being sorted...");
	std::sort(m_vecPackFileList.begin(), m_vecPackFileList.end(), comp);

	// _log_action(".pack files is sorted in the following order: ");

	int i = 1;
	for (auto it : m_vecPackFileList)
	{
		// _log_action("[PACK FILE] " + to_string(i++) + " : " + it->strInternalPath);
	}

	m_packed_files_total = 0;

	SystemLib::system_clock oClock;
	// _log_action("Start Parsing of Paths in .pack files...");
	for (auto it : m_vecPackFileList)
	{
		readPackFile(it->strPackFullPath);
	}

	volatile double time = oClock.getLocalTime();
	// _log_action("Parsed " + to_string(m_packed_files_total) + " path entries.");
	// _log_action("Parsing took: " + to_string(time) + " sec.");

	volatile int debug_break_1 = 1;
	m_bPackReadingComplete = true;
	m_bFileReadingComplete = false;
	m_index = 0;

	// _log_action("Internal file indexes: " + to_string(m_packed_files_total));
//volatile double time_passed = clock.getLocalTime();

	return true;
}

bool pack_file::findValidPackFiles(const wstring& _strDir, const vector<uint32_t>& _vecPackTypes)
{
	wstring strTemp = _strDir + L"\\" + wstring(L"*.pack");

	QString qstr = QString::fromStdWString(strTemp.c_str());

	WIN32_FIND_DATAW oFindData;

	HANDLE hFile = FindFirstFile(qstr.toStdWString().c_str(), &oFindData);

	if (hFile == INVALID_HANDLE_VALUE) // if not "first file" found, no .pack in this folder
	{
		string msg = "Function: findValidPakFiles() : File Not Found.Content:\r\n '";
		msg += QString::fromStdWString(_strDir.c_str()).toStdString().c_str(); +"' \r\nPossibly invalid \\ data \\ folder set.";

		// // _log_action_error(msg);

		return false;
	}
	else
		do {
			//SystemLib::DataStream oMemFile;
			pack_file_node* po = new pack_file_node;

			QString qstrTemp = QString::fromStdWString(_strDir + L"\\" + oFindData.cFileName);

			//oMemFile = new memory_file;
			ifstream file(qstrTemp.toStdString());

			//bool b = il

			//fstream file(strTemp);

			// read the header of the .packs to check if they are of the wanted type
			file.read((char*)&(*po).header.oHeaderId.sz, 4);
			(*po).uiHeaderLength = getHeaderLength((*po).header.oHeaderId.sz);

			file.read((char*)&(*po).header.dwTypeFlags, (*po).uiHeaderLength - 4);
			file.close();
			//oMemFile.m_dwBufferOffset = oMemFile.m_fsFile.tellg();
			//oMemFile.m_bInMemory = false;
			po->strPackFullPath = qstrTemp.toStdString();

			po->strInternalPath = tools::GetFileFromFullPath(qstrTemp.toStdString());

			/*if ((*po).header.dwType == pack_type::release ||
				(*po).header.dwType == pack_type::boot ||
				(*po).header.dwType == pack_type::patch

				) // || (*po).header.dwType == pack_type::patch)*/

				//po->strPackFullPath = tools::GetFileFromFullPath(po->strPackFullPath.c_str());

			m_vecPackFileList.push_back(po); // LEAK: **** Possible mem leak  **** are the "po"s ever deleted again?
			//sort(m_vecPackFileList.begin(), m_vecPackFileList.end());
			//m_dwMemoryUsed += sizeof(strTemp);
			//oMemFile.close();
			//delete po;
		} while (FindNextFile(hFile, &oFindData));

		return true;
}

bool pack_file::readPackFile(const std::string& _strFileName)
{
	// read header
	// read all path data into memory
	// parse path data into file list "pack_file_node"
	/*if (!open(_strFileName, 48))
		return false;*/

	ifsInputFile.open(_strFileName, ios::binary);

	m_strFileNameTemp = _strFileName;
	m_pack_files_total = m_vecPackFileList.size();

	if (!readHeader())
	{
		//window::_getOKErrorBox("An error has occured ");
		return false;
	}
	ifsInputFile.close();

	m_vector_index = 0;
	m_header_length = m_oAnimFile.uiHeaderLength;

	oInputFile.open(_strFileName, m_oAnimFile.oHeader.iDataStart + m_oAnimFile.uiHeaderLength);
	oInputFile.seek_absolute(m_oAnimFile.uiHeaderLength);
	//// allocate enough memory to hold the file list
	//open(	oFile.oHeader.iDataStart + oFile.uiHeaderLength];

	//// read the entire file list into memory
	//this->read(m_pFileBuffer, oFile.oHeader.iDataStart + oFile.uiHeaderLength);

	//// clsoe disk file
	//m_ifsFile.close();

	// switch the file object to be memory based
	//this->m_bInMemory = true;

	/*if (!open(_strFileName, oFile.oHeader.iDataStart))
		return false;  */

		//oInputFile.seekg(oFile.uiHeaderLength);

		//	log_file.writeLn("Reading .pack file: '" + _strFileName + " ' ");

			// read file list
	readFileListQuick(_strFileName);

	//	log_file.write(" -- SUCESS!");

		//delete[] this->m_pFileBuffer;

	ifsInputFile.close();
	return true;
}

bool pack_file::readFileListQuick(const std::string& _strFileName)
{
	//m_strCurrentPackFileTemp = _strFileName;
	uint32_t m_offset = m_oAnimFile.uiHeaderLength;
	m_strIndexStrings.clear();

	char szPath[MAX_PATH * 4];
	char* PszString[30];

	for (size_t i = 0; i < m_oAnimFile.oHeader.dwPackileIndexCount; i++)
	{
		char chIn = '\0';
		string strPackFileName = "";
		do
		{
			oInputFile.readMem(&chIn, 1);
			strPackFileName += chIn;
			m_offset++;
		} while (chIn != '\0');

		m_strIndexStrings.push_back(strPackFileName);
		//m_offset += strPackFileName.size();
		//m_offset += strPackFileName.size();
	}

	//char szPath[256];

	//bool bFilesWithExt = false;

	//size_t vec_offset = m_vecInternalFileList.size();
	//m_vecInternalFileList.resize(vec_offset + (size_t)oFile.oHeader.uiFileCount);

	for (m_file_list_index = 0; m_file_list_index < m_oAnimFile.oHeader.uiFileCount/* && !m_bFileFound*/; m_file_list_index++)
	{
		auto poPFN = new pack_file_node;

		// read file size from file
		oInputFile.readMem((char*)&poPFN->file_size, 4);

		// read compression flag byte, if a

		uint32_t shader_stuff;
		uint32_t compression_stuff;
		if (m_oAnimFile.oHeader.oHeaderId.dw == pack_file::makeID("PFH5"))
		{
			oInputFile.readMem((char*)&poPFN->chCompressionFlag, 1);

			if (poPFN->chCompressionFlag != 0)
				int a = 1;
		}

		if (m_oAnimFile.oHeader.dwTypeFlags & 0b1000000)
			oInputFile.readMem((char*)&shader_stuff, 4);

		// save the absolute offset
		poPFN->data_absolute_offset = m_offset + m_oAnimFile.oHeader.iDataStart;

		// Save the offset for the files
		// add the file size, so next file will get the right offset
		m_offset += poPFN->file_size;

		size_t ptr_index = 0;
		size_t str_index = 0;
		size_t index = 0;
		//memset(szPath, 0, 256);
	//	memset(PszString, 0, 256*sizeof(char*));

		//string strPath = "";
		do
		{
			//char ch;

			oInputFile.readMem(&szPath[index], 1);

			//strPath += ch;
			//
			//if (ch == '\0')
			//	break;
			//'index++;

			//szPath[index] = tolower(szPath[index]);

			//strPath += szPath[index];

			if (szPath[index] == '\0')
			{
				PszString[ptr_index] = &szPath[str_index];
				//PszString[ptr_index + 1] = nullptr;
				break;
			}

			if (szPath[index] == '\\')
			{
				szPath[index] = '\0'; // divide the string by inserting a 0 terminator

				PszString[ptr_index] = &szPath[str_index];

				str_index = index + 1; // "start" point to the first char after the inserted '\0'

				ptr_index++; // next item in the pointer list
			}
			//	index++;
		} while (szPath[index++] != '/0');

		//bool bOverWriteAndOverWrritten = true;
		//if (this->oFile.oHeader.dwType == pack_type::mod || this->oFile.oHeader.dwType == pack_type::movie)
		//	bOverWrite = true;

		path_node* poPathNode = sm_poPathTree->setData(
			PszString,
			ptr_index + 1,
			shared_ptr<pack_file_node>(poPFN),
			true
		);

		//		path_node* poPathNode = sm_poPathTree->setData(strPath, shared_ptr<pack_file_node>(po));

				/*if (oFile.oHeader.dwTypeFlags == pack_type::mod || bOverWriteAndOverWrritten)
					poPathNode->bIsModded = true;

				if (!poPathNode->bHasData)
					int i = 1;*/

		for (size_t i = 0; i < index; i++)
		{
			if (szPath[i] == '\0')
				szPath[i] = '\\';
		}

		poPFN->strPackFullPath = _strFileName;
		//po->strInternalPath = tools::GetFileFromFullPath(szPath);
		poPFN->strInternalPath = szPath;

		m_packed_files_total++;

		//po->strExtension = tools::GetExtensionFromFullPath(po->strInternalPath.c_str());

		//bool bResult = sm_oPathTree->setData(szPath, po);
		//pack_file_node* pDataNode = sm_oPathTree->getData(szPath);
		//m_vecFileInfo.push_back(po);

		// -- TODO: REMOVED LINEAR PACK BUFFER ?
		//m_vecInternalFileList[m_file_list_index + vec_offset] = po;

		//if (po->chCompressionFlag == 1)
		//{
		//	lzma_file oLZMAFile;

		//	oLZMAFile.decompressToMemory(po, "");
		//}
	}

	return true;
}

//bool pack_file::readFileListQuick(const std::string& _strFileName)
//{
//	m_strCurrentPackFileTemp = _strFileName;
//	uint32_t m_offset = oFile.uiHeaderLength;
//	m_strIndexStrings.clear();
//
//
//	for (size_t i = 0; i < oFile.oHeader.dwPackileIndexCount; i++)
//	{
//		char chIn = '\0';
//		string strPackFileName = "";
//		do
//		{
//			read(&chIn, 1);
//			strPackFileName += chIn;
//			m_offset++;
//
//		} while (chIn != '\0');
//
//		m_strIndexStrings.push_back(strPackFileName);
//		//m_offset += strPackFileName.size();
//		//m_offset += strPackFileName.size();
//	}
//
//	char szPath[1024];
//	char* PszString[1024];
//
//
//	bool bFilesWithExt = false;
//
//	uint32_t vec_offset = m_vecInternalFileList.size();
//	m_vecInternalFileList.resize(vec_offset + oFile.oHeader.uiFileCount);
//
//	for (m_file_list_index = 0; m_file_list_index < oFile.oHeader.uiFileCount/* && !m_bFileFound*/; m_file_list_index++)
//	{
//
//		// ****EXIT - *******
//		/*if (m_poParentThread)
//			if (m_poParentThread->endRequested())
//			{
//				return false;
//			}*/
//
//
//
//
//
//
//		pack_file_node* po = new pack_file_node;
//		//po->strParentFile =  _strFileName;
//		//po->strParentFolder = m_strPackFileFolder;
//
//
//		//if (po->strParentFolder[po->strParentFolder.size()] != '\\')
//		//	po->strParentFolder += '\\';
//
//
//		//po->strPackFullPath = po->strParentFolder +tools::GetFileFromFullPath(po->strParentFile.c_str());
//
//		uint32_t index = 0;
//
//		// read file size from file
//		read(&po->file_size, 4);
//
//		// read compression flag byte, if a
//
//		uint32_t shader_stuff;
//		if (oFile.oHeader.oHeaderId.dw == pack_file::makeID("PFH5"))
//		{
//			read(&po->chCompressionFlag, 1);
//
//			if (po->chCompressionFlag == 0)
//				int a = 1;
//		}
//		else
//
//			if (oFile.oHeader.dwType & 0b1000000)
//				read(&shader_stuff, 4);
//
//		po->data_absolute_offset = m_offset + oFile.oHeader.iDataStart;
//		// save the absolute offset
//
//		// Save the offset for the files
//		// add the file size, so next file will get the right offset
//		m_offset += po->file_size;
//
//
//
//		uint32_t ptr_index = 0;
//		uint32_t str_index = 0;
//		index = 0;
//		memset(szPath, 0, 1024);
//		while (1)
//		{
//			read(&szPath[index], 1);
//			szPath[index] = tolower(szPath[index]);
//
//			if (szPath[index] == '\0')
//			{
//				PszString[ptr_index] = &szPath[str_index];
//				break;
//			}
//
//			if (szPath[index] == '\\')
//			{
//				szPath[index] = '\0'; // divide the string by inserting a 0 terminator
//
//				PszString[ptr_index] = &szPath[str_index];
//
//				str_index = index + 1; // "start" point to the first char after the inserted '\0'
//
//				ptr_index++; // next item in the pointer list
//
//			}
//			index++;
//		}
//
//		bool bOverWrite = true;
//		//if (this->oFile.oHeader.dwType == pack_type::mod || this->oFile.oHeader.dwType == pack_type::movie)
//		//	bOverWrite = true;
//
//
//
//		//bool bResult = sm_poPathTree->setData(PszString, ((uint32_t)ptr_index + 1), po, bOverWrite);
//
//		for (size_t i = 0; i < index; i++)
//		{
//			if (szPath[i] == '\0')
//				szPath[i] = '\\';
//		}
//
//
//
//
//		po->strPackFullPath = _strFileName;
//		po->strInternalPath = tools::GetFileFromFullPath(szPath);
//		po->strInternalPath = szPath;
//
//		po->strExtension = tools::GetExtensionFromFullPath(po->strInternalPath);
//
//
//		bool bResult = sm_poPathTree->setData(szPath, po);
////		pack_file_node* pDataNode = sm_poPathTree->getPath(szPath);
//
//		//m_vecFileInfo.push_back(po);
//		m_vecInternalFileList[m_file_list_index + vec_offset] = po;
//
//
//		//if (po->chCompressionFlag == 1)
//		//{
//		//	lzma_file oLZMAFile;
//
//		//	oLZMAFile.decompressToMemory(po, "");
//		//}
//
//
//	}
//	return true;
//}

constexpr uint32_t pack_file::makeID(const char _csz[4])
{
	return ((uint32_t)(uint8_t)(_csz[0]) | ((uint32_t)(uint8_t)(_csz[1]) << 8) |
		((uint32_t)(uint8_t)(_csz[2]) << 16) | ((uint32_t)(uint8_t)(_csz[3]) << 24));
}

constexpr uint32_t pack_file::getHeaderLength(const char _szID[4])
{
	uint32_t result = 0;
	//uint32_t id:;

	//if (!_szID )
	//	id = makeID(oFile.oHeader.sz[0], oFile.oHeader.sz[1], oFile.oHeader.sz[2], oFile.oHeader.sz[3]);
	//else
	//	id = makeID(_szID[0], _szID[1], _szID[2], _szID[3]);

	switch (makeID(_szID))
	{
		//case "PFH0":
	case makeID("PFH0"):
		result = 24;
		break;

	case makeID("PFH2"):
	case makeID("PFH3"):
		// PFH2+ contain a FileTime at 0x1C (I think) in addition to PFH0's header
		result = 32;
		break;

	case makeID("PFH5"):
	case makeID("PFH4"):
		result = 28;;
		break;

	default:
		throw exception("Unknown .PACK file format");
		// if this ever happens, go have a word with MS
	}

	return result;
}

bool pack_file::readHeader() {
	ifsInputFile.read((char*)&m_oAnimFile.oHeader.oHeaderId.sz, 4);
	m_oAnimFile.uiHeaderLength = getHeaderLength(m_oAnimFile.oHeader.oHeaderId.sz);

	ifsInputFile.read((char*)&m_oAnimFile.oHeader.dwTypeFlags, m_oAnimFile.uiHeaderLength - 4);
	m_oAnimFile.oHeader.oHeaderId.dw = makeID(m_oAnimFile.oHeader.oHeaderId.sz);

	return true;

	//read(&oFile.oHeader.oHeaderId.sz, 4);
	//oFile.uiHeaderLength = getHeaderLength(oFile.oHeader.oHeaderId.sz);
	//read(&oFile.oHeader.dwType, oFile.uiHeaderLength - 4);

	//oFile.oHeader.oHeaderId.dw = makeID(oFile.oHeader.oHeaderId.sz);

	//return true;
}

bool pack_file::readAllPackFilesInPath(const wstring& _strPath)
{
	SystemLib::system_clock clock;
	clock.resetLocalTime();

	// find pack files
	vector<string> vecPaths;
	if (!findValidPackFiles(_strPath))
		return false;

	//	for (auto it = m_vecSearchFileList.rbegin(); it != m_vecPackFileList.rend(); it++)

			//for (auto it : m_vecFileInfo)
	for (auto it = m_vecSearchFileList.rbegin(); it != m_vecSearchFileList.rend(); ++it)
	{
		this->readPackFile((*it)->strInternalPath);
	}

	volatile double time_passed = clock.getLocalTime();

	return true;
}

//vector<uint8_t> pack_file::decrypt_packed_file(vector<uint8_t>& ciphertext)
//{
//	uint64_t DATA_KEY = 0x8FEB2A6740A6920E;
//
//	auto size = ciphertext.size();
//	auto padding = 8 - (size % 8);
//	if (padding < 8) ciphertext.resize(size + padding, 0);
//
//
//	vector<uint8_t> plaintext;
//	plaintext.resize(ciphertext.size());
//
//	uint32_t edi = 0;
//
//	for (uint32_t i = 0; i < ciphertext.size() / 8; i++)
//	{
//		uint64_t v = ciphertext.size() / 8;
//		uint64_t u = edi;
//
//		uint64_t rod = (DATA_KEY * tools::overflow_wrap(~u));
//
//
//		uint64_t esi = sizeof(edi);
//		uint64_t prod = 0;
//
//
//		{
//			uint64_t l = 0;
//			l |= ((uint64_t)ciphertext[esi + 0] << 0);
//			l |= ((uint64_t)ciphertext[esi + 1] << 8);
//			l |= ((uint64_t)ciphertext[esi + 2] << 16);
//			l |= ((uint64_t)ciphertext[esi + 3] << 24);
//			l |= ((uint64_t)ciphertext[esi + 4] << 32);
//			l |= ((uint64_t)ciphertext[esi + 5] << 40);
//			l |= ((uint64_t)ciphertext[esi + 6] << 48);
//			l |= ((uint64_t)ciphertext[esi + 7] << 56);
//
//			prod ^= l;
//		}
//		uint8_t* p = (uint8_t*)&prod;
//
//		uint64_t test = 0xFFFFFFFFFFFFFFFF;
//		vector<uint8_t> vec1;
//		vec1.resize(10);
//		::memcpy(&vec1[0], &test, 8);
//
//		for (size_t b = 0; b < 8; b++)
//			plaintext[(i * 8 + b)] = p[b];
//	}
//
//	edi += 8;
//
//
//	plaintext.resize(size);
//
//	return plaintext;
//
//
//}

pack_file* pack_file::sm_poAll = nullptr;
vector<packed_file_info_node*> pack_file::sm_vecFilesInAllPacks;
path_node* pack_file::sm_poPathTree = path_node::createRoot("INTERNAL_FOLDERS");

path_node* g_poTreeAll = pack_file::sm_poPathTree;

//bool pack_file::clear()
//{
//	/*for (size_t i = 0; i < m_vecSearchFileList.size(); i++)
//	{
//		delete m_vecSearchFileList[i];
//	}
//	m_vecSearchFileList.clear();
//
//	for (size_t i = 0; i < m_vecPackFileList.size(); i++)
//	{
//		delete m_vecPackFileList[i];
//	}
//	m_vecPackFileList.clear();*/
//
//	for (size_t i = 0; i < m_vecFileInfo.size(); i++)
//	{
//		delete m_vecFileInfo[i];
//	}
//	m_vecFileInfo.clear();
//
//	m_files_completed = 0;
//	m_files_total = 0;
//	m_pack_files_completed = 0;
//	m_pack_files_total = 0;
//
//	return true;
//}
//
//
//
//bool pack_file::getNextFile_INIT(vector<string> _strDataDir, const std::string& _strExtension, bool _bEqualToExension,
//	vector<string> _vecSubstr, bool _bEqualToSubstring, const std::string& _strFormatId, const std::string& _strSecondryId)
//{
//	m_vecFileInfo.clear();
//	m_vecPackFileList.clear();
//	m_vecSearchFileList.clear();
//	m_vecstrTempSubstr.clear();
//
//	m_strFormatId = _strFormatId;
//	m_strSecondaryId = _strSecondryId;
//
//	m_strSearchExtension = tools::lower(_strExtension);
//	m_vecstrTempSubstr = _vecSubstr;
//
//	for (auto it : _vecSubstr)
//		it = tools::lower(it);
//
//	m_pack_files_completed = 0;
//	m_pack_files_total = 0;
//	m_files_completed = 0;
//	m_files_total = 0;
//	m_bPackReadingComplete = false;
//	m_bEqualToSubstring = _bEqualToSubstring;
//	m_bEqualToExtension = _bEqualToExension;
//	m_bFileFound = false;
//
//
//	m_strSearchSubString = "";
//
//	// is a set of dirs supplied, if not use the config file
//	vector<string> vecPaths;
//	if (_strDataDir.size() == 0)
//	{
//		if (main_process::sm_oConfigFile.getValueCount("data_paths") == 0)
//			return false;
//
//		vecPaths = main_process::sm_oConfigFile.getStringValues("data_paths");
//	}
//	else
//		vecPaths = _strDataDir;
//
//	// find pack files
//	for (size_t i = 0; i < vecPaths.size(); i++)
//	{
//		if (!findValidPackFiles(vecPaths[i]))
//			return false;
//	}
//
//	m_bPackReadingComplete = true;
//
//	m_bFileReadingComplete = false;
//	m_index = 0;
//
//	return true;
//
//}
//
//bool pack_file::getNextPackFiles(uint32_t num)
//{
//
//	m_strFileName = tools::GetFileFromFullPath(m_strFileNameTemp.c_str());
//	m_strPackFileFolder = m_vecPackFileList[m_index]->strPackFullPath;
//
//	m_strFileNameTemp = m_vecPackFileList[m_index]->strPackFullPath;
//
//	m_strCurrentPackFileTemp = tools::GetFileFromFullPath(m_strFileNameTemp.c_str());
//	readPackFile(m_strFileNameTemp);
//
//	m_pack_files_completed++;
//	//m_strCurrentPackFile = tools::GetFileFromFullPath(m_strFileName.c_str());
//
//	close();
//
//	m_index++;
//
//
//	if (m_index < m_vecPackFileList.size())
//		return true;
//	else
//		return false;
//}
//
//bool pack_file::getNexAnimFile(cstr& _strFomratId, cstr& _strSecondaryId, uint32_t num)
//{
//
//
//	//for (size_t i = 0; i < num; i++)
//	{
//		if (m_index >= m_vecFileInfo.size())
//		{
//
//			m_oAnimFile.m_strSecondaryId = "test";
//			close();
//			m_oAnimFile.close();
//			return false;
//
//
//		}
//		pack_file_node* poPack = m_vecFileInfo[m_index];
//
//
//		//if (poPack->strExtension == "anim")
//		{
//
//			//	m_message += p->strFileName;
//				//*pWin = m_message;
//
//			m_i++;
//			m_oAnimFile.open(poPack->strPackFullPath, "", poPack->data_absolute_offset, poPack->file_size, memory_usage::off);
//			m_oAnimFile.readHeaderInfo();
//
//
//
//			if (_strFomratId == m_oAnimFile.getFormaIdString() &&
//				_strSecondaryId == m_oAnimFile.getSecondaryIdString()
//				)
//			{
//				poPack->fFrameRate = m_oAnimFile.m_file.oHeader.fFrameRate;
//				poPack->fEndTime = m_oAnimFile.m_file.oHeader.fEndTime;
//				poPack->dwFrameCount = m_oAnimFile.m_file.dwFrameCount;
//
//				poPack->strFormatId = _strFomratId;
//				poPack->strSecondaryId = _strSecondaryId;
//				m_vecSearchFileList.push_back(poPack);
//
//
//
//
//			}
//			m_oAnimFile.close();
//			m_oAnimFile.clear();
//
//		}
//		m_index++;
//	}
//	m_files_completed = m_index;
//
//	//delete poANIM;
//
//
//	return true;
//}
//
//
//
//
//uint32_t pack_file::getReadHeaderAndGetPackType(const std::string& _strFileName)
//{
//
//	fstream file(_strFileName);
//
//	read(&oFile.oHeader.oHeaderId.sz, 4);
//	oFile.uiHeaderLength = getHeaderLength(oFile.oHeader.oHeaderId.sz);
//
//	read(&oFile.oHeader.dwType, oFile.uiHeaderLength - 4);
//
//
//	/*uint32_t dwTypeBits = 0;
//
//	oFile.seekg(4, ios_base::cur);
//	oFile.read((char*)& dwTypeBits, 4);
//
//
//	uint32_t test = dwTypeBits  &= 0B111;*/
//
//	return oFile.oHeader.dwType;
//
//}
//
//
//
//bool pack_file::writeCacheFile(const std::string& _strExtension, const std::string& _strFolder)
//{
//	getNextFile_INIT({ _strFolder }, _strExtension, true, {}, false);
//
//	system_clock::init();
//
//	float old_time = system_clock::getSeconds();
//
//	while (getNextPackFiles(23));
//
//	float time_passed = system_clock::getSeconds() - old_time;
//
//	pack_cache cache_file;
//
//
//
//
//	cache_file.write(m_vecFileInfo);
//
//
//
//
//	return true;
//
//}
//
//
//bool pack_file::writeCacheFileAll()
//{
//	return false;
//}
//
//const pack_file_node* pack_file::getFileInfo(const std::string& _strFileName)
//{
//
//	return NULL;
//}
//
//bool pack_file::readAllCache(const std::string& _strDataDir, const vector<string>& _vecstrSubString,
//	const std::string& _strFormat, const std::string& _strSecondaryId)
//{/*
//	clear();
//
//
//	m_strPackFileFolder = _strDataDir;
//
//	m_vecstrTempSubstr = _vecstrSubString, m_vecstrTempSubstr = _vecstrSubString,
//
//		readPackInfoCache(_strDataDir);
//
//	if (_strFormat != "" || _strSecondaryId != "")
//		getMatchingFiles(_strFormat, _strSecondaryId);
//
//	return true;
//
//}
//
//bool pack_file::readAll(vector<string> _strDataDir, const std::string& _strExtension, vector<string> _vecSubstr, const std::string& _strFormatId, const std::string& _strSecondryId)
//{
//
//
//	m_vecFileInfo.clear();
//	m_vecPackFileList.clear();
//	m_vecSearchFileList.clear();
//	m_vecstrTempSubstr.clear();
//	m_strExtension = "xxx111-dda";
//	m_vecstrTempSubstr = _vecSubstr,
//
//		m_strExtension = _strExtension;
//	m_pack_files_completed = 0;
//	m_pack_files_total = 0;
//	m_files_completed = 0;
//	m_files_total = 0;
//	m_bPackReadingComplete = false;
//
//
//
//	readPackInfo(_strDataDir, "");
//
//	m_bPackReadingComplete = true;
//
//	m_bFileReadingComplete = false;
//
//
//	//***** EXIT -*******
//	if (m_poParentThread)
//		if (m_poParentThread->endRequested())
//			return false;
//	//****** EXIT -*******
//
//
//	m_files_completed = 0;
//	m_files_total = m_vecFileInfo.size();;
//
//	if (_strFormatId != "" || _strSecondryId != "")
//		getMatchingFiles(_strFormatId, _strSecondryId);
//
//	m_bFileReadingComplete = true;
//
//	return true;
//}
//
//bool pack_file::readPackInfo(vector<string>& _vecstrPaths, const std::string& _strSubString)
//{
//	m_strSearchSubString = _strSubString;
//
//
//	findValidPackFiles(_vecstrPaths[0]);
//
//	m_pack_files_total = m_vecPackFileList.size();
//
//
//	//	 ******* EXIT -*******
//	if (m_poParentThread)
//		if (m_poParentThread->endRequested())
//			return false;
//	// -******* EXIT -*******
//
//	for (size_t i = 0; i < m_vecPackFileList.size(); i++)
//	{
//
//
//
//		m_strFileNameTemp = m_vecPackFileList[i]->strPackFullPath;
//
//		m_strCurrentPackFileTemp = tools::GetFileFromFullPath(m_strFileNameTemp.c_str());
//		readPackFile(m_strFileNameTemp);
//
//		m_pack_files_completed++;
//		//m_strCurrentPackFile = tools::GetFileFromFullPath(m_strFileName.c_str());
//
//		close();
//	}
//
//
//	/*THREAD_END;*/
//
//	/*m_thread_task = ret.first;
//*/
//
//	uint32_t u = m_dwMemoryUsed;
//
//
//	return true;
//}
//
////bool pack_file::readPackInfoCache(const std::string& _strPath)
////{
////
////	findValidPackFiles(_strPath);
////
////	readPackFileCache();
////
////
////
////	return true;
////
////
////}
//
//bool comp(pack_file_node* i, pack_file_node* j) { return (i->strInternalPath > j->strInternalPath); }
//
//
//
//
//bool pack_file::isFinished()
//{
//	return m_bFinished;
//}
//
//bool pack_file::readPackFile(const std::string& _strFileName)
//{
//	// if no the file does not contain the specified extension
//
//	/*if (m_bEqualToExtension)
//	{
//		if (CONF.find("no_" + m_strSearchExtension, _strFileName) > 0)
//			return true;
//	}*/
//
//	if (!memory_file::open(_strFileName, memory_usage::off))
//		return false;
//
//	m_strFileNameTemp = _strFileName;
//
//	m_pack_files_total = m_vecPackFileList.size();
//
//	if (!readHeader())
//	{
//		window::_getOKErrorBox("An error has occured ");
//		return false;
//	}
//
//	m_vector_index = 0;
//	m_header_length = oFile.uiHeaderLength;
//
//	//case makeID("PFH2"):
//	//case makeID("PFH3"):
//		// PFH2+ contain a FileTime at 0x1C (I think) in addition to PFH0's header
//
//	// save the pack file index, if there is one
//
//
//
//	//if (makeID("PFH4") == oFile.oHeader.oHeaderId.dw)
//	readFileList(_strFileName);
//	/*else
//	if (makeID("PFH5") == oFile.oHeader.HeadderId.dw)
//		readFileList_PFH5(_strFileName);*/
//
//
//		// what kind of pack file header is it?
//	   /*switch (getHeaderLength(oFile.oHeader.oHeaderId.sz))
//	   {
//	   case makeID("PFH4"):readFileList_PFH4(_strFileName);
//	   break;
//
//	   case makeID("PFH5"):readFileList_PFH4(_strFileName);
//	   break;
//
//
//		   default:
//			   readFileList_PFH4(_strFileName);
//
//	   }*/
//	m_files_total = m_vecFileInfo.size();
//
//	return true;
//}
//
//
//bool pack_file::readPackFileCache()
//{
//
//	for (auto it : m_vecPackFileList) {
//
//
//		if (!memory_file::open(it->strPackFullPath, memory_usage::off))
//			return false;
//
//		//m_strFileNameTemp = it->strPackFullPath;
//
//		//m_pack_files_total = m_vecPackFileList.size();
//
//		if (!readHeader())
//		{
//			window::_getOKErrorBox("An error has occured ");
//			return false;
//		}
//
//		//m_vector_index = 0;
//		//m_ffset = oFile.uiHeaderLength;
//
//
//		readFileListCache(it->strPackFullPath);
//		close();
//	}
//
//
//	return true;
//
//}
//
//
//
//
//
//bool pack_file::close()
//{
//	//oFile = { 0 };
//	m_bIsHeaderRead = false;
//	m_oAnimFile.close();
//
//	return memory_file::close();
//}
//
//uint32_t pack_file::getNumberOfElements()
//{
//	return m_vecFileInfo.size();
//}
//
//const vector<pack_file_node*>& pack_file::searchList(const vector<pack_file_node*>& _vecSrc, const std::string& _strSearchString)
//{
//	m_vecTempList.clear();
//	for (size_t i = 0; i < _vecSrc.size(); i++)
//	{
//
//		if (string::npos != _vecSrc[i]->strInternalPath.find(_strSearchString))
//		{
//			m_vecTempList.push_back(_vecSrc[i]);
//		}
//	}
//
//	return m_vecTempList;
//}
//
//const vector<pack_file_node>& pack_file::searchForPath(const std::string& _strFileName, const vector<string>& _vecPacksToSearch)
//{
//	if (_vecPacksToSearch.size() == 0)
//	{
//		m_vecTempPACK_FILE_INFO.clear();
//
//		return m_vecTempPACK_FILE_INFO;
//	}
//
//
//
//
//
//}
//
//const vector<pack_file_node*>& pack_file::getMatchingFiles(cstr& _strFomratId, cstr& _strSecondaryId)
//{
//
//	for (size_t i = 0; i < m_vecFileInfo.size(); i++)
//	{
//
//		//	 ******* EXIT -*******
//		if (m_poParentThread)
//			if (m_poParentThread->endRequested())
//				return m_vecFileInfo;
//		//	 -******* EXIT -*******
//
//
//		pack_file_node* poPack = m_vecFileInfo[i];
//
//
//		if (poPack->strExtension == "anim")
//		{
//
//			//	m_message += p->strFileName;
//				//*pWin = m_message;
//
//			anim_file a;
//			anim_file* poANIM = &a;
//			poANIM->open(poPack->strPackFullPath, "", poPack->data_absolute_offset, poPack->file_size, memory_usage::off);
//			poANIM->readHeaderInfo();
//
//
//
//			if (_strFomratId == poANIM->getFormaIdString() &&
//				_strSecondaryId == poANIM->getSecondaryIdString()
//				)
//			{
//				poPack->fFrameRate = poANIM->m_file.oHeader.fFrameRate;
//				poPack->fEndTime = poANIM->m_file.oHeader.fEndTime;
//				poPack->dwFrameCount = poANIM->m_file.dwFrameCount;
//
//				poPack->strFormatId = _strFomratId;
//				poPack->strSecondaryId = _strSecondaryId;
//				m_vecSearchFileList.push_back(poPack);
//
//
//
//
//			}
//			poANIM->close();
//
//			//delete poANIM;
//
//
//		}
//		m_files_completed = i;
//
//
//	}
//
//
//	return m_vecSearchFileList;
//
//}
//
//bool pack_file::readAllFileHeaders(const std::string&, const std::string&)
//{
//	return false;
//}
//
//thread_class* pack_file::getThread()
//{
//	return m_thread_task;
//}
//
//uint32_t pack_file::getNumberOfFIles(const vector<string>& _vecPacksToSearch)
//{
//	if (_vecPacksToSearch.size() < 2)
//	{
//		return 0;
//	}
//
//
//	//readFile
//
//	return 0;
//
//
//
//}
//
//
//
//bool pack_file::readHeaderCache()
//{
//	//m_bInMemory = false;
//	read(&oFile.oHeader.oHeaderId.sz, 4);
//	oFile.uiHeaderLength = getHeaderLength(oFile.oHeader.oHeaderId.sz);
//
//	read(&oFile.oHeader.dwType, oFile.uiHeaderLength - 4);
//
//
//	//seek(12, 1);
//	//	//seek(oFile.uiHeaderLength - 4);
//	//read(&oFile.header.uiFileCount, 8);
//
//	// seek to the start of file name / file length list
//	//seek(oFile.uiHeaderLength);
//	return true;
//
//}
//
//bool pack_file::readFileList(const std::string& _strFileName)
//{
//	//pack_file_node* po = nullptr;
//
//	//const vector<string> sm_vecRome2Packs = { "models.pack", "data.pack" };
//	m_strCurrentPackFileTemp = _strFileName;
//
//	uint32_t m_offset = oFile.uiHeaderLength;
//
//	if (_strFileName == "e:\\steamlibrary\\steamapps\\common\\total war warhammer ii\\data\\campaign_variants_2.pack")
//		int a = 1;
//
//	m_strIndexStrings.clear();
//	for (size_t i = 0; i < oFile.oHeader.dwPackileIndexCount; i++)
//	{
//		char chIn = '\0';
//		string strPackFileName = "";
//		do
//		{
//			read(&chIn, 1);
//			strPackFileName += chIn;
//			m_offset++;
//
//		} while (chIn != '\0');
//
//		m_strIndexStrings.push_back(strPackFileName);
//		//m_offset += strPackFileName.size();
//		//m_offset += strPackFileName.size();
//	}
//
//
//
//
//
//
//	char szPath[1024];
//	bool bFilesWithExt = false;
//	for (m_file_list_index = 0; m_file_list_index < oFile.oHeader.uiFileCount/* && !m_bFileFound*/; m_file_list_index++)
//	{
//
//		// ****EXIT - *******
//		/*if (m_poParentThread)
//			if (m_poParentThread->endRequested())
//			{
//				return false;
//			}*/
//
//
//
//
//
//
//		pack_file_node* po = new pack_file_node;
//		//po->strParentFile =  _strFileName;
//		//po->strParentFolder = m_strPackFileFolder;
//
//
//		//if (po->strParentFolder[po->strParentFolder.size()] != '\\')
//		//	po->strParentFolder += '\\';
//
//
//		//po->strPackFullPath = po->strParentFolder +tools::GetFileFromFullPath(po->strParentFile.c_str());
//
//		uint32_t index = 0;
//
//		// read file size from file
//		read(&po->file_size, 4);
//
//		// read compression flag byte, if a
//
//		if (oFile.oHeader.oHeaderId.dw == pack_file::makeID("PFH5"))
//			read(&po->chCompressionFlag, 1);
//
//		po->data_absolute_offset = m_offset + oFile.oHeader.iDataStart;
//		// save the absolute offset
//
//		// Save the offset for the files
//		m_offset += po->file_size;
//
//		// add the file size, so next file will get the right offset
//
//		char ch = 'a';
//
//		while (ch != '\0')
//		{
//			read(&ch, 1);
//			szPath[index] = ch;
//			index++;
//		}
//		po->strInternalPath = szPath;
//		po->strInternalPath = tools::GetFileFromFullPath(szPath);
//		po->strPackFullPath = _strFileName;
//		po->strExtension = tools::GetExtensionFromFullPath(po->strInternalPath.c_str());
//		po->strInternalPath = tools::GetFileFromFullPath(po->strInternalPath);
//
//		if (po->strExtension == m_strSearchExtension)
//		{
//			bFilesWithExt = true;
//			if (m_bEqualToExtension)
//				m_vecFileInfo.push_back(po);
//
//		}
//
//		// if equal to empty substring, add ALL file
//		if (m_vecstrTempSubstr.size() == 0 && m_bEqualToSubstring)
//		{
//			m_vecFileInfo.push_back(po);
//		}
//		else
//			if (m_vecstrTempSubstr.size() > 0)
//			{
//				// check for substrings
//				for (size_t i = 0; i < m_vecstrTempSubstr.size(); i++)
//				{
//
//					po->strInternalPath = tools::lower(po->strInternalPath);
//
//					if (m_bEqualToSubstring) // if equal flag = true check for equality
//					{
//
//						if (po->strInternalPath == m_vecstrTempSubstr[i])
//						{
//							// only read for anim files if secondary id set
//							if (m_strSecondaryId != "")
//							{
//								// aloocate and read .anim file, for ID strings
//								anim_file* poAnimFile = new anim_file;
//								if (poAnimFile->openAndReadIds(po->strPackFullPath, "",
//									po->data_absolute_offset, po->file_size, memory_usage::off)
//									)
//
//									// check if secondary matches the ID string being searched for
//									if (m_strSecondaryId == poAnimFile->m_strSecondaryId)
//									{
//										// add file to vector
//										m_vecFileInfo.push_back(po);
//										m_bFileFound = true;
//									}
//
//								delete poAnimFile;
//							}
//							else
//								m_vecFileInfo.push_back(po);
//
//							m_dwMemoryUsed += sizeof(*po);
//						}
//					}
//					else// if equal flag = false check for sub string
//						if (po->strInternalPath.find(m_vecstrTempSubstr[i]) != std::string::npos)
//						{
//							m_vecFileInfo.push_back(po);
//							m_dwMemoryUsed += sizeof(*po);
//						}
//				}
//			}
//		/*else if ((m_strSearchExtension == ""))
//		{
//
//			m_vecFileInfo.push_back(po);
//			m_dwMemoryUsed += sizeof(*po);
//		}*/
//
//
//		m_files_total++;
//	}
//
//
//	if (!bFilesWithExt)
//	{
//		if (!CONF.find("no_" + m_strSearchExtension, _strFileName))
//		{
//			CONF.setValues("no_" + m_strSearchExtension, { _strFileName });
//			CONF.write();
//		}
//	}
//
//
//
//
//	return true;
//
//	/*
//	fixed_data.extend_from_slice(&data[4..8]);
//	fixed_data.push(0);
//	fixed_data.extend_from_slice(&data[0..4]);
//	fixed_data.extend_from_slice(&[0; 4]);
//	fixed_data.extend_from_slice(&data[9..]);
//	*/
//
//
//
//	// do stuff with compressed files
//
////		if (po->chCompressionFlag != 0 & 1 == 0)
////
////		{
////			if (po->pLZMAFile)
////				delete po->pLZMAFile;
////
////			po->pLZMAFile = new lzma_file;
////			//po->pLZMAFile->compressed_size = po->file_size;
////
////
////			ifstream f2(po->strPackFullPath);
////			f2.seekg(po->data_absolute_offset);
////
////
////			/*po->pLZMAFile->open(po->strPackFullPath);
////			po->pLZMAFile->seek(po->data_absolute_offset);*/
////
////			//ofstream f("c:\\temp\\test_2.7z", ios::binary);
////			//char* pch = new char[po->pLZMAFile->compressed_size];
////
////			//po->pLZMAFile->read((char*)&po->pLZMAFile->oHeader, 13);
////
////
////			/*for (size_t i = 0; i < po->pLZMAFile->compressed_size; i++)
////			{
////				pch[i] = ~pch[i];
////			}
////			f.write(pch, po->pLZMAFile->compressed_size);
////			f.close();*/
////
////
////
////
////			//po->pLZMAFile->read(&po->pLZMAFile->oHeader, sizeof(po->pLZMAFile->oHeader)-4);
////
////
////			/**********************************************************************/
////
////			uint8_t* pch = (uint8_t*)&po->pLZMAFile->oHeader;
////
////
////			uchar p2[13], p[9];
////
////			f2.read((char*)p, 9);
////
////			/**********************************************************************/
////
////			//// 4 5 6 7  0 0 1 2  3 0 0 0 0 the rest
////
////			p2[0] = p[4];
////
////			p2[1] = p[5];
////			p2[2] = p[6];
////			p2[3] = p[7];
////			p2[4] = 0;
////
////			p2[5] = p[0];
////			p2[6] = p[1];
////			p2[7] = p[2];
////			p2[8] = p[3];
////
////			p2[9] = 0;
////			p2[10] = 0;
////			p2[11] = 0;
////			p2[12] = 0;
////
////			memcpy(&po->pLZMAFile->oHeader.properties, &p2[0], 1);
////			memcpy(&po->pLZMAFile->oHeader.dictionary_size, &p2[1], 4);
////			memcpy(&po->pLZMAFile->oHeader.uncompressed_size, &p2[5], 4);
////
////			uint32_t result = this->oFile.oHeader.dwType & pack_flags::PFHFlags::HAS_ENCRYPTED_DATA;
////					   			uint32_t compressed_size = po->file_size;
////			uint32_t uncompressed_size = po->pLZMAFile->oHeader.uncompressed_size;
////
////			// create buffer and make room for the new header
////
////			char* pData = new char[compressed_size];
////			f2.read(pData+13, compressed_size -13);
////
////			// copy the changed header tinto the buffer
////			memcpy(pData, p2, 13);
////
////			vector<uchar> in2(pData, pData + (compressed_size));
////
////			vector<uchar> out;
////
////			out.resize(po->pLZMAFile->oHeader.uncompressed_size);
////
////
////
////
////
////			//vector<uchar> in2;
////			/*int i = 0;
////			for (; i < 13; i++)
////				in2.push_back[p2[i]];*/
////			//in2 = decrypt_packed_file(in);
////
////
////
////			//for (size_t i = 0; i < 9; i++)
////			//{
////			//	in2.insert(in2.begin() + i, p2[i]);
////			//}
////
////			Uncompress1(
////				out,
////				in2,
////				uncompressed_size);
////
////
////			/*uint32_t output_size = po->pLZMAFile->oHeader.uncompressed_size;
////			std::unique_ptr<uint8_t[]> P_CH = lzmaDecompress(p2,
////				size, &output_size);*/
////
////
////			//char* p2 = (char*)&po->pLZMAFile->oHeader;
////			// 4 5 6 7  0 0 1 2  3 0 0 0 0 the rest
//////
//////
//////			ELzmaStatus lzmaStatus;
//////			SizeT procOutSize = po->pLZMAFile->oHeader.uncompressed_size,
//////				procInSize = po->pLZMAFile->compressed_size - 13;
//////
//////			size = procOutSize;
//////			auto blob = std::make_unique<uint8_t[]>(size);
//////
////			/*vector<uchar> in(p2, p2+size);
////			vector<uchar> out;
////
////
////			Uncompress1(
////				out,
////				in,
////				po->pLZMAFile->oHeader.uncompressed_size);*/
////
////
////
////
//////			int status = LzmaDecode(blob.get(), &procOutSize, &po->pLZMAFile->pData[13],
//////				&procInSize, &po->pLZMAFile->oHeader.properties, 5, LZMA_FINISH_ANY, &lzmaStatus, &_allocFuncs);
//////
//////			uint32_t outputSize;
//////			if (status == SZ_OK && procOutSize == size) {
//////				outputSize = size;
//////
//////				 blob;
//////			}
//////
//////
//////
//////
//////
//////			ofstream file("c:\\temp\\test.7z", ios::binary);
//////
//////			//le.write((char*)"7z", 2);
//////			//file.write((char*)p2, 13);
//////			file.write((char*)&po->pLZMAFile->oHeader, 13);
//////			file.write((char*)po->pLZMAFile->pData, po->pLZMAFile->compressed_size - 13);
//////			file.close();
//////
//////			_BREAK_POINT;
///////*
//////			unsigned lc, pb, lp;
//////			uint32_t dictSize;
//////			uint32_t	 dictSizeInProperties;
//////
//////			uint8_t properties = po->pLZMAFile->oHeader.properties;
//////
//////			{
//////				unsigned d = properties;
//////				if (d >= (9 * 5 * 5))
//////					throw "Incorrect LZMA properties";
//////				lc = d % 9;
//////				d /= 9;
//////				pb = d / 5;
//////				lp = d % 5;
//////				dictSizeInProperties = 0;
//////				for (int i = 0; i < 4; i++)
//////					dictSizeInProperties |= (uint32_t)properties[i + 1] << (8 * i);
//////				dictSize = dictSizeInProperties;
//////				if (dictSize < (1 << 16 ))
//////					dictSize = (1 << 16);
//////			}
////
////
////
////		}
////
////
//
//
//}
//
//
//bool pack_file::readFileListCache(const std::string& _strFileName)
//{
//
//
//	uint32_t m_offset = oFile.uiHeaderLength;
//
//
//
//	//m_strIndexStrings.clear();
//	for (size_t i = 0; i < oFile.oHeader.dwPackileIndexCount; i++)
//	{
//		char chIn = '\0';
//		string strPackFileName = "";
//		do
//		{
//			read(&chIn, 1);
//			strPackFileName += chIn;
//
//
//		} while (chIn != '\0');
//
//		//		m_strIndexStrings.push_back(strPackFileName);
//			//	m:m_offset += strPackFileName.size() + 1;
//				//m_offset += strPackFileName.size();
//	}
//
//
//
//
//	//32 m_vector_index = 0;
//
//		//uint32_t m_memory = 28;
//
//		//anim_file* poAnimFile = new anim_file;
//
//	pack_file_node* po = new pack_file_node;
//	char szPath[1024];
//	bool bFilesWithExt = false;
//	uint32_t offset = m_vecFileInfo.size();
//	m_vecFileInfo.resize(offset + oFile.oHeader.uiFileCount);
//	for (m_file_list_index = 0; m_file_list_index < oFile.oHeader.uiFileCount && !m_bFileFound; m_file_list_index++)
//	{
//
//		// ****EXIT - *******
//		/*if (m_poParentThread)
//			if (m_poParentThread->endRequested())
//			{
//				return false;
//			}*/
//
//
//
//
//
//
//			//pack_file_node* po = new pack_file_node;
//			//po->strParentFile =  _strFileName;
//			//po->strParentFolder = m_strPackFileFolder;
//
//
//			//if (po->strParentFolder[po->strParentFolder.size()] != '\\')
//			//	po->strParentFolder += '\\';
//
//
//			//po->strPackFullPath = po->strParentFolder +tools::GetFileFromFullPath(po->strParentFile.c_str());
//
//		uint32_t index = 0;
//
//		// read file size from file
//		read(&po->file_size, 4);
//
//		// read compression flag byte, if a
//
//		if (oFile.oHeader.oHeaderId.dw == pack_file::makeID("PFH5"))
//			read(&po->chCompressionFlag, 1);
//
//		po->data_absolute_offset = m_offset + oFile.oHeader.iDataStart;
//		// save the absolute offset
//
//		// Save the offset for the files
//		m_offset += po->file_size;
//
//		// add the file size, so next file will get the right offset
//
//
//		char ch = 'a';
//		while (ch != '\0')
//		{
//			read(&ch, 1);
//			szPath[index] = ch;
//			index++;
//		}
//		//po->strInternalPath = szPath;
//		//po->strInternalPath = szPath;
//		//po->strPackFullPath = _strFileName;
//
//		//	std::transform(po->strPackFullPath.begin(), po->strPackFullPath.end(), po->strPackFullPath.begin(), ::tolower);
//
//		//po->strExtension = tools::GetExtensionFromFullPath(po->strInternalPath.c_str());
//		//po->strInternalPath = tools::GetFileFromFullPath(po->strInternalPath);
//
//		//m_vecFileInfo[offset + m_file_list_index] = po;
//	}
//
//	return true;
//
//
//}
//
//bool pack_file::readFileList_PFH5(const std::string& _strFileName)
//{
//	{
//		//pack_file_node* po = nullptr;
//
//		//const vector<string> sm_vecRome2Packs = { "models.pack", "data.pack" };
//
//
//
//
//		m_strCurrentPackFileTemp = _strFileName;
//
//
//		uint32_t m_vector_index = 0;
//		uint32_t m_ffset = oFile.uiHeaderLength;
//		uint32_t m_memory = 28;
//
//		char szPath[1024];
//		for (m_file_list_index = 0; m_file_list_index < oFile.oHeader.uiFileCount; m_file_list_index++)
//		{
//
//			// ****EXIT - *******
//			/*if (m_poParentThread)
//				if (m_poParentThread->endRequested())
//				{
//					return false;
//				}*/
//
//
//
//
//
//
//			pack_file_node* po = new pack_file_node;
//			//po->strParentFile =  _strFileName;
//			//po->strParentFolder = m_strPackFileFolder;
//
//
//			//if (po->strParentFolder[po->strParentFolder.size()] != '\\')
//			//	po->strParentFolder += '\\';
//
//
//			//po->strPackFullPath = po->strParentFolder +tools::GetFileFromFullPath(po->strParentFile.c_str());
//
//			uint32_t index = 0;
//
//			// read file size from file
//			read(&po->file_size, 4);
//			read(&po->chCompressionFlag, 1);
//
//
//
//			// save the absolute offset
//			po->data_absolute_offset = m_ffset + oFile.oHeader.iDataStart;
//
//
//			// Save the offset for the files
//			m_ffset += po->file_size;
//
//			// add the file size, so next file will get the right offset
//
//			char ch = 'a';
//
//			while (ch != '\0')
//			{
//				read(&ch, 1);
//				szPath[index] = ch;
//				index++;
//			}
//			po->strInternalPath = szPath;
//			po->strPackFullPath = _strFileName;
//
//			//	std::transform(po->strPackFullPath.begin(), po->strPackFullPath.end(), po->strPackFullPath.begin(), ::tolower);
//
//			po->strExtension = tools::GetExtensionFromFullPath(po->strInternalPath.c_str());
//
//
//
//			if (po->strExtension == m_strSearchExtension)
//			{
//				m_vecFileInfo.push_back(po);
//
//			}
//
//
//			po->strInternalPath = tools::GetFileFromFullPath(po->strInternalPath);
//
//
//
//			if (m_vecstrTempSubstr.size() > 0)
//			{
//				// check for substrings
//				for (size_t i = 0; i < m_vecstrTempSubstr.size(); i++)
//				{
//
//					string strTemp = tools::GetFileFromFullPath(szPath);
//					string strSub = m_vecstrTempSubstr[i];
//
//
//					std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);
//
//					std::transform(strSub.begin(), strSub.end(), strSub.begin(), ::tolower);
//
//
//					if (m_bEqualToSubstring) // if equal flag = true check for equality
//					{
//						if (strTemp == strSub)
//						{
//							m_vecFileInfo.push_back(po);
//							m_dwMemoryUsed += sizeof(*po);
//						}
//					}
//					else// if equal flag = false check for sub string
//						if (strTemp.find(strSub) != string::npos)
//						{
//							m_vecFileInfo.push_back(po);
//							m_dwMemoryUsed += sizeof(*po);
//						}
//				}
//			}
//			else if ((m_strSearchExtension == ""))
//			{
//
//				m_vecFileInfo.push_back(po);
//				m_dwMemoryUsed += sizeof(*po);
//			}
//
//
//			m_files_total++;
//		}
//
//
//		return true;
//	}
//
//}
//
//
//
//vector<pack_file_node*>& pack_file::searchForPath(const std::string& _strFilePath)
//{
//	string strTemp = tools::getLowerAndBackSlash(_strFilePath);
//
//
//	for (auto& it : m_vecFileInfo)
//	{
//		if (strTemp == it->strInternalPath)
//			m_vecSearchFileList.push_back(it);
//
//	}
//
//	return m_vecSearchFileList;
//}
//
//vector<pack_file_node*>& pack_file::searchForFile(const std::string& _strFileName)
//{
//	m_vecSearchFileList.clear();
//	for (auto& it : m_vecFileInfo)
//	{
//		if (it->strInternalPath == _strFileName)
//			m_vecSearchFileList.push_back(it);
//	}
//
//	return m_vecSearchFileList;
//}
//
//vector<pack_file_node*>& pack_file::searchForExt(const std::string& _strExt)
//{
//	m_vecSearchFileList.clear();
//	for (auto& it : m_vecFileInfo)
//	{
//		if (it->strExtension == _strExt)
//			m_vecSearchFileList.push_back(it);
//	}
//
//	return m_vecSearchFileList;
//}
//
//vector<pack_file_node*>& pack_file::searchForPartialPath(const std::string& _strExt)
//{
//	m_vecSearchFileList.clear();
//
//	for (size_t i = 0; i < m_vecFileInfo.size(); i++)
//	{
//		if (tools::compare_string_backward(m_vecFileInfo[i]->strInternalPath, _strExt))
//			m_vecSearchFileList.push_back(m_vecFileInfo[i]);
//
//	}
//	//for (auto it : )
//	//{
//	//	if (tools::compare_string_backward(it->strInternalPath, _strExt))
//	//		m_vecSearchFileList.push_back(it);
//	//}
//
//	return m_vecSearchFileList;
//}
//
//vector<pack_file_node*>& pack_file::searchForSubStringAndExtention(const std::string& _strSubString, const std::string& _strExtention)
//{
//	m_vecSearchFileList.clear();
//	for (auto& it : m_vecFileInfo)
//	{
//		if (it->strInternalPath.find(_strSubString) != string::npos && it->strExtension == _strExtention)
//			m_vecSearchFileList.push_back(it);
//	}
//
//	return m_vecSearchFileList;
//}
//
//bool pack_file::readPackFileListCache(const std::string& _cstrFileName)
//{
//
//
//	//
//	//
//	//	m_files_total = 0;
//	//	//pack_file_node* po = nullptr;
//	//
//	//	//const vector<string> sm_vecRome2Packs = { "models.pack", "data.pack" };
//	//	uint32_t vector_index = 0;
//	//	uint32_t offset = oFile.uiHeaderLength;
//	//
//	//
//	//
//	//	m_strCurrentPackFile = tools::GetFileFromFullPath(_cstrFileName.c_str());
//	//	for (size_t i = 0; i < m_vecPackFileList.size(); i++)
//	//	{
//	//
//	//
//	//
//	//		//uint32_t memory = 28;
//	//
//	//	/*	pack_file_node* po = new pack_file_node;
//	//		po->pMemFile = new memory_file;
//	//		po->pMemFile->open(m_vecPackFileList[i]->strPackFullPath);
//	//		po->pMemFile->seek(oFile.uiHeaderLength);
//	//
//	//		po->strParentFile = m_vecPackFileList[i]->strPackFullPath;
//	//		po->strParentFolder = tools::GetDirFromFullPath(m_vecPackFileList[i]->strPackFullPath);
//	//
//	//		m_strCurrentPackFile = tools::GetFileFromFullPath(m_vecPackFileList[i]->strPackFullPath.c_str());
//	//
//	//		if (po->strParentFolder[po->strParentFolder.size()] != '\\')
//	//			po->strParentFolder += '\\';*/
//	//
//	//
//	////		po->strPackFullPath = po->strPackFullPath;
//	//
//	//		uint32_t index = 0;
//	//
//	//		// read file size from file
//	//	//	po->pMemFile->read(&po->file_size, 4);
//	//
//	//
//	//		// read file size from file
//	//
//	//	//	po->data_absolute_offset = offset + oFile.header.iDataStart;
//	//		// save the absolute offset
//	//
//	//		// Save the offset for the files
//	//	//	offset += po->file_size;
//	//		// save the absolute offset
//	//
//	//		// Save the offset for the files
//	//
//	//		// add the file size, so next file will get the right offset
//	//
//	//		//char ch = 'a';
//	//
//	//		//while (ch != '\0')
//	//		//{
//	//		//	po->pMemFile->read(&ch, 1);
//	//		//	po->szPath[index] = ch;
//	//		//	index++;
//	//		//}
//	//		//
//	//
//	//		//po->strPath = po->szPath;
//	//
//	//		//std::transform(po->strPath.begin(), po->strPath.end(), po->strPath.begin(), ::tolower);
//	//		//
//	//		//po->strExtension = tools::GetExtensionFromFullPath(po->strPath.c_str());
//	//		//po->strFileName = tools::GetFileFromFullPath(po->strPath.c_str());
//	//
//	//		for (size_t i = 0; i < m_vecstrTemp.size(); i++)
//	//			if (m_vecstrTemp[i].size() > 0)
//	//			{
//	//				if (m_vecstrTemp[i] == po->strFileName) // is there a search substring?
//	//				{
//	//					string strTemp = po->szPath;
//	//					string strSub = m_strSearchSubString;
//	//
//	//
//	//					std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);
//	//					std::transform(strSub.begin(), strSub.end(), strSub.begin(), ::tolower);
//	//
//	//					if (strTemp.find(strSub) != string::npos)
//	//					{
//	//
//	//						m_vecFileInfo.push_back(po);
//	//						m_dwMemoryUsed += sizeof(*po);
//	//						m_vecFileInfo.erase(m_vecFileInfo.begin() + i);
//	//
//	//					}
//	//				}
//	//			}
//	//			else // not search string, save all
//	//			{
//	//
//	//				m_vecFileInfo.push_back(po);
//	//				//m_dwMemoryUsed += sizeof(*po);
//	//			}
//	//
//	//		m_pack_files_completed++;
//	//
//	//
//	//	}
//	//
//
//	return true;
//}
//
//const PACK_FILE_HEADER& pack_file::getHeader(const std::string& _strFileName)
//{
//
//	uint32_t b = 0b0000 - 0001 - 0000 - 0000;;
//
//	fstream oFile(_strFileName);
//
//	//oFile.read((char*)m_oTempoHeader.sz, 4);
//
//
//	//uint32_t header_len = getHeaderLength(m_oTempoHeader.sz);
//
//	//oFile.read((char*)& m_oTempHeader.dwType, header_len - 4);
//
//	// read only the 4 type bytes:
//	oFile.seekg(4, ios_base::cur);
//	oFile.read((char*)&m_oTempHeader.dwType, 4);
//
//
//	m_oTempHeader.dwType &= 0B111;
//
//
//	return m_oTempHeader;
//}
//
//const std::string& pack_file::getCurrentPackFileName()
//{
//	_getCurrentPackFileName_Mutex.create("GetFileName");
//	_getCurrentPackFileName_Mutex.wait();
//
//	return m_strFileName;
//
//	_getCurrentPackFileName_Mutex.close();
//}
//
//bool pack_file::extractFiles(const std::string& _strDestFie)
//{
//	//	// no .pack files
//	//
//
//	//uint32_t pack_index = 0;
//	//
//	//for (pack_index = 0; pack_index < m_vecFileInfo.size(); pack_index++)
//	//{
//	//	string szPath = _strDestFie;
//
//	//	/*if (_vecPackFiles[0].strFolder.size() > 0)
//	//		if (_vecPackFiles[0].strFolder[_vecPackFiles[0].strFolder.size()] != 92)
//	//		{
//	//			szPath += "\\";
//	//		}*/
//
//	//	// make complete path to file
//	//	//szPath += m_vecFileInfo[pack_index]->strPath;
//
//	//	if (!readPackFile(szPath))
//	//		return false;
//
//	//	// get and set the start data offset
//	//	m_dwBufferOffset = m_vecFileInfo[pack_index]->data_absolute_offset; // m_oFile.header.iDataStart + getHeaderLength();
//
//	//	if (!m_bInMemory)
//	//		seek(m_vecFileInfo[pack_index]->data_absolute_offset);
//
//	//	size_t i;
//	//	/*thread::createTask<pack_file>(this,
//	//	[this](pack_file* poThis) -> LRESULT*/
//
//	//	uint32_t errors = 0;
//	//	{
//
//	//		// Create all the folders
//
//	//		//if (_bCreateFolders)
//	//			for (i = 0; i < m_vecFileInfo.size(); i++)
//	//			{
//
//
//
//
//	//				size_t index = 0;
//	//				size_t next_path_index = 0;
//	//				bool bLastFolder = true;
//	//				string szPathTemp = _strDestFie;
//	//				if (szPathTemp[szPathTemp.size()] != 92)
//	//				{
//	//					szPathTemp += "\\";
//	//				}
//
//
//
//	//				do
//	//				{
//	//					bLastFolder = true;
//	//					uint32_t pathIndex = szPathTemp.size();
//
//	//					for (uint32_t p = index; m_vecFileInfo[i]->szPath[p] != '\0'; p++)
//	//					{
//	//						if (m_vecFileInfo[i]->szPath[p] == 92)
//	//						{
//	//							bLastFolder = false;
//	//						}
//	//					}
//
//	//					if (!bLastFolder)
//	//						while (m_vecFileInfo[i]->szPath[index] != 92
//	//							&& m_vecFileInfo[i]->szPath[index] != '\0')
//	//						{
//	//							szPathTemp += m_vecFileInfo[i]->szPath[index];
//
//	//							index++;
//
//	//						}
//
//	//					if (!bLastFolder)
//	//					{
//
//	//						CreateDirectory(szPathTemp.c_str(), NULL);
//	//						szPathTemp += '\\';
//	//					}
//
//	//					index++;
//	//					//next_path_index += (index+1);
//
//	//				} while (!bLastFolder);
//	//			}
//
//	//		string strPathTemp = _strDestFie;
//	//
//	//		if (strPathTemp[strPathTemp.size()] != 92)
//	//			strPathTemp += "\\";
//
//
//	//		// extract the files
//	//		for (i = 0; i < m_vecFileInfo.size(); i++)
//	//		{
//
//	//			m_dwBufferOffset = m_vecFileInfo[i]->data_absolute_offset; // m_oFile.header.iDataStart + getHeaderLength();
//
//	//			if (!m_bInMemory)
//	//				seek(m_vecFileInfo[i]->data_absolute_offset);
//
//
//	//			string strFileName = "";
//
//	//			//if (_bCreateFolders)
//	//				strFileName = strPathTemp + m_vecFileInfo[i]->szPath;
//	//			//else
//	//				//strFileName = strPathTemp + tools::GetFileFromFullPath(vecFileInfo[i]->szPath);
//	//			//string strFileName = vecFileInfo[i]->szPath; // + tools::GetFileFromFullPath(vecFileInfo[i]->szPath);
//	//			//CreateDirectory(strFilePath.c_str(), NULL);
//
//	//			if (m_bInMemory)
//	//			{
//	//				ofstream outFile(strFileName);
//	//				outFile.write(&this->m_pFileBuffer[this->m_dwBufferOffset], m_vecFileInfo[i]->file_size);
//	//				this->m_dwBufferOffset += m_vecFileInfo[i]->file_size;
//	//				outFile.close();
//	//			}
//	//			else
//	//			{
//
//	//				ofstream outFile(strFileName, ios::out | ios::binary);
//
//	//				char* p = new char[m_vecFileInfo[i]->file_size];
//	//				read(p, m_vecFileInfo[i]->file_size);
//
//	//				outFile.write(p, m_vecFileInfo[i]->file_size);
//	//				outFile.close();
//
//	//				delete[] p;
//
//	//			}
//
//	//		}
//
//
//
//	//	}
//
//
//	//	close();
//	//}
//	return true;
//}
//
//
//
//
//
////
////const game_pack_list pack_file::sm_packfilesRome2 =
////{ "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Total War Rome II\\data",
////{
////	{"models.pack"},
////	{"data.pack"}
////}
////};
////
////
//void pack_file_package::setFolder(const std::string& _strFolder)
//{
//	strFolder = _strFolder;
//}
//
//void pack_file_package::addPackFile(const std::string& _strFile)
//{
//	vecPackFile.push_back(_strFile);
//}
////////////////////////////////////////////////////////////////////////////////////////
////		Static definitions
////////////////////////////////////////////////////////////////////////////////////////

void pack_file_block::insertFile(const std::string&, uint8_t* _pBuffer, uint32_t file_size)
{
	// TODO: FINISH
}