#pragma once

#include <d3d11.h>

#include <wrl/client.h>

#include <map>
#include <string>

//#include "IDrawableNode.h"                                   r
#include "..\SystemLib\ISerializeabe.h"

#include "..\ImporterLib\IRigidModelmporter.h"
#include "..\ImporterLib\FBX_Reader.h"
#include "..\ImporterLib\RigidModelFileStructs.h"
#include "..\ImporterLib\RigidModelV2_Reader.h"
#include "..\ImporterLib\RigidModelV2_Enums.h"

#include "vertex_layouts.h"
#include "VariantMeshStructs.h"
#include "Shader.h"

namespace RenderLib
{
	class Scene;

	uint8_t* makeVertexBuffer_CINEMATIC(uint8_t* input, size_t vertex_size, size_t vertex_count);
	uint8_t* makeVertexBuffer_WEIGHTED(uint8_t* input, size_t vertex_size, size_t vertex_count);
	uint8_t* makeVertexBuffer_DEFAULT(ImporterLib::RigidModel::SubMeshHeader* _poSubmeshHeader, uint8_t* input, size_t vertex_size, size_t vertex_count, int32_t bone_index = -1);

	struct TextureData
	{
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> poD3D_shaderViewResource = nullptr;
		ID3D11Resource* poD3D_Texture = nullptr;
		ETextureType EType = ETextureType::eTEX_ERROR_NOT_SET;
		string strInternalPath = "";
		wstring wstrInternalPathh = L"";;
	};

	struct MaterialData
	{
		vector<TextureData> vecTextures;
		vector<ID3D11ShaderResourceView*> vecD3D_RawshaderVievResources;
		uint32_t PIsGammaRGB[64] = { 0 };

		uint32_t alpha = 0;
		uint32_t mask = 0;

		void setTexture(ETextureType _type, ID3D11ShaderResourceView* _srv)
		{
			size_t index = static_cast<size_t>(_type); // convert texture ENUM to index

			// only resize if index if out of bounds
			vecD3D_RawshaderVievResources.resize(index + 1 >= vecD3D_RawshaderVievResources.size() ? index + 1 : vecD3D_RawshaderVievResources.size());

			vecD3D_RawshaderVievResources[index] = _srv;
		}
	};

	class SubMeshData
	{
		friend class MeshLodData;
		friend class BaseNode;
		friend class GridNode;
		friend class MeshNode;
		friend class IDrawableNode;

	public:
		SubMeshData() {};
		SubMeshData(size_t _vertex_buffer_size, size_t _index_buffer_size);
		virtual ~SubMeshData();

		bool loadTextures(const MaterialInfo* _poMaterial);

		virtual void drawMesh(Scene* _poScene, Shader::Shader* _poShader = nullptr);

		SystemLib::return_value createVertexBuffer(
			ID3D11Device* _poDevice,
			UINT _data_size,
			const void* _pvData = nullptr,
			D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
			UINT _CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
		);

		SystemLib::return_value createIndexBuffer(
			ID3D11Device* _poDevice,
			UINT _data_size,
			const void* _pvData = nullptr,
			D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
			UINT _CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
		);

		HRESULT fillVertices(const void* _src, UINT _size);
		HRESULT fillIndices(const void* _src, UINT _size);

		ID3D11Buffer* vertexBuffer() { return m_cpVertexBuffer.Get(); };
		ID3D11Buffer* const* vertexBufferAddressReadOnly() const { return m_cpVertexBuffer.GetAddressOf(); };
		ID3D11Buffer** vertexBufferAddress() { return m_cpVertexBuffer.GetAddressOf(); };
		ID3D11RasterizerState* rasterizerState() { return m_poRasterizerState; };

		ID3D11Buffer* indexBuffer() const { return m_poIndexBuffer.Get(); };

		UINT indexCount() { return m_index_count; };
		void setIndexCount(UINT _i_count) { m_index_count = _i_count; };

		void setPixelShader(ID3D11PixelShader* _poPixelShader) { m_poPixelShader = _poPixelShader; };
		void setVertexShader(ID3D11VertexShader* _poVertexShader) { m_poVertexShader = _poVertexShader; };
		void setInputLayout(ID3D11InputLayout* _poInputLayout) { m_poInputLayout = _poInputLayout; };
		void setRasterizerState(ID3D11RasterizerState* _poRasterizerState) { m_poRasterizerState = _poRasterizerState; };

	public:
		D3D11_PRIMITIVE_TOPOLOGY ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		DXGI_FORMAT EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;

	private:
		// ---  vertices + indices ----

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_poIndexBuffer;

	public:
		UINT m_index_count = 0;

		// material (textures)
		MaterialData oMaterial;

		// rasterizer
		ID3D11RasterizerState* m_poRasterizerState = nullptr;;

		// shaders
		ID3D11VertexShader* m_poVertexShader = nullptr;
		ID3D11PixelShader* m_poPixelShader = nullptr;

		// input layout, describes the format of the data going into the vertex shader
		ID3D11InputLayout* m_poInputLayout = nullptr;

		DirectX::XMFLOAT3 vBBMin = { 0,0,0 };
		DirectX::XMFLOAT3 vBBMax = { 0,0,0 };
	};

	struct SubMeshes
	{
		vector<SubMeshData> vecspoMeshData;
	};

	class MeshLodData
	{
	public:
		string strName = "";
		string strPath = "";

		virtual ~MeshLodData()
		{
			int debug_1 = 1;
		}

		void setLodSettings(size_t _lod_count, const vector<float>& _vecSwitchPoints = vector<float>());

		const SubMeshes& operator[](size_t i) const;
		SubMeshes& operator[](size_t i);

		virtual void drawLOD(Scene* _poScene, Shader::Shader* _poShader = nullptr);

		virtual bool loadFromRigidModel(Scene* _poScene, ImporterLib::RigidModel::RigidModelFileCommon*, const VariantMeshInfo* _poMeshInfo = nullptr, int32_t _bone_index = -1);
		virtual bool loadFromRigidModel_new(Scene* _poScene, RigidModelV2::Common::CommonFile*, const VariantMeshInfo* _poMeshInfo = nullptr, int32_t _bone_index = -1);

		virtual bool loadFromFbxMeshes(Scene* _poScene, std::vector<FBX_Mesh>& m_vecMeshes);

		/// <summary>
		/// Loads an new set of texture in
		/// If there is a problem loading the speficied texture
		/// a default texture will be loaded
		/// </summary>
		/// <param name="lod"></param>
		/// <param name="group"></param>
		/// <param name="strPath"></param>
		/// <param name="type_index"></param>
		/// <param name=""></param>
		/// <returns></returns>
		virtual bool loadTexture(
			size_t lod,
			size_t group,
			const string& strPath,
			size_t type_index
		)
		{
			return false;
		}
		DirectX::XMFLOAT3 vBBMin = { 0,0,0 };
		DirectX::XMFLOAT3 vBBMax = { 0,0,0 };

		vector<SubMeshes> vecLODs;
	};
}