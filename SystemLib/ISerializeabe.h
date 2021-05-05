#pragma once

#include <d3d11.h>

#include <map>

#include "Object.h"
#include "data_stream.h"

#include "return_value.h"

namespace SystemLib 
{
    struct ISerializable //:
        //public Object
    {
    public:
        ISerializable()
        {
        }
        
        ISerializable(const ISerializable& _ref)
        {

        }

        //virtual std::pair<int, const char*> read(DataStream& _DataStream) { return { 0, nullptr }; };
        //virtual std::pair<int, const char*> write(DataStream& _DataStream) { return { 0, nullptr }; };
        //virtual bool read(DataStream& _DataStream) { return false; };
        //virtual bool write(DataStream& _DataStream) { return false; };
        virtual return_value read(DataStream& _DataStream) = 0;
        virtual return_value write(DataStream& _DataStream) = 0;
        virtual size_t size() { return 0; };
        virtual return_value validate() { return true; };
        
        bool last_error(const std::string& _strErr)
        {
            strLastError = "Error!: " + _strErr;
            return false;
        }

        std::string strLastError = "";

        DataStream oDataStream;
    };

}