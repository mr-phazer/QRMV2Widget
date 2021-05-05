#include "res_tools.h"

SystemLib::return_value res_tools::loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath, ID3D11PixelShader** ppDest)

{
	size_t pixel_shader_code_size = tools::getFileSize(_strPath);

	auto spPixelShaderCodeRaw = std::shared_ptr<uint8_t>(new uint8_t[pixel_shader_code_size],
		[](uint8_t* p)
		{
			delete p;
		}
	);

	std::ifstream in(_strPath, std::ios::binary);
	in.read((char*)spPixelShaderCodeRaw.get(), pixel_shader_code_size);
	in.close();

	HRESULT hr = _poDevice->CreatePixelShader(
		spPixelShaderCodeRaw.get(),
		pixel_shader_code_size,
		nullptr,
		ppDest
	);

	assert(SUCCEEDED(hr));

	return hr;
}

SystemLib::return_value res_tools::loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::wstring& _strPath, ID3D11PixelShader** ppDest)
{
	size_t pixel_shader_code_size = tools::getFileSize(_strPath);

	auto spPixelShaderCodeRaw = std::shared_ptr<uint8_t>(new uint8_t[pixel_shader_code_size],
		[](uint8_t* p)
		{
			delete p;
		}
	);

	std::ifstream in(_strPath, std::ios::binary);
	in.read((char*)spPixelShaderCodeRaw.get(), pixel_shader_code_size);
	in.close();

	HRESULT hr = _poDevice->CreatePixelShader(
		spPixelShaderCodeRaw.get(),
		pixel_shader_code_size,
		nullptr,
		ppDest
	);

	assert(SUCCEEDED(hr));

	return hr;
}

std::pair<ID3D11VertexShader*, vector<uint8_t>> res_tools::loadVertexShaderFromDisk2(ID3D11Device* _poDevice, const std::string& _strPath)
{
	size_t vertex_shader_code_size = tools::getFileSize(_strPath);

	std::pair<ID3D11VertexShader*, vector<uint8_t>> tempPair;
	tempPair.second.resize(vertex_shader_code_size);

	// open vertex shader file
	ifstream in(_strPath, ios::binary);

	in.read((char*)&tempPair.second[0], vertex_shader_code_size);
	in.close();

	HRESULT hr = _poDevice->CreateVertexShader(
		&tempPair.second[0],
		vertex_shader_code_size,
		nullptr,
		&tempPair.first
	);

	return tempPair;
}

std::pair<ID3D11VertexShader*, vector<uint8_t>> res_tools::loadVertexShaderFromDisk(ID3D11Device* _poDevice, const std::wstring& _strPath)
{
	size_t vertex_shader_code_size = tools::getFileSize(_strPath);

	std::pair<ID3D11VertexShader*, vector<uint8_t>> tempPair;
	tempPair.second.resize(vertex_shader_code_size);

	// open vertex shader file
	ifstream in(_strPath, ios::binary);

	in.read((char*)&tempPair.second[0], vertex_shader_code_size);
	in.close();

	HRESULT hr = _poDevice->CreateVertexShader(
		&tempPair.second[0],
		vertex_shader_code_size,
		nullptr,
		&tempPair.first
	);

	return tempPair;
}

SystemLib::return_value res_tools::createInputLayout2(
	ID3D11Device* _poDevice,
	ID3D11InputLayout** ppInputLayout,
	const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc,
	UINT layout_elelemnt_count,
	vector<uint8_t> _vecCodeRaw)

{
	HRESULT hr = _poDevice->CreateInputLayout(pInputLayoutDesc, layout_elelemnt_count, (void*)&_vecCodeRaw[0],
		_vecCodeRaw.size(), ppInputLayout);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

SystemLib::return_value res_tools::createVertexBuffer(ID3D11Device* _poDevice, ID3D11Buffer** ppoVertexBuffer, const void* _pvData, UINT _data_size, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags)
{
	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = Usage;
	vertexBufferDesc.ByteWidth = _data_size;
	vertexBufferDesc.BindFlags = BindFlags;
	vertexBufferDesc.CPUAccessFlags = CPUAccessFlags;
	vertexBufferDesc.MiscFlags = //0
		vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = _pvData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &vertexData, ppoVertexBuffer);
	assert(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}