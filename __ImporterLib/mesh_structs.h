/*************************************************************************************************************
	FILE: mesh_structs.h
--------------------------------------------------------------------------------------------------------------
	AUTHOR: phazer(mr)
--------------------------------------------------------------------------------------------------------------
	TIME: 2018-2019
--------------------------------------------------------------------------------------------------------------
	DESCRIPTION: Data structure defintions for reading of 3d models. Raw and converted vertex structures.
	Plus not yet used experimental strcutures for the same purposes.
--------------------------------------------------------------------------------------------------------------
	DISCLAMINER: Usage and modification permissible for non-profit purposes only
	and then only with clear indication of original author

	For profit usage of the code is not permissible at all at this stage.

*************************************************************************************************************/
#pragma once

using namespace std;

//#include "..\EASY_GUI\Definitions.h"
//#include "..\EASY_GUI\text.h"
#include <fstream>

//#include <safe_enum.h>
//nclude "str_info.h"

//#include "..\RenderLib\lib3d.h"
#include "lib3d.h"
//#include "..\QtRMV2Widget\QConsole.h"

#include <DirectXMath.h>

#include "half.h"

using half_float::half;

typedef half float16;

struct ILastError {
	virtual bool ErrorString(const std::string& _strLastError, bool _bReturn = false) { m_strLastError = _strLastError; return _bReturn; };
	virtual const std::string& getLastErrorString() { return m_strLastError; };

protected:
	std::string m_strLastError;
};

struct IFile {
	virtual bool createFile(const std::string& _strFileName) = 0;

protected:
	std::ofstream m_ofsFile;
};

/// <summary>
/// interface for writing / reading stuff from disk
/// </summary>
struct ISerializable {
	virtual bool create(const std::string& _strFileName) { return false; };
	virtual bool write(std::ofstream& _ofsFile) { return false; };;
	virtual size_t read(std::ifstream& _ifsFile) { return 0; };
	virtual size_t size() = 0;

	constexpr static size_t _size() { return 0; };
};

struct readable_and_wite_vector : public ISerializable, public std::vector<std::shared_ptr<ISerializable>>
{
	virtual size_t data_size()
	{
		if (size() != 0)
			return size() * this[0].size();
		else
			return 0;
	}

	virtual size_t size()
	{
		return size();
	}

	virtual bool write(std::ofstream& _ofs)
	{
		for (auto& it : *this)
		{
			it->write(_ofs);
		}

		return true;
	}

	virtual size_t read(std::ifstream& _ifsInput) override
	{
		size_t bytes_read = 0;
		for (auto& it : *this)
		{
			bytes_read += it->read(_ifsInput);
		}

		return bytes_read;
	}

	/*void resize(const size_t size)
	{
		vecData.resize(size);
	}*/

	/*TYPE& operator[](size_t i)
	{
		return vecData[i];
	}

	const TYPE& operator[](size_t i) const
	{
		return vecData[i];
	}

	vector<TYPE> vecData;*/
	size_t stride = 0;
};

struct rgba
{
	uint8_t r, g, b, a;
};

/// <summary>
/// Union for storing packed / quantisizing
/// </summary>
union xyzw_8
{
	uint32_t data = 0;
	uint8_t x;
	uint8_t y;
	uint8_t z;
	uint8_t w;
};

enum class element_type : int
{
	POSITION,
	TEXTCOORD1,
	TEXTCOORD2,
	NORMAL,
	BITANGENT,
	TANGENT,
	BONE,
	INVALID = -1
};

struct i_vertex_raw_base
{
	virtual bool write(std::ofstream file) = 0;
	virtual size_t size() = 0;

	void setPosition(XMFLOAT3 _pos);
	void etNormal(XMFLOAT3 _pos);
	void setTangent(XMFLOAT3 _pos);
	void setBiTangent(XMFLOAT3 _pos);

	void setInfluences(std::vector< pair<int, float >> _influences);
};

struct vertex_element_base
{
	string name = "INVALID";

	element_type type = element_type::INVALID;
	uint size = 0;
	void* pDestination = nullptr;;
};

template <class TYPE1, class TYPE2>
struct vertex_element_xyzw : public vertex_element_base
{
public:
	uint read_size = sizeof(TYPE1) * 4;

	vertex_element_xyzw(element_type _type = element_type::INVALID, string textName = "")
	{
		type = _type;
		size = sizeof(raw);
		pDestination = &raw;

		switch (_type)
		{
		case element_type::POSITION:
			name = "POSITION"; break;

		case element_type::NORMAL:
			name = "NORMAL"; break;

		case element_type::BITANGENT:
			name = "BITANGENT"; break;

		case element_type::TANGENT:
			name = "TANGENT"; break;

		default:
			name = "INVALID";
		}
	};

