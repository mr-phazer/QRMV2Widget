#pragma once
#include "..\SystemLib\ISerializeabe.h"
#include "RigidModelFileStructs.h"

namespace ImporterLib
{

    namespace RigidModel
{
        struct IRigidModelFile : SystemLib::ISerializable
        {

        };
        
        struct RigidModelFile7
        {
            RigidModelSubMesh* GetSubMesh(size_t lod, size_t group);


            FileHeader oFileHeader;
            SystemLib::SerializableVector<LODHeaderV7> vecLodHeaders;
            SystemLib::SerializableVector<RigidModelLodData> vecLodData

        };



        class IRigidModelV2File

        {
        public:

            template<class VERTEX_TYPE>
            VERTEX_TYPE* getVertexDataAs(size_t lod, size_t group);
            
            
            virtual void* getVertexData(size_t lod, size_t group);
            
            virtual void* getVertexData(size_t lod, size_t group);
            virtual uint16_t* getIndexData(size_t lod, size_t group);

        protected:
            std::vector <std::shared_ptr<ILODHeader>> m_vecLodHeader;



        };

       
    }
}