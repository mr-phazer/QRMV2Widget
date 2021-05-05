#include "IDrawableNode.h"

//SystemLib::return_value RenderLib::IDrawableNode::createInputLayou(ID3D11Device* _poDevice, const D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc, uint32_t size)
//{
//	HRESULT hr = _poDevice->CreateInputLayout(pInputLayoutDesc, size, spVerterShaderCodeRaw.get(),
//		vertex_shader_code_size, &poInputLayout);
//	assert(SUCCEEDED(hr));
//
//	return SUCCEEDED(hr);
//}
//
//
//SystemLib::return_value RenderLib::IDrawableNode::loadVertexShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath)
//{
//	vertex_shader_code_size = tools::getFileSize(_strPath);
//
//	spVerterShaderCodeRaw = shared_ptr<uint8_t>(new uint8_t[vertex_shader_code_size],
//		[](uint8_t* p)
//			{
//				delete p;
//			}
//	);
//
//	ifstream in(_strPath, ios::binary);
//	in.read((char*)spVerterShaderCodeRaw.get(), vertex_shader_code_size);
//	in.close();
//
//
//	HRESULT hr = _poDevice->CreateVertexShader(
//		spVerterShaderCodeRaw.get(),
//		vertex_shader_code_size,
//		nullptr,
//		&poVertexShader
//	);
//
//	assert(SUCCEEDED(hr));
//
//	/*if (size)
//		*size = dwSourceSize;
//
//	if (_poByteCode)
//		*_poByteCode = p;*/
//
//	return SUCCEEDED(hr);
//
//}
//SystemLib::return_value RenderLib::IDrawableNode::loadPIxelShaderFromDisk(ID3D11Device* _poDevice, const std::string& _strPath)
//{
//	pixel_shader_code_size = tools::getFileSize(_strPath);
//
//	spPixelShaderCodeRaw = shared_ptr<uint8_t>(new uint8_t[pixel_shader_code_size],
//		[](uint8_t* p)
//	{
//		delete p;
//	}
//	);
//
//
//	ifstream in(_strPath, ios::binary);
//	in.read((char*)spPixelShaderCodeRaw.get(), pixel_shader_code_size);
//	in.close();
//
//	HRESULT hr = _poDevice->CreatePixelShader(
//		spPixelShaderCodeRaw.get(),
//		pixel_shader_code_size,
//		nullptr,
//		&poPixelShader
//	);
//
//	assert(SUCCEEDED(hr));
//
//	return SUCCEEDED(hr);
//}
//

//SystemLib::return_value RenderLib::IDrawableNode::createVertexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size, ID3D11Buffer** _ppBuffer)
//{
//
//
//
//	// Set up the description of the static vertex buffer.
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	vertexBufferDesc.ByteWidth = _data_size;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = _pvData;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &vertexData, _ppBuffer);
//	assert(SUCCEEDED(hr));
//
//	return hr;
//}

bool RenderLib::IDrawableNode::init(Scene* _poScene)
{
	bool bResult = BaseNode::init(_poScene);

	allocateMeshDataContainer();

	return bResult;
}

RenderLib::IDrawableNode* RenderLib::IDrawableNode::create(Scene* _poScene, const string& _strName)
{
	auto poNode = new RenderLib::IDrawableNode(_strName);
	poNode->init(_poScene);
	poNode->allocateMeshDataContainer();

	return poNode;
}

void RenderLib::IDrawableNode::allocateMeshDataContainer()
{
}

//SystemLib::return_value RenderLib::IDrawableNode::createVertexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size,
//
//	D3D11_USAGE Usage,
//	UINT        BindFlags,
//	UINT        CPUAccessFlags
//
//
//)
//{
//
//
//
//	// Set up the description of the static vertex buffer.
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	vertexBufferDesc.Usage = Usage;
//	vertexBufferDesc.ByteWidth = _data_size;
//	vertexBufferDesc.BindFlags = BindFlags;
//	vertexBufferDesc.CPUAccessFlags = CPUAccessFlags;
//	vertexBufferDesc.MiscFlags = //0
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = _pvData;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &poVertexBuffer);
//	assert(SUCCEEDED(hr));
//
//	return SUCCEEDED(hr);
//
//	return hr;
//}
//
//
//
//
//SystemLib::return_value RenderLib::IDrawableNode::createIndexBuffer(ID3D11Device* _poDevice, const void* _pvData, UINT _data_size)
//{
//
//	// Set up the description of the static vertex buffer.
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	vertexBufferDesc.ByteWidth = _data_size;
//	vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = _pvData;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//	HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &poIndexBuffer);
//	assert(SUCCEEDED(hr));
//
//	return SUCCEEDED(hr);
//}