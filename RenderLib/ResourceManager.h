#pragma once

#include <memory>
#include <map>


#include "IAbstractObject.h"
#include "VariantMeshStructs.h"


using namespace std;

namespace RenderLib {
    class MeshLodData;
    class Scene;
    
    class ResourceManager :
        public IAbstractObject
    {
    public:
        virtual bool destroy() override;
        shared_ptr<MeshLodData> getMeshLodDataFromInternalPath(Scene* _poScene, const VariantMeshInfo* _poMeshInfo, int32_t bone_index = -1);

    protected:
        map<string, shared_ptr<MeshLodData>> m_mapStringToMeshLodData;
    };

}