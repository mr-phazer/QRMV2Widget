#pragma once

#include "IAnimImporter.h"
#include "..\SystemLib\ISerializeabe.h"
#include <vector>
#include <map>

using namespace std;

namespace ImporterLib
{
	class anim_file;

	struct bone_table_element
	{
		string strBoneName = "";
		string strBoneType = "";
		int32_t sort_order = 0;
		int32_t optional_flags = 0;
	};


	struct bone_table
	{
	public:		
		bool read(const wstring& _strFileName);

		bool getBonesFrom_BONE_TABLE(const string& _strFileName);
		bool getBonesFrom_ANIM_FILE(const string& _strFileName);

		int32_t getIdFromName(const string& _strBoneName);

		const std::vector<bone_table_element>& getBoneTable() const;

	private:
		bool readFromBoneTable(const wstring& _strFileName);

		std::vector<string> vecBones;
		std::vector<bone_table_element> m_vecBoneTableElements;
		map<string, int32_t> mapStringToId;

	private:
		string getNextBone();

		char* pchBuffer = nullptr;
		uint32_t  dwBufferIndex = 0;
		uint32_t  dwBufferSize = 0;
	};

}