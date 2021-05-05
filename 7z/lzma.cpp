#include "lzma.h"
#include "LzmaLib.h"
#include <vector>
#include <assert.h>
#include <fstream>

using namespace std;

std::unique_ptr<uint8_t[]> lzmaDecompress(const uint8_t* input, uint32_t inputSize, uint32_t* outputSize) {
	if (inputSize < 13)
		return NULL; // invalid header!

	// extract the size from the header
	UInt64 size = 0;
	for (int i = 0; i < 8; i++)
		size |= (input[5 + i] << (i * 8));

	if (size <= (256 * 1024 * 1024)) {
		auto blob = std::make_unique<uint8_t[]>(inputSize);

		ISzAlloc is;
		
		ELzmaStatus lzmaStatus;
		SizeT procOutSize = size, procInSize = inputSize - 13;
		int status = LzmaDecode(blob.get(), &procOutSize, &input[13], &procInSize, input, 5, LZMA_FINISH_ANY, &lzmaStatus, &is);

		
		if (status == SZ_OK && procOutSize == size) {
			*outputSize = size;
			return blob;
		}
		
	}



	return NULL;
}


void Uncompress1(
	std::vector<unsigned char>& outBuf,
	const std::vector<unsigned char>& inBuf,
	unsigned dwUnCompressedSize)

{
	outBuf.resize(dwUnCompressedSize);
	size_t dstLen = outBuf.size();


	ofstream file("C:\\temp\\WH2.lzma");
	for (auto i : inBuf)
		file.write((char*)&i, 1);

	size_t srcLen = inBuf.size() - LZMA_PROPS_SIZE - 8;
	SRes res = LzmaUncompress(
		&outBuf[0], &dstLen,
		&inBuf[0], &srcLen,
		&inBuf[0], LZMA_PROPS_SIZE);
	if (res == SZ_OK)
	{
		outBuf.resize(dstLen); // If uncompressed data can be smaller
		ofstream file2("c:\\temp\\dds_1.dds", ios::binary);

		for (auto i: outBuf)
		{
			file2.write((char*)&i, 1);
		}			

	}
	else
		return;
}

