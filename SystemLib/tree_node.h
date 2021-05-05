#pragma once
#include "tools.h"
#include <memory>

#include <map>
#include <vector>

using namespace std;


//template<> bool compare(string _t1, string _t2)
//{
//	auto t1 = tools::lower(_t1);
//	auto t2 = tools::lower(_t2);
//
//	return ( t1 == t2 );
//}


template <class KEY, class DATA>
struct tree_node
{
	friend struct path_node;
	friend class vmd_tree_view;
	friend class variant_mesh_def;

public:
	typedef tree_node<KEY, DATA> ret_type;

	bool bHasData = false;

public:

	static std::shared_ptr<tree_node<KEY, DATA>> m_spoDummy;
	void destroyTree()
	{

		if (m_poRoot)
			m_poRoot->destroyChildren_Recursive();
	}


	void destroyChildren_Recursive()
	{
		for (auto& it : m_mapChildren)
		{

			/*it.second->destroyChildren_bRursive();
			it.second.reset();*/
			//it->second->destroyChildren();
		}

	}

	//	vector<DATA>& findChildren(KEY start, KEY item_to_find); 
	//private:
	//	vector<DATA>& _seachTreeRecursive(ret_type* _poParentTreeNoe, KEY item_to_find);

public:

	static tree_node<KEY, DATA>* createRoot(const std::string& strID)
	{
		tree_node<KEY, DATA>* spo = new tree_node<KEY, DATA>;
		sm_mapRoot[strID] = spo;
		spo->id = strID;
		spo->bIsRoot = true;
		spo->m_poRoot = spo;

		return spo;
	}

	static tree_node<KEY, DATA>* getRoot(const std::string& strID)
	{
		auto it = sm_mapRoot.find(strID);

		if (it != sm_mapRoot.end())
			return it->second;

		return nullptr;


	}
	// create a new node by traversing the tree like:
	// [KEY]\\[KEY]\\[KEY]\\[KEY]\\[KEY]\\[DATA]
	// fist KEY is the root





	tree_node<KEY, DATA>* createNode(KEY _key, DATA _data)
	{

		shared_ptr<tree_node<KEY, DATA>> spo(new tree_node<KEY, DATA>);
		spo->Key = _key;
		spo->data = _data;

		// no parent == root
		spo->m_oParent = this;

		if (!this->m_oParent)
			spo->m_poRoot = this;
		else
			spo->m_poRoot = this->m_poRoot;


		// add to children list
		m_mapChildren[_key] = spo;

		return spo.get();










		//tree_node<KEY, DATA>* po = new tree_node;
		//po->Key = _key;
		//po->data = _data;

		//if (!this->m_oParent)
		//{
		//	po->m_oParent = nullptr;
		//	//po->m_poRoot;
		//}
		//else
		//{
		//	po->m_oParent = this->m_oParent;
		//	po->m_poRoot = this->m_poRoot;
		//}

		//// add to children list
		//m_mapChildren[Key] = po;

		//return po;
	}

	tree_node<KEY, DATA>* createNode(KEY _key)
	{
		// add to children list		
		shared_ptr<tree_node<KEY, DATA>> spo(new tree_node<KEY, DATA>);
		m_mapChildren[_key] = spo;

		spo->Key = _key;
		//spo->data = nullptr;
		spo->bHasData = false;

		// no parent == root
		spo->m_oParent = this;

		if (!this->m_oParent)
			spo->m_poRoot = this;
		else
			spo->m_poRoot = this->m_poRoot;
			
		// do this to avoid "allocated on the stack error
		return spo.get();
	}


	//tree_node* createNodes(vector<KEY> _path, DATA _data)
	//{
	//	tree_node* po = m_poRoot;
	//	for (auto it : _path)
	//	{
	//		it-
	//	}

	//}

