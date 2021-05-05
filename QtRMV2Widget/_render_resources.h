#include "..\RenderLib\IResourceContainer.h"
#pragma once

// STL
#include <map>
#include <stdexcept>

// d3d / COM
#include <d3d11.h>
#include <wrl\client.h>

// author
#include "..\RenderLib\res_tools.h"
#include <exception>

using namespace std;

namespace Resources
{
	
	enum class EPixelShaders : size_t;
	enum class EVertexShader : size_t;
	enum class ETextures : size_t;

	class IResourceContainer
	{
	public:
	


		bool loadAll(ID3D11Device* _poDevice)
		{
			loadPixelShaders(_poDevice);
			loadVertexShaders(_poDevice);
			loadTextures(_poDevice);

			return true;
		}



		bool loadTextures(ID3D11Device* _poDevice)
		{
			for (auto it : getmapTexturePaths())
			{
				ID3D11PixelShader* poPixelShader = nullptr;

				QString qstr = QString::fromStdString(it.second);
				
				std::wstring wstrPath = qstr.toStdWString();
				ID3D11ShaderResourceView* srv;
				HRESULT hr = DirectX::CreateDDSTextureFromFile(_poDevice, wstrPath.c_str(), NULL, &srv);
				assert(SUCCEEDED(hr));

				if (srv)
				{
					m_mapIdToTexuertComPtr[it.first] = srv;
				}
				else
					throw runtime_error("poInputLayou = NULL");
				
			}

			return true;
		}


		bool loadPixelShaders(ID3D11Device* _poDevice)
		{
			for (auto it : getmapPixelShaderPaths())
			{
				ID3D11PixelShader* poPixelShader = nullptr;

				if (res_tools::loadPIxelShaderFromDisk(_poDevice, it.second, &poPixelShader))
				{
					m_mapIdToPixelShaderComPtr[it.first] = poPixelShader;
				}
				else
					throw runtime_error("poInputLayou = NULL");

			}

			return true;
		}

		bool loadVertexShaders(ID3D11Device* _poDevice)
		{
			for (auto it : getmapVertexShaderPaths())
			{
				//D11PixelShader* poPixelShader = nullptr;
				ID3D11InputLayout* poInputLayout = nullptr;

				auto VSCreateReturn = res_tools::loadVertexShaderFromDisk2(_poDevice, it.second.strPath);
				if (!VSCreateReturn.first)
					throw runtime_error("tempRes.first= NULL");

				// save vertex shader
				m_mapIdVertexShaderComPtr[it.first] = VSCreateReturn.first;

				res_tools::createInputLayout2(_poDevice, &poInputLayout,
											  it.second.pInputLayoutDesc, it.second.layout_elelemnt_count, VSCreateReturn.second);

				if (poInputLayout)
					m_mapIdToInputLayoutComPtr[it.first] = poInputLayout;

				else
					throw runtime_error("poInputLayou = NULL");

				








			}

			return true;
		}

		ID3D11ShaderResourceView* getTexture(ETextures _index)
		{
			auto it = m_mapIdToTexuertComPtr.find(_index);
			if (it == m_mapIdToTexuertComPtr.end())
			{
				throw std::out_of_range("Error Loading Pixel Shader: Index is not in map");
			}
			else
				return it->second.Get();
		}


		ID3D11PixelShader* getPixelShader(EPixelShaders _index)
		{
			auto it = m_mapIdToPixelShaderComPtr.find(_index);
			if (it == m_mapIdToPixelShaderComPtr.end())
			{
				throw std::out_of_range("Error Loading Pixel Shader: Index is not in map");
			}
			else
				return it->second.Get();
		}

		ID3D11VertexShader* getVertexShader(EVertexShader _index)
		{
			auto it = m_mapIdVertexShaderComPtr.find(_index);
			if (it == m_mapIdVertexShaderComPtr.end())
			{
				throw std::out_of_range("Error Loading Vertex Shader: Index is not in map");
			}
			else
				return it->second.Get();
		}

		ID3D11InputLayout* getInputLayout(EVertexShader _index)
		{
			auto it = m_mapIdToInputLayoutComPtr.find(_index);
			if (it == m_mapIdToInputLayoutComPtr.end())
			{
				throw std::out_of_range("Error Loading Vertex Shader: Index is not in map");
			}
			else
				return it->second.Get();
		}


		struct VertexShaderData {
			const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc = nullptr;
			UINT layout_elelemnt_count = 0;
			std::string strPath = "";
		};

		virtual map<EPixelShaders, std::string> getmapPixelShaderPaths() = 0;
		virtual map<EVertexShader, VertexShaderData> getmapVertexShaderPaths() = 0;
		virtual map<ETextures, std::string> getmapTexturePaths() = 0;

		map<EPixelShaders, Microsoft::WRL::ComPtr<ID3D11PixelShader>> m_mapIdToPixelShaderComPtr;
		map<EVertexShader, Microsoft::WRL::ComPtr<ID3D11VertexShader>> m_mapIdVertexShaderComPtr;
		map<EVertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout>> m_mapIdToInputLayoutComPtr;
		map<ETextures, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_mapIdToTexuertComPtr;
		
	};



};



namespace Resources
{

	enum class EPixelShaders : size_t
	{
		eFlat,
		ePBRSpecGloss,
		eGrid
		//eShadedSimple,
		//ePBRSpecGloss,
		
	};

	enum class  EVertexShader : size_t
	{
		eSimple,
		eWeighted4

	};


	enum class  ETextures : size_t
	{
		eDefaultDiffuse,
		eDefaultSpecular,
		eDeFaultGlossMap,
		eDefaultNormal,
		eDefaultNoTexture,


	};



	class ResourceContainer : public IResourceContainer
	{
	public:

		


	private:

		map<ETextures, std::string> getmapTexturePaths()
		{
			return
			{
				{ETextures::eDefaultDiffuse, R"(tex\default_diffuse.dds)"},
				{ETextures::eDeFaultGlossMap, R"(tex\default_gloss_map.dds)"},
				{ETextures::eDefaultNormal, R"(tex\default_normal.dds)"},
				{ETextures::eDefaultSpecular, R"(tex\default_specular.dds)"},
				{ETextures::eDefaultNoTexture, R"(tex\no_texture.dds)"},
				
			};
		} 





		map<EPixelShaders, std::string> getmapPixelShaderPaths()
		{
			return 			
			{
				{EPixelShaders::eFlat, R"(C:\Coding\QtRME_GUI\RenderLib\shaders\PS_Simple.cso)"},
				{EPixelShaders::eGrid, R"(C:\Coding\QtRME_GUI\RenderLib\shaders\PS_Grid.cso)"},
				{EPixelShaders::ePBRSpecGloss, R"(C:\Coding\QtRME_GUI\RenderLib\shaders\PS_PbrSpecGloss.cso)"}
			};
		}

		


		map<EVertexShader, VertexShaderData> getmapVertexShaderPaths()
		{
			return
			{
				{EVertexShader::eSimple, {layout_Weighted4, _countof(layout_Weighted4), R"(C:\Coding\QtRME_GUI\RenderLib\shaders\VS_Simple.cso)"} },
				{EVertexShader::eWeighted4, {layout_Weighted4, _countof(layout_Weighted4), R"(C:\Coding\QtRME_GUI\RenderLib\shaders\VS_Weighted4.cso)"} }
			};
		}


	};
};