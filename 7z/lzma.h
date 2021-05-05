
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <memory>
#include <vector>
#include "LzmaEnc.h"
#include "LzmaDec.h"

#include "Alloc.h"





extern std::unique_ptr<uint8_t[]> lzmaDecompress(const uint8_t* input, uint32_t inputSize, uint32_t* outputSize);

extern void Uncompress1(
	std::vector<unsigned char>& outBuf,
	const std::vector<unsigned char>& inBuf,
	unsigned dwUnCompressedSize);
