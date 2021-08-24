
#include "pack_file.h"
#include "pack_file_structs.h"
#include "lzma_file.h"

bool pack_file_node::extractFile(const std::string& _dir)
{
	ifstream fileRead(this->strPackFullPath, ios::binary);
	if (!fileRead.is_open())
		return false;

	fileRead.seekg(this->data_absolute_offset);
	if (fileRead.eof())
		return false;

	// is compressed ?
	if (chCompressionFlag == 1) {

		lzma_file O;

		O.decompressToMemory(this);

		ofstream fileWrite(_dir, ios::binary);
		if (!fileWrite.is_open())
			return false;

		fileWrite.write((char*)pBuffer.get(), outBuf.size());

		fileWrite.close();

		return true;
	}

	/*string dir = _dir;
	if (dir[dir.size() - 1] != '\\')
		dir += '\\';

	string path = (dir + this->strInternalFileName);*/
	
	
	ofstream fileWrite(_dir, ios::binary);
	if (!fileWrite.is_open())
		return false;

	char* pch = new char[this->file_size];
	fileRead.read(pch, this->file_size);
	if (fileRead.eof())
		return false;


	fileWrite.write(pch, this->file_size);
	
	delete[] pch;

	fileRead.close();
	fileWrite.close();
	
	return true;
}

const uint8_t* pack_file_node::extractFileToMemory(bool _bStorePtr)
{

	// if ´compressed fill bugger with deompresed data
	if (chCompressionFlag == 1)	{

		lzma_file O;

		O.decompressToMemory(this);

		return pBuffer.get();
	}


	ifstream fileRead(this->strPackFullPath, ios::binary);	
	fileRead.seekg(this->data_absolute_offset);		
	
	pBuffer.reset();
	pBuffer = shared_ptr<uint8_t>(new uint8_t[this->file_size], [](uint8_t* p) {delete[] p; });
	fileRead.read((char*)pBuffer.get(), this->file_size);
	return pBuffer.get();;
}





//pack_file_node::operator pack_node_info()
//{
//	pack_node_info pni;
//
//	pni.strPackFullPath = this->strPackFullPath;
//	pni.strFileName = this->strInternalFileName;
//	pni.strExtension = this->strExtension;
//	pni.file_size = this->file_size;
//	pni.data_absolute_offset = this->data_absolute_offset;
//	pni.strInternalPath = this->strInternalPath;
//
//
//	return pni;
//}
//
