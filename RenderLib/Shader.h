#pragma once

// d3d / COM
#include <d3d11.h>
#include <wrl\client.h>

#include <string>
#include <memory>
#include "vertex_layouts.h"

namespace RenderLib
{
	namespace Shader
	{
		class Shader
		{
		public:
			Shader() {};

			static std::shared_ptr<Shader> create(

				ID3D11Device* _poDevice,

				const D3D11_INPUT_ELEMENT_DESC* _poLayout,
				size_t _layout_elements,

				const std::wstring& _strPathVertexShader,
				const std::wstring& _strPathPixelShader

			);

			Shader(
				ID3D11Device* _poDevice,

				const D3D11_INPUT_ELEMENT_DESC* _poLayout,
				size_t _layout_elements,

				const std::wstring& _strPathVertexShader,
				const std::wstring& _srtPathPixelShader
			);

			Shader(
				ID3D11Device* _poDevice,

				const D3D11_INPUT_ELEMENT_DESC* _poLayout,
				size_t _layout_elements,

				const std::wstring& _strPathVertexShader
			);

			bool load(
				ID3D11Device* _poDevice,

				const D3D11_INPUT_ELEMENT_DESC* _poLayout,
				size_t _layout_elements,

				const std::wstring& _strPathVertexShader,
				const std::wstring& _srtPathPixelShader
			);

			ID3D11PixelShader* getPixelShader() { return m_cpoPixelShader.Get(); };
			ID3D11VertexShader* getVertexShader() { return m_cpoVertexShader.Get(); };
			ID3D11InputLayout* getInputLayout() { return m_cpoInputLayout.Get(); };

			/// <summary>
			/// Creates shader from compiled shader object file (.cso)
			/// </summary>
			/// <param name="_srtPath">path to .sco file</param>
			/// <returns>"true" if all operations succeeded</returns>
			virtual bool loadPixelShader(ID3D11Device* _poDevice, const std::wstring& _srtPath);
			virtual bool loadVertexShader(
				ID3D11Device* _poDevice,
				const D3D11_INPUT_ELEMENT_DESC* _poLayout,
				size_t _layout_elements,
				const std::wstring& _srtPath);

			// TODO: make memory loading of shader code

			//virtual bool loadPixelShader(ID3D11Device* _poDevice, void*, size_t);
			//virtual bool loadVertexShader(ID3D11Device* _poDevice, void*, size_t);

		private:
			// shaders
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_cpoVertexShader;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_cpoPixelShader;

			// input layout, describes the layout of 1 vertex, the data going into the vertex shader
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_cpoInputLayout;
		};
	}
}
