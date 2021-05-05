#include "bone_table.h"
#include <fstream>
#include "..\SystemLib\tools.h"
//#include "anim_file.h"

using namespace ImporterLib;

bool ImporterLib::bone_table::read(const wstring& _wstrFileName)
{
	auto wstr = tools::lower(tools::GetExtensionFromFullPath(_wstrFileName));

	if (tools::lower(tools::GetExtensionFromFullPath(_wstrFileName)) == L"bone_table");
		return readFromBoneTable(_wstrFileName);

	return false;
}

bool ImporterLib::bone_table::getBonesFrom_BONE_TABLE(const string& _strFileName)
{
	ifstream input_file(_strFileName);

	if (!input_file.is_open())
		return false;

	dwBufferSize = tools::getFileSize(_strFileName);

	pchBuffer = new char[dwBufferSize];
	input_file.read(pchBuffer, dwBufferSize);
	input_file.close();
		
	// populate the vector with bone names
	while (dwBufferIndex < dwBufferSize)
	{
		string strBoneName = getNextBone();

		if (strBoneName != "")
		{
			// next id = current size of name vector			
			mapStringToId[strBoneName] = vecBones.size();;
			vecBones.push_back(strBoneName);
		}
	}	
	
	delete[] pchBuffer;

	return true;
}

bool ImporterLib::bone_table::getBonesFrom_ANIM_FILE(const string& _strFileName)
{
	/*mapStringToId.clear();
	vecBones.clear();	

	unique_ptr<anim_file> spoAnimFile(new anim_file);

	spoAnimFile->readFileInfo(_strFileName);

	for (size_t bone = 0; bone < spoAnimFile->m_file.oHeader.dwJointCount; bone++)
	{
		mapStringToId[spoAnimFile->m_file.PJoints[bone].szName] = bone;
		vecBones.push_back(spoAnimFile->m_file.PJoints[bone].szName);
	}
	spoAnimFile->close();*/

	

	return true;
}

int32_t  ImporterLib::bone_table::getIdFromName(const string& _strBoneName)
{

	auto it = mapStringToId.find(tools::lower(_strBoneName));

	// if name is not known return id = -1
	if (it == mapStringToId.end())
		return -1;

	return it->second;
}

const vector<bone_table_element>& ImporterLib::bone_table::getBoneTable() const
{
	return m_vecBoneTableElements;
}

bool ImporterLib::bone_table::readFromBoneTable(const wstring& _strFileName)
{
	std::ifstream file(_strFileName);

	
	for (string strLine = ""; std::getline(file, strLine); )
	{		
		bone_table_element element;
		strLine = tools::lower(strLine);

		if (strLine == "")
			continue;

		if (strLine.compare(0, 2, "//", 0, 2) == 0)
			continue;

		if (strLine.size() >= strlen("version"))
		if (strLine.compare(0, strlen("version"), "version", 0, strlen("version")) == 0)
			continue;

		if (strLine.size() >= strlen("reference_skeleton"))
		if (strLine.compare(0, strlen("reference_skeleton"), "reference_skeleton", 0, strlen("reference_skeleton")) == 0)
			continue;

		if (strLine.size() >= strlen("cinematic"))
		if (strLine.compare(0, strlen("cinematic"), "cinematic", 0, strlen("cinematic")) == 0)
			continue;
		
		size_t i = 0;
		for (; i < strLine.length(); i++)
		{
			if (strLine[i] == ',')
				break;
			 
			element.strBoneName += strLine[i];
		}

		i++;
		while (strLine[i] == ' ' || strLine[i] == '\t') i++;

		for (; i < strLine.length(); i++)
		{
			if (strLine[i] == ',')
				break;

			element.strBoneType += strLine[i];
		}

		i++;
		while (strLine[i] == ' ' || strLine[i] == '\t') i++;
			
		string strSortOrder = "";

		for (; i < strLine.length(); i++)
		{
			if (strLine[i] == ',')
				break;

			strSortOrder += strLine[i];
		}
		
		element.sort_order = stoi(strSortOrder);

		m_vecBoneTableElements.push_back(element);
	}

	return true;
}


string ImporterLib::bone_table::getNextBone()
{	
	string m_strName = "";
	
	if (dwBufferIndex + 1 >= dwBufferSize) // index+1 past the last char?
		return "";
	
	// A comment line, skip to next line, and return ""
	if (pchBuffer[dwBufferIndex] == '/' && pchBuffer[dwBufferIndex + 1] == '/')
	{ 	
		while (pchBuffer[dwBufferIndex++] != '\n');

		return "";
	}
	
	// run until a "terminating char" is found, or EOF
	while (pchBuffer[dwBufferIndex] != '\n' && pchBuffer[dwBufferIndex] != ',' && dwBufferIndex < dwBufferSize)
	{
		m_strName += pchBuffer[dwBufferIndex];

		dwBufferIndex++;
	}

	// a comman after a new-line means a bone bane in between
	if (pchBuffer[dwBufferIndex] == ',')
	{
		// fast forward to end of the line
		while (pchBuffer[dwBufferIndex] != '\n' && dwBufferIndex < dwBufferSize)
			dwBufferIndex++;

		return m_strName;
	}
	 
	dwBufferIndex++;
	// return empty
	return "";
}