	template <class TYPE1, class TYPE2>
	operator vertex_element_xyzw<TYPE1, TYPE2>()&
	{
		return *this;
	};

	struct {
		TYPE1 x = (TYPE1)0, y = (TYPE1)0, z = (TYPE1)0, w = (TYPE1)0;
	} raw;

	struct {
		TYPE2 x = (TYPE2)0, y = (TYPE2)0, z = (TYPE2)0, w = (TYPE2)0;
	} converted;

private:
	vertex_element_xyzw() {};
};

template <class TYPE1, class TYPE2>
struct vertex_element_xy : public vertex_element_base
{
	vertex_element_xy(element_type _type)
	{
		switch (_type)
		{
		case element_type::TEXTCOORD1:
			name = "TEXTCOORD1"; break;

		case element_type::TEXTCOORD2:
			name = "TEXTCOORD2"; break;

		case element_type::BONE:
			name = "BONE"; break;

		default:
			name = "INVALID";
		}

		type = _type;
		size = sizeof(raw);
		pDestination = &raw;
	};

	struct _raw {
		TYPE1 x, y;
	};
	_raw raw;

	struct _converted {
		TYPE2 x, y;
	};
	_converted converted;

	vertex_element_xy() {};
};

//
//struct vertex_element_POSITION : public vertex_element_xyzw<half, float>
//{
//
//public:
//	using vertex_element_xyzw<half, float>::vertex_element_xyzw;
//
//	vertex_element_POSITION()
//	{
//		type = element_type::POSITION;
//		size = sizeof(vertex_element_xyzw::_raw);
//		pDestination = &this->raw;
//	}
//};
//

template <int len>
struct vertex_data {
	vertex_element_base* vertex_elements[len];
};

struct vertex_container
{
	vertex_data<10> vertex;

	void make_default_vertex()
	{
	};

	int read_vertex()
	{
		vertex.vertex_elements[0] = new vertex_element_xyzw<half, float>(element_type::POSITION);
		vertex.vertex_elements[1] = new vertex_element_xyzw<half, float>(element_type::NORMAL);
		vertex.vertex_elements[2] = new vertex_element_xy<half, float>(element_type::TEXTCOORD1);

		if (vertex.vertex_elements[0]->type == element_type::POSITION)
		{
			vertex_element_xyzw<half, float>* p = static_cast<vertex_element_xyzw<half, float>*>(vertex.vertex_elements[0]);

			p->converted.x = 1.2f;
			p->converted.y = 1.3f;
			p->converted.z = 1.4f;

			int i = 1;
		}
		return 1;
	}
};

struct RMV2_Vertex_Raw_Default {
	float16 pos_x;
	float16 pos_y;
	float16 pos_z;
	float16 pos_w;

	float16 u;
	float16 v;  // 12

	float16 textcord_u2;
	float16 textcord_v2; // 16

	uint8_t normal_x;
	uint8_t normal_y;
	uint8_t normal_z;
	uint8_t normal_w; // 20

	uint8_t tangent_x;
	uint8_t tangent_y;
	uint8_t tangent_z;
	uint8_t tangent_w;

	// 24
	uint8_t bitangent_x;
	uint8_t bitangent_y;
	uint8_t bitangent_z;
	uint8_t bitangent_w; // 28

	rgba color = { 0,0,0,1 };

	bool write(ofstream& out, size_t _size)
	{
		/*size_t new_offset = out.tellp();
		uint16_t x;
		uint16_t y;
		uint16_t z;
		uint16_t w;

		memcpy(&x, &pos_x, 2);
		out.write((char*)&x, 2);

		memcpy(&y, &pos_y, 2);
		out.write((char*)&y, 2);

		memcpy(&z, &pos_z, 2);
		out.write((char*)&z, 2);

		memcpy(&w, &pos_w, 2);
		out.write((char*)&w, 2);

		*/
		//out.write((char*)&this[8], size-8);
		out.write((char*)this, _size);

		return true;
	}

	int32_t ui_last = 0;

	uint32_t filler[60];
};

struct RMV2_Vertex_Raw_Weighted
{
	RMV2_Vertex_Raw_Weighted()
	{
		ZeroMemory(this, sizeof(this));
	}

	half pos_x;
	half pos_y;
	half pos_z;
	half pos_w; // offset 8

	int8_t joint_id1;
	int8_t joint_id2; // offset 10p

	uint8_t weight1; // vertex->weight = uint8_t_t_norm / 255.0f;
//	vertex->weight1 = 1.0f - vertex->weight;

	uint8_t weight2; // offset 12

	uint8_t normal_x;
	uint8_t normal_y;
	uint8_t normal_z;
	uint8_t normal_w; // offet 16

	half u;
	half v; // offet 20

	uint8_t tangent_x;
	uint8_t tangent_y;
	uint8_t tangent_z;
	uint8_t tangent_w; // offet 28

	uint8_t bitangent_x;
	uint8_t bitangent_y;
	uint8_t bitangent_z;
	uint8_t bitangent_w; // offet 24

	uint8_t r = 0, g = 0, b = 0, a = 0; // offet 32

	bool write(ofstream& out, size_t _size)
	{
		out.write((char*)this, _size);

		return true;
	}
	//static constexpr size_t vertex_size = 28;
};

struct IVertexBase : public ISerializable
{
	constexpr static size_t _vertex_size = 0;
	/*constexpr static size_t _size()
	{
		return 0;
	}*/
};

struct RMV2_Vertex_Raw_Cinematic/* : public IVertexBase*/
{
	/*constexpr static size_t _vertex_size = 32;
	size_t size() { return (size_t)32; };
	static constexpr size_t _size = 32;*/

	RMV2_Vertex_Raw_Cinematic()
	{
		memset(this, sizeof(this), 0);
	};

	// offset 0
	half pos_x = half(0);
	half pos_y = half(0);
	half pos_z = half(0);
	half pos_w = half(1.0f);

	// offset 8
	uint8_t joint_id1 = 0;
	uint8_t joint_id2 = 0;
	uint8_t joint_id3 = 0;
	uint8_t joint_id4 = 0;

	// offset 12
	uint8_t weight1 = 255;
	uint8_t weight2 = 0;
	uint8_t weight3 = 0;
	uint8_t weight4 = 0;

	// 16
	uint8_t norm_x = 0;
	uint8_t norm_y = 0;
	uint8_t norm_z = 0;
	uint8_t norm_w = 0;

	// 20
	half u = half(0);
	half v = half(0);

	// 24
	uint8_t bitangent_x = 0;
	uint8_t bitangent_y = 0;
	uint8_t bitangent_z = 0;
	uint8_t bitangent_w = 0;

	// 28
	uint8_t tangent_x = 0;
	uint8_t tangent_y = 0;
	uint8_t tangent_z = 0;
	uint8_t tangent_w = 0;

	// 32
	rgba color = { 0, 0, 0, 1 };

	// 36

	bool write(ofstream& out, uint32_t _size)
	{
		/*size_t new_offset = out.tellp();
		uint16_t x;
		uint16_t y;
		uint16_t z;
		uint16_t w;

		memcpy(&x, &pos_x, 2);
		out.write((char*)&x, 2);

		memcpy(&y, &pos_y, 2);
		out.write((char*)&y, 2);

		memcpy(&z, &pos_z, 2);
		out.write((char*)&z, 2);

		memcpy(&w, &pos_w, 2);
		out.write((char*)&w, 2);

		*/
		//out.write((char*)&this[8], size-8);

		out.write((char*)this, _size);

		return true;
	}

	//static constexpr size_t vertex_size = 32;

	static RMV2_Vertex_Raw_Cinematic oCinematicRaw;
};

struct RMV2_Vertex_Raw_Grass
{
	//	vertex_<20> f;

	half pos_x;
	half pos_y;
	half pos_z;
	half pos_w;

	float u, v;
	//8

	uint8_t normal_x;
	uint8_t normal_y;
	uint8_t normal_z;	//19	//char unknown4_1;
	uint8_t normal_w;

	uint8_t face_normal_x;
	uint8_t face_normal_y;
	uint8_t face_normal_z;	//19	//char unknown4_1;
	uint8_t face_normal_w;

	uint8_t r, g, b, a;

	char filler[60];
};

struct RMV2_Vertex_Raw_Default_1
{
	//	vertex_<20> f;

	char uk1[3];

	//6
	half pos_x;
	half pos_y;
	half pos_z;
	//12
	half u;
	half v;
	//16
	uint8_t normal_x;
	uint8_t normal_y;
	uint8_t normal_z;
	uint8_t normal_w;
	//20

	uint8_t bitangent_x;
	uint8_t bitangent_y;
	uint8_t bitangent_z;
	uint8_t bitangent_w;
	//24
	////	char unknown5_1;
	//
	////24
	//	uint8_t tangent_x;
	//	uint8_t tangent_y;
	//	uint8_t tangent_z;
	//	uint8_t tangent_w;
	//// 27
	//	//char unknown7_6[4]; // if size 32 read 5 uint8_ts, if size 28 read 1 uint8_t.
	//	half	uk1;
	//	half	uk2;
	//
	//// 32
	//
	char filler[60];
};

struct RMV2_Vertex_Common_RAW
{
	RMV2_Vertex_Common_RAW()
	{
		memset(this, sizeof(this), 0);
	};

	half pos_x = half(0);
	half pos_y = half(0);
	half pos_z = half(0);
	half pos_w = half(1.0f);

	uint8_t joint_id1 = 0;
	uint8_t joint_id2 = 0;
	uint8_t joint_id3 = 0;
	uint8_t joint_id4 = 0;

	uint8_t weight1 = 255;
	uint8_t weight2 = 0;
	uint8_t weight3 = 0;
	uint8_t weight4 = 0;

	uint8_t normal_x = 0;
	uint8_t normal_y = 0;
	uint8_t normal_z = 0;
	uint8_t normal_w = 0;

	half u = half(0);
	half v = half(0);

	uint8_t tangent_x = 0;
	uint8_t tangent_y = 0;
	uint8_t tangent_z = 0;
	uint8_t tangent_w = 0;

	uint8_t bitangent_x = 0;
	uint8_t bitangent_y = 0;
	uint8_t bitangent_z = 0;
	uint8_t bitangent_w = 0;

	rgba color = { 0,0,0,0 };

	RMV2_Vertex_Raw_Default& getDefaultRaw();
	RMV2_Vertex_Raw_Weighted& getWeighted();
	RMV2_Vertex_Raw_Cinematic& getCinematicRaw();

	static RMV2_Vertex_Raw_Default oDefaultRawVertex;
	static RMV2_Vertex_Raw_Cinematic oCinematicRawVertex;
	static RMV2_Vertex_Raw_Weighted oWeightedRawVertex;;
};

struct RMV2_Vertex_Common
{
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT2 textcords = { 0.0f, 0.0f };
	DirectX::XMFLOAT3 normal = { 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT2 textcords2 = { 0.0f, 0.0f };
	DirectX::XMFLOAT3 tangent = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 bitangent = { 0.0f, 0.0f, 0.0f };

	//uint32_t dwNumberOfJoints;

	uint8_t PBone_Ids[4] = { 0,0,0,0 };
	float	PVertex_Weights[4] = { 1.0f, 0.0f, 0.f, 0.0f };

	RMV2_Vertex_Common()
	{
	}

	/*RMV2_Vertex_Common(const RMV2_Vertex_Common& value)
	{
		this->position = value.position;
	}*/

	//float vertexWeight1 = 1.0f;
	//float vertexWeight2 = 0.0f;
	//float vertexWeight3 = 0.0f;
	//float vertexWeight4 = 0.0f;

	//pair<int, float> PInflunces[10];

	int i = 0;

	void addInfluence(int bone_id, float weight) {
		vecInfluences.insert(vecInfluences.begin(), { bone_id, weight });
	}

	// assignment operator overload
	/*const RMV2_Vertex_Common& operator=(RMV2_Vertex_Common& value)
	{
		this->position = value.position;

		return *this;
	}

	const RMV2_Vertex_Common& operator-(RMV2_Vertex_Common& value)
	{
		RMV2_Vertex_Common temp;
		temp.position = this->position;

		temp.position.x -= value.position.x;
		temp.position.y -= value.position.y;
		temp.position.z -= value.position.z;

		return temp;
	}
*/

	std::vector<pair<int, float>> vecInfluences = { {-1, 0.0} };
	//uint32_t lod, groupo;

	//	vertex->weight = uint8_t_t_norm / 255.0f;
//	vertex->weight1 = 1.0f - vertex->weight;

	/// <summary>
	/// Get the "Cinematic" raw vertex format ready to write to disk, based on the contex of the "common veertex"
	/// </summary>
	/// <returns></returns>
	RMV2_Vertex_Common_RAW& getCommonRaw();

	//const RMV2_Vertex_Raw_Cinematic& getCinematicRaw2(vector<pair<int, float>>& _vecInfluences);

	/// <summary>
	/// Get the "Weighted" raw vertex format ready to write to disk, based on the contex of the "common veertex"
	/// </summary>
	/// <returns>Reference to raw cinematic struct ready to write</returns>
	//const RMV2_Vertex_Raw_Weighted& getWeightedRaw();

	/// <summary>
	/// Generate 1 raw vertex of the "default" format from the "common type" vertex
	/// this data is readu to wrute to disk
	/// </summary>
	/// <returns>reference to generated raw vertex</returns>
	//const RMV2_Vertex_Raw_Default& getDefaultRaw();

	/*--------------------------------------
		operator over loading

	--------------------------------------*/
	/*operator const RMV2_Vertex_Raw_Default& () { return getDefaultRaw(); }
	operator const RMV2_Vertex_Raw_Cinematic& () { return getCinematicRaw(); }
	operator const RMV2_Vertex_Raw_Weighted& () { return getWeightedRaw(); }*/

	//RMV2_Vertex_Raw_Default oDefaultVertex;
	//RMV2_Vertex_Raw_Cinematic oCinematicRaw;
	//RMV2_Vertex_Raw_Weighted otWeightedRaw;

	static RMV2_Vertex_Common_RAW oCommonRaw;

	static size_t null_weight_error_count;
};

//
//file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
//boneName.resize(nameLength);
//file.read(reinterpret_cast<char*>(&boneName[0]), nameLength);
//file.read(reinterpret_cast<char*>(&parentID), sizeof(parentID));
//if (parentID == -1)
//++mRootBonesCount;
//
//mBoneName.push_back(string(boneName.begin(), boneName.end()));
//mBoneParentID.push_back(parentID);
//if (parentID >= 0)
//++mChildBonesCount[parentID];
//

//void Mesh::read_weighted(ifstream& file, const FXMVECTOR& pivot, Vertex* vertex)
//{
//	half half_t_pos;
//	uint8_t uint8_t_t_norm;
//	XMFLOAT3 piv;
//	XMStoreFloat3(&piv, pivot);
//	file.read(reinterpret_cast<char*>(&t_pos), sizeof(t_pos));

//	vertex->position.x = -t_pos + piv.x;
//	file.read(reinterpret_cast<char*>(&t_pos), sizeof(t_pos));

//	vertex->position.y = t_pos + piv.y;
//	file.read(reinterpret_cast<char*>(&t_pos), sizeof(t_pos));

//	vertex->position.z = t_pos + piv.z;
//	file.seekg(2, ios_base::cur);

//	file.read(reinterpret_cast<char*>(&vertex->boneID0), sizeof(vertex->boneID0));
//	file.read(reinterpret_cast<char*>(&vertex->boneID1), sizeof(vertex->boneID1));
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->weight = uint8_t_t_norm / 255.0f;
//	vertex->weight1 = 1.0f - vertex->weight;
//	file.seekg(1, ios_base::cur);

//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->normal.x = -(uint8_t_t_norm / 127.0f - 1.0f);
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->normal.y = uint8_t_t_norm / 127.0f - 1.0f;
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->normal.z = uint8_t_t_norm / 127.0f - 1.0f;
//	XMStoreFloat3(&vertex->normal, XMVector3Normalize(XMLoadFloat3(&vertex->normal)));
//	file.seekg(1, ios_base::cur);
//
//	file.read(reinterpret_cast<char*>(&half_t_pos), sizeof(half_t_pos));
//	vertex->texCoord.x = half_t_pos;
//	file.read(reinterpret_cast<char*>(&half_t_pos), sizeof(half_t_pos));
//	vertex->texCoord.y = 1.0f - half_t_pos;
//
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->bitangent.x = -(uint8_t_t_norm / 127.0f - 1.0f);
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->bitangent.y = uint8_t_t_norm / 127.0f - 1.0f;
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->bitangent.z = uint8_t_t_norm / 127.0f - 1.0f;
//	XMStoreFloat3(&vertex->bitangent, XMVector3Normalize(XMLoadFloat3(&vertex->bitangent)));
//	file.seekg(1, ios_base::cur);
//
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->tangent.x = -(uint8_t_t_norm / 127.0f - 1.0f);
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->tangent.y = uint8_t_t_norm / 127.0f - 1.0f;
//	file.read(reinterpret_cast<char*>(&uint8_t_t_norm), sizeof(uint8_t_t_norm));
//	vertex->tangent.z = uint8_t_t_norm / 127.0f - 1.0f;
//	XMStoreFloat3(&vertex->tangent, XMVector3Normalize(XMLoadFloat3(&vertex->tangent)));
//	file.seekg(1, ios_base::cur);
//
