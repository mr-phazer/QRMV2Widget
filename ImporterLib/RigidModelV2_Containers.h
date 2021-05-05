#pragma once

#include <vector>
#include <memory>

#include "RigidModelV2_Data_Structs.h"

namespace RigidModelV2
{
	struct IContainer
	{
	protected:
		virtual void setFromData(uint8_t*) = 0;
		virtual uint8_t* getData() = 0;
	};

	template <typename T>
	struct TContainerType : public IContainer
	{
	protected:
		virtual uint8_t* getData() override {
			return reinterpret_cast<uint8_t*>(m_poData);
		};

		virtual void setFromData(uint8_t* _p) override {
			m_poData = reinterpret_cast<T*>(_p);
		};

		T* m_poData = nullptr;
	};

	struct IFileHeaderContainer
	{
		virtual const char* getMagicChars() { return ""; };
		virtual uint32_t getVersion() { return 0; };
		virtual uint32_t getLODCount() { return 0; };
		virtual const char* getSkeletonId() { return ""; };
	};

	struct FileHeaderContainer_V6_V7_V8 : public TContainerType<FileHeader_V6_V7_V8>, public IFileHeaderContainer
	{
		virtual const char* getMagicChars() override { return m_poData->oFileInfoHeader.Signature.szSignature; };
		virtual uint32_t getVersion() override { return m_poData->oFileInfoHeader.dwModelVersion; };
		virtual uint32_t getLODCount() override { return m_poData->wLod; };
		virtual const char* getSkeletonId() override { return m_poData->szSkeletonId; };
	};

	struct ILODHeaderEntryContainer
	{
		virtual uint32_t getGroupCount() { return 0; };
		virtual uint32_t getVerticesDataLength() { return 0; };
		virtual uint32_t getIndicesDataLength() { return 0; };
		virtual uint32_t getStartOffset() { return 0; };
		virtual float getVisibilityDistance() { return 0.f; };		// distance when engine switches to next LOD

		virtual uint32_t getAuthoredLodNumber() { return 0; };					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that

		virtual LodQualityLevel_V7_V8 getQualityLevel() { return { 0 }; }; // v7/v8 related to graphics setting "mesh quality"
	};

	struct LODHeader : public TContainerType<LodHeaderElement_V5_V6>, public ILODHeaderEntryContainer
	{
		virtual uint32_t getGroupCount() override { return m_poData->dwGroupCount; };
		virtual uint32_t getVerticesDataLength() override { return m_poData->dwVerticesDataLength; };
		virtual uint32_t getIndicesDataLength() override { return m_poData->dwIndicesDataLength; };
		virtual uint32_t getStartOffset() override { return m_poData->dwStartOffset; };
		virtual float getVisibilityDistance() override { return m_poData->fVisibilityDistance; };		// distance when engine switches to next LOD

		//virtual uint32_t getAuthoredLodNumber() { return 0; };					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that

		//virtual LodQualityLevel_V7_V8 getQualityLevel() { return { 0 }; }; // v7/v8 related to graphics setting "mesh quality"
	};

	/*template <typename T>
	struct TLODHeaderEntryContainter_Derived
	{
		typedef T ContainerType;

	protected:
		virtual void setFromData(uint8_t* _p) override {
			m_poData = reinterpret_cast<FileHeader_V6_V7_V8*>(_p);
		};

		T* m_poData = nullptr;
	};*/

	struct ILODHeaderContainer;

	struct IGroupHeaderContainer;

	struct AttachmentPointEntryCommon;
	struct AttachmentPointTableCommon;

	struct TextureEntryCommon;
	struct TextureTableCommon;


	struct VertexFloat32
	{
		DirectX::XMFLOAT4 position;
		
		DirectX::XMFLOAT2 uv1;
		DirectX::XMFLOAT2 uv2;
		
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT4 bitangent;
		
		DirectX::XMFLOAT4 bone_weights;
		DirectX::XMUINT4 bone_indices;
		
		DirectX::XMFLOAT4 color;

	};

	struct IVertexBlockContainerBase
	{
		virtual DirectX::XMFLOAT4 getPosition(size_t _vertex);
		virtual DirectX::XMFLOAT2 getUV1(size_t _vertex);
		virtual DirectX::XMFLOAT2 getUV2(size_t _vertex);
		virtual DirectX::XMFLOAT4 getNormal(size_t _vertex);
		virtual DirectX::XMFLOAT4 getTangent(size_t _vertex);
		virtual DirectX::XMFLOAT4 getBitangent(size_t _vertex);
		virtual DirectX::XMFLOAT4 getBoneWeights(size_t _vertex);
		virtual DirectX::XMUINT4 getBoneIndices(size_t _vertex);
		virtual DirectX::XMFLOAT4 getColor(size_t _vertex);

		virtual bool allocate(size_t _num_of_vertices);

		// for storage
		std::vector<uint8_t> getDefault_Block_RAW();
		std::vector<uint8_t> getWeighedt_Block_RAW();
		std::vector<uint8_t> getCinematic_RAW();

		// get common vector using "float" for all fields
		// contains 2x UV, bone weigts + indices, etc
		std::vector<VertexFloat32> getCommonFloat();

		
	};

	template <typename T>
	struct VertexBlockContainer_T : public IVertexBlockContainerBase
	{
	protected:
		T* m_poVertexData;

	};

	



	struct IndexBlockCommon;

	/*------------------------------------------------------------------*/
	struct IFileContainer
	{
		IFileHeaderContainer* getFileHeader() { return m_spoHeader.get(); };

	protected:
		std::shared_ptr<IFileHeaderContainer> m_spoHeader;
	};
};