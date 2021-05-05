#include "lzma_file.h"
#include "..\7z\LzmaLib.h"
#include <assert.h>

bool lzma_file::decompressToMemory(pack_file_node* _ppfn)
{
	// fid filke
	open(_ppfn->strPackFullPath);
	seek_absolute(_ppfn->data_absolute_offset);

	/******************************************************************/

	uint8_t p2[13], p[9];

	readMem((char*)p, 9);

	/**********************************************************************/

	//// 4 5 6 7  0 0 1 2  3 0 0 0 0 the rest

	// fix header

	p2[0] = p[4];

	p2[1] = p[5];
	p2[2] = p[6];
	p2[3] = p[7];
	p2[4] = 0;

	p2[5] = p[0];
	p2[6] = p[1];
	p2[7] = p[2];
	p2[8] = p[3];

	p2[9] = 0;
	p2[10] = 0;
	p2[11] = 0;
	p2[12] = 0;

	memcpy(&oHeader.properties, &p2[0], 1);
	memcpy(&oHeader.dictionary_size, &p2[1], 4);
	memcpy(&oHeader.uncompressed_size, &p2[5], 4);

	//uint32_t result = this->oFile.oHeader.dwType & pack_flags::PFHFlags::HAS_ENCRYPTED_DATA;

	uint32_t compressed_size = _ppfn->file_size - 13;
	uint32_t uncompressed_size = oHeader.uncompressed_size;

	// create buffer and make room for the new header

	uint8_t* pData = new uint8_t[_ppfn->file_size + 4];
	memcpy_s(pData, _ppfn->file_size + 4, p2, 13);
	readMem(&pData[13], _ppfn->file_size - 9);

	_ppfn->pBuffer = shared_ptr<uint8_t>(new uint8_t[oHeader.uncompressed_size], [](uint8_t* p) {delete[] p; });

	size_t OutCount = oHeader.uncompressed_size;
	size_t SrcCount = _ppfn->file_size + (uint32_t)4;

	_ppfn->outBuf.resize(uncompressed_size);
	size_t dstLen = _ppfn->outBuf.size();
	size_t srcLen = SrcCount - LZMA_PROPS_SIZE - 8;

	SRes res = LzmaUncompress(
		_ppfn->pBuffer.get(), &dstLen,
		&pData[LZMA_PROPS_SIZE + 8], &srcLen,
		&pData[0], LZMA_PROPS_SIZE);

	assert(res == SZ_OK);
	_ppfn->outBuf.resize(dstLen); // If uncompressed data can be smaller

	//ofstream file("c:\\temp\\" + _ppfn->strInternalFileName, ios::binary);
	//file.write((char*)(&_ppfn->outBuf[0]) ,_ppfn->outBuf.size());
	//file.close();

	delete[] pData;

	return true;
}