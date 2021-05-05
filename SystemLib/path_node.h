#pragma once

#include <memory>

#include "tree_node.h"
#include "..\ImporterLib\pack_file_structs.h"

using namespace std;

struct path_node : public tree_node<string, shared_ptr<pack_file_node>>
{
public:
	path_node()
	{
		Key = "";
		Data = nullptr;
	}

	vector<shared_ptr<pack_file_node>> searchForFiles(const std::string& _pathStart, const std::string& _item_to_find = "", const string& _srtExt = "");

private:
	void _searchForFilesRecursive(const std::string& _item_to_find, const string& _strExt, vector<shared_ptr<pack_file_node>>& vec);
	vector<shared_ptr<pack_file_node>> vecTemp;
public:

	bool bIsModded = false;

	static path_node* createRoot(const std::string& strID)
	{
		path_node* po = new path_node;
		sm_mapRoot[strID] = po;
		po->id = strID;
		po->bIsRoot = true;
		po->m_poRoot = po;

		return po;
	}

	bool isFolder()
	{
		return !((bool)Data);
	}

	bool isFile()
	{
		return ((bool)Data);
	}

	path_node* createPath(const std::string& _strPath);

	path_node* createPath(char** _Psz, size_t num_of_string);

	path_node* getPath(const std::string& _strPath);

	//vector<pack_file_node*> findFile(const std::string& _strFileName)
	//{
	//	vecTemp.clear();

	//}

	//vector<pack_file_node*> vecTemp;

	//vector<pack_file_node*> _findFile(const std::string& _strFileName)
	//{
	//}

	/// <summary>
	/// Return the path string of the node by traversing backward in the tree
	/// </summary>
	/// <returns>Path in the format "folder1\folder2\file.txt"</returns>
	string getPathString()
	{
		auto m_poParent = this->m_oParent;
		string strPath = Key;
		// ítarate backwards through the tree using the parent ptr
		while (m_poParent)
		{
			strPath.insert(0, m_poParent->Key + "\\"); // insert the folder name before the existin folder name
			m_poParent = m_poParent->m_oParent;
		}

		strPath.erase(0, 1);

		return strPath;
	}

	path_node* setData(const std::string& _strPath, shared_ptr<pack_file_node> po)
	{
		path_node* poNode = createPath(_strPath);
		if (poNode)
		{
			poNode->Data = po;
			poNode->bHasData = true;
		}
		else
			return nullptr;

		return poNode;
	}

	path_node* setData(char** _Psz, size_t num_of_string, shared_ptr<pack_file_node> po)
	{
		path_node* poNode = createPath(_Psz, num_of_string);
		if (poNode)
		{
			poNode->Data = po;
			poNode->bHasData = true;
		}
		else
			return nullptr;

		return poNode;
	}

	path_node* setData(char** _Psz, size_t num_of_string, shared_ptr<pack_file_node> spoPFN, bool _bOverWrite)
	{
		//	_bOverWrite = false; // set the init value = "not overidding"
		path_node* poNode = createPath(_Psz, num_of_string);
		if (poNode)
			if (poNode->Data) // ´There is existing Data
			{
				if (_bOverWrite) // shall existing Data be overridding?
					if (poNode)
					{
						poNode->Data = spoPFN;
						poNode->bHasData = true;
						//				_bOverWrite = true; // signal back that this node has been overridden
					}
			}
			else
			{
				poNode->Data = shared_ptr<pack_file_node>(spoPFN);
				poNode->bHasData = true;
			}

		return poNode;
	}

	bool getAllFilesInFolder(vector<const pack_file_node*> _vecFiles, const std::string& _strExtention = "")
	{
		for (auto it : m_mapChildren)
		{
			if (tools::lower(tools::GetExtensionFromFullPath(it.first)) == _strExtention || _strExtention == "")
				_vecFiles.push_back(it.second->Data.get());
		}
		return true;
	}

	bool getAllFilesInSubdFolders(ret_type* node, vector<pack_file_node*>& _vecFiles, const std::string& _strExtention = "")
	{
		for (auto it : node->m_mapChildren)
		{
			if (tools::lower(tools::GetExtensionFromFullPath(it.first)) == _strExtention || _strExtention != "")
				_vecFiles.push_back(it.second->Data.get());

			getAllFilesInSubdFolders(it.second.get(), _vecFiles, _strExtention);
		}
		return true;
	}

	path_node* getPathNode(const std::string& _strPath)
	{
		string strTemp = tools::getLowerAndBackSlash(_strPath);

		ret_type* poTreeNode = m_poRoot;

		/*if (!poTreeNode)
			ret_err("path_node::getNodeFromInternalPath():: oTreeNode == nullptr");*/

		m_strTempPath = strTemp;
		m_index = 0;
		string strFolder = "";
		do {
			strFolder = tools::lower(getNextFolderName());

			if (strFolder == "")
			{
				return static_cast<path_node*>(poTreeNode);
			}

			poTreeNode = poTreeNode->findChild(strFolder);

			if (!poTreeNode)
				return nullptr;
		} while (strFolder != "");

		if (!poTreeNode)
			return nullptr;
		else
			return static_cast<path_node*>(poTreeNode);
	}

	path_node* findChildPath(string _key)
	{
		for (auto it = m_mapChildren.begin(); it != m_mapChildren.end(); it++)
		{
			if (tools::lower(it->first) == tools::lower(_key))
			{
				return static_cast<path_node*>(it->second.get());
			}
		}

		return nullptr;
	}

	pack_file_node* getNodeFromInternalPath(const std::string& _strPath)
	{
		//string strTemp = tools::getLowerAndBackSlash(_strPath);
		string strTemp = tools::getLowerAndBackSlash(_strPath);

		//ee_node<string, shared_ptr<pack_file_node>>* poTreeNode =
		auto poTreeNode = static_cast<path_node*>(m_poRoot);
		//path_node* poTreeNode = static_cast< path_node* >( m_poRoot );

		//if (!poTreeNode)
		//	ret_err("path_node::getNodeFromInternalPath():: oTreeNode == nullptr");

		m_strTempPath = strTemp;
		m_index = 0;
		string strFolder = "";
		do {
			strFolder = tools::lower(getNextFolderName());

			if (strFolder == "")
			{
				return poTreeNode->Data.get();
			}

			poTreeNode = poTreeNode->findChildPath(strFolder);

			if (!poTreeNode)
				return nullptr;
		} while (strFolder != "");

		if (!poTreeNode)
			return nullptr;
		else
			return poTreeNode->Data.get();
	}

	path_node* getNodeFromInternalPath_IncludingFolders(const std::string& _strPath)
	{
		//string strTemp = tools::getLowerAndBackSlash(_strPath);

		//if (_strPath == "")
		//	return static_cast<path_node*>(m_poRoot);

		string strTemp = (_strPath);

		path_node* poTreeNode = static_cast<path_node*>(m_poRoot);
		path_node* poChild = nullptr;

		//if (!poTreeNode)
		//	ret_err("path_node::getNodeFromInternalPath():: oTreeNode == nullptr");

		m_strTempPath = strTemp;
		m_index = 0;
		string strFolder = "";

		do {
			strFolder = tools::lower(getNextFolderName());

			if (strFolder != "" && poTreeNode)
				poTreeNode = static_cast<path_node*>(poTreeNode->findChild(strFolder));
		} while (strFolder != "");

		return poTreeNode;
	}

	path_node* getNodeFromInternalPath_IncludingFolders_IgnoredLast(const std::string& _strPath)
	{
		//string strTemp = tools::getLowerAndBackSlash(_strPath);

		/*if (_strPath == "")
			return static_cast<path_node*>(m_poRoot);*/

		string strTemp = (_strPath);

		path_node* poTreeNode = static_cast<path_node*>(m_poRoot);
		path_node* poChild = nullptr;

		//if (!poTreeNode)
		//	ret_err("path_node::getNodeFromInternalPath():: oTreeNode == nullptr");

		m_strTempPath = strTemp;
		m_index = 0;
		string strFolder = "";

		do {
			strFolder = tools::lower(getNextFolderName());

			if (strFolder != "" && poTreeNode)
				poTreeNode = static_cast<path_node*>(poTreeNode->findChild(strFolder));
		} while (strFolder != "");

		return poTreeNode;
	}

private:
	string m_strTempPath;

	string m_strName = "";

	//tree_node<string, Data>::ret_type test_var;

	string m_strFolder = "";

	size_t m_index = 0;
	size_t m_start;
	size_t m_stop;
	size_t len;

	string& getNextFolderName();
};

inline void path_node::_searchForFilesRecursive(const std::string& _item_to_find, const string& _strExt, vector<shared_ptr<pack_file_node>>& vec)
{
	for (auto it : m_mapChildren)
	{
		if (_strExt != "")
		{
			if (tools::GetExtensionFromFullPath(tools::lower(it.second->Key)) == (tools::lower(_strExt)))
				vec.push_back(it.second->Data);
		}
		else if (_item_to_find != "")
		{
			if (tools::lower(it.second->Key) == (tools::lower(_item_to_find)))
				vec.push_back(it.second->Data);
		}

		static_cast<path_node*>(it.second.get())->_searchForFilesRecursive(_item_to_find, _strExt, vec);
	}
}

inline vector<shared_ptr<pack_file_node>> path_node::searchForFiles(const std::string& _pathStart, const std::string& _item_to_find, const string& _srtExt)
{
	//m_vecTemp.clear();

	vector<shared_ptr<pack_file_node>> vec;

	auto startnode = getPathNode(_pathStart);

	startnode->_searchForFilesRecursive(_item_to_find, _srtExt, vec);

	return vec;
}

inline path_node* path_node::createPath(const std::string& _strPath)
{
	m_index = 0;
	m_strTempPath = _strPath;
	//tree_node* poRoot = getRoot("INTERNAL_FOLDERS");
	// po = the current
	tree_node<string, shared_ptr<pack_file_node>>* po = m_poRoot;

	string strTemp = "";
	do {
		strTemp = getNextFolderName();

		// po = the child that has been create or found
		if (strTemp != "")
			po = po->getOrCreate(strTemp);
		else
			return (path_node*)po;
	} while (strTemp != "");

	return NULL;
}

inline path_node* path_node::createPath(char** _Psz, size_t num_of_string)
{
	tree_node<string, shared_ptr<pack_file_node>>* po = m_poRoot;
	for (size_t i = 0; i < num_of_string; i++)
	{
		po = po->getOrCreate(_Psz[i]);
	}

	return (path_node*)po;
}

inline path_node* path_node::getPath(const std::string& _strPath)
{
	m_strTempPath = _strPath;
	string strFolderName = "";

	// starta at the begining of the path
	m_index = 0;

	path_node* po = this;
	do {
		strFolderName = getNextFolderName(); // get next folder name
		if (strFolderName != "") // only search if folder name is not empty
		{
			po = static_cast<path_node*>(po->findChild(strFolderName)); // find folder, and set the new search point in THAT folder
			if (!po)
				return (path_node*)tree_node::m_spoDummy.get(); // folder not found
		}
		std::shared_ptr<int> spi = std::make_shared<int>();
	} while (strFolderName != "");

	return po;
}

inline string& path_node::getNextFolderName()
{
	m_strName = "";

	// add chars to folder name until a '\\' is hit hit or end of string
	while (m_strTempPath[m_index] != '\\' && m_strTempPath[m_index] != '/' && m_strTempPath[m_index] != '\0' && m_index < m_strTempPath.size())
	{
		m_strName += m_strTempPath[m_index++];
	}

	if (m_index < m_strTempPath.size())
		m_index++;

	// return folder name
	return m_strName;
}