	// if can't find key in child map, creat newr
	tree_node<KEY, DATA>* getOrCreate(KEY _key)
	{

		auto it = m_mapChildren.find(_key);
		if (it == m_mapChildren.end())
			return createNode(_key);


		// return exiting
		return it->second.get();
	}




	tree_node* getParent()
	{
		return m_oParent;
	}

	size_t getChildCount()
	{
		return m_mapChildren.size();
	}

	tree_node<KEY, DATA>* getChild(KEY key)
	{
		auto it = m_mapChildren.find(key);
		if (it != m_mapChildren.end())
			return it->second.get();

		return nullptr;
	}

	tree_node<KEY, DATA>* getFirstChild()
	{
		auto it = m_mapChildren.begin();
		if (it != m_mapChildren.end())
			return it->second.get();

		return nullptr;
	}

	vector<tree_node*>* getChildrenOpt()
	{
		return &m_vecChildrenOptimized;
	}

	
	
	

	map<KEY, shared_ptr<tree_node<KEY, DATA>>>* getChildren()
	{		
		return &m_mapChildren;
	}

	tree_node<KEY, DATA>* findChild(KEY _key)
	{
		for (auto it = m_mapChildren.begin(); it != m_mapChildren.end(); it++)
		{
			if (tools::compare<KEY>(it->first, _key))
				return it->second.get();
		}	

		return nullptr;
	}

	


	vector<DATA> findKey(KEY _key)
	{
		m_vecTemp.clear();
		_findKey(_key);



	}

	/*vector<DATA> _findKey(KEY _key)
	{
		for (auto& it : m_mapChildren)
		{
			if (it == _key)
				if (it.it.second)
					if (it.it.second->data)
						m_vecTemp.push_back(it.second->data);

			if (it.second)
				it.second->_findFile(_strFileName);

		}
	}*/
	// 123

public:
	inline bool isValid() { return m_bIsValid; };

	
	KEY Key;
	DATA Data = (DATA)0;
	
	string id = "";
	bool bIsRoot = false;
//protected:
	vector<DATA> m_vecTemp;
	tree_node<KEY, DATA>* m_oParent = nullptr;
	tree_node<KEY, DATA>* m_poRoot = nullptr;

	size_t m_uiNumberofChildren = 0;
	size_t m_dwChildLoopCounter = 0;

	vector<tree_node*> m_vecChildrenOptimized; // for "slow-compare" data type like string use this 0...num_children-1 numerical ID
	map<KEY, shared_ptr<tree_node<KEY, DATA>>> m_mapChildren;


private:
	bool m_bDatasSet = false;
	static map<string, tree_node<KEY, DATA>*> sm_mapRoot;
	bool m_bIsValid = false;
};

template <class KEY, class DATA>
map<string, tree_node<KEY, DATA>*> tree_node<KEY, DATA>::sm_mapRoot;

//template <class KEY, class DATA>
//shared_ptr<tree_node<KEY, DATA>> tree_node<KEY, DATA>::m_spoDummy = shared_ptr< tree_node<KEY, DATA>>(new tree_node<KEY, DATA>);





//template<class KEY, class DATA>
//inline vector<DATA>& tree_node<KEY, DATA>::findChildren(KEY start, KEY item_to_find)
//{
//	m_vecTemp.clear();
//	tree_node<KEY, DATA>* poTree = findChild(start);
//
//	if (!poTree)
//		return vector<DATA>();
//
//	return _seachTreeRecursive(poTree, item_to_find);	
//}
//
//template<class KEY, class DATA>
//inline vector<DATA>& tree_node<KEY, DATA>::_seachTreeRecursive(ret_type* _poParentTreeNoe, KEY item_to_find)
//{
//	for (auto it : m_mapChildren)
//	{
//		if (it.first == item_to_find)
//		{
//			m_vecTemp.push_back();
//
//			_seachTreeRecursive(it, item_to_find); // search into all brachnes of the tree
//		}		
//	}
//	
//	return m_vecTemp;
//}

