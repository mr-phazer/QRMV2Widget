#include "Shader.h"
#include "res_tools.h"

std::shared_ptr<RenderLib::Shader::Shader> RenderLib::Shader::Shader::create(ID3D11Device* _poDevice, const D3D11_INPUT_ELEMENT_DESC* _poLayout,
	size_t _layout_elements,
	const std::wstring& _strPathVertexShader,
	const std::wstring& _strPathPixelShader)
{
	Shader* po = new Shader();

	po->load(_poDevice, _poLayout, _layout_elements,
		_strPathVertexShader,
		_strPathPixelShader);

	return std::shared_ptr<Shader>(po);
}

bool RenderLib::Shader::Shader::load(ID3D11Device* _poDevice, const D3D11_INPUT_ELEMENT_DESC* _poLayout, size_t _layout_elements,
	const std::wstring& _strPathVertexShader,
	const std::wstring& _strPathPixelShader)
{
	loadVertexShader(_poDevice, _poLayout, _layout_elements, _strPathVertexShader);
	loadPixelShader(_poDevice, _strPathPixelShader);

	return true;
}

bool RenderLib::Shader::Shader::loadPixelShader(ID3D11Device* _poDevice, const std::wstring& _srtPath)
{
	ID3D11PixelShader* poPixelShader = nullptr;

	res_tools::loadPIxelShaderFromDisk(_poDevice, _srtPath, &m_cpoPixelShader);

	if (!m_cpoPixelShader)
		throw runtime_error("poInputLayou = NULL");

	return true;
}

bool RenderLib::Shader::Shader::loadVertexShader(
	ID3D11Device* _poDevice,
	const D3D11_INPUT_ELEMENT_DESC* _poLayout,
	size_t _layout_elements,
	const std::wstring& _strPath)
{
	auto pairVSLoadReturn = res_tools::loadVertexShaderFromDisk(_poDevice, _strPath);
	if (!pairVSLoadReturn.first)
		throw runtime_error("tempRes.first= NULL");

	// save vertex shader
	m_cpoVertexShader = pairVSLoadReturn.first;

	res_tools::createInputLayout2(_poDevice, &m_cpoInputLayout,
		_poLayout, _layout_elements, pairVSLoadReturn.second);

	if (!m_cpoInputLayout)
		throw runtime_error("poInputLayou = NULL");

	return true;
}