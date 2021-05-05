#pragma once

#include <string>
#include <vector>
#include <map>

#include <DirectXMath.h>
#include "BaseNode.h"
#include <memory>

using namespace std;

namespace RenderLib
{
	struct BoneNodeMeta
	{
		std::string m_strName = "empty bone name";
		int32_t id = -1;
		int32_t parent_id = -1;

	};
	//struct bone_node;
	//typedef shared_ptr<bone_node> bone_node_ptr;

	struct bone_node : public BaseNode, public BoneNodeMeta
	{
	public:		
		bone_node() {};
		bone_node(bool _bValid) { m_bIValid = _bValid; };
			

	};	

	
	//struct object_manager
	//{	

	//	object_manager();

	//	skeleton* bone(const std::string& _strName); // creates a skeleton and stores it in 
	//	skeleton* create_bone(const std::string& _strName, int _id, int _parent, bone_node* _poParent = nullptr); // creates a skeleton and stores it in 
	//	
	//		
	//protected:
	//
	//	object_manager* m_poInstance = nullptr; // used by the static methods to access the local member variables, 

	//	// skeletons
	//	map<string, shared_ptr<_3dObjectBase>> map_spoNameToSkeleton;
	//	vector<string, shared_ptr<_3dObjectBase>> map_spoNameToSkeleton;
	//};


	
}
