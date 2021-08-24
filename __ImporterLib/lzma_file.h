#pragma once

#include "pack_file_structs.h"
#include "lzma_file.h"


#include "..\SystemLib\data_stream.h"

struct lzma_file : public SystemLib::DataStream
{

	struct lzma_header {


		uint8_t properties;			// 0										

		uint32_t dictionary_size;	// 1,2,3,4						
		uint32_t uncompressed_size;	// 5,6,7,8		
		uint32_t filler = 0;			// 9, 10, 11


	};

	lzma_header oHeader;

	uint32_t compressed_size;


	uint8_t* pData = nullptr;



	//bool ecompressionToMemory(pack_file_node* _ppfn);

	bool decompressToMemory(pack_file_node* _ppfn);



};




