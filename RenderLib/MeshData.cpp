#include "MeshData.h"
#include "..\ImporterLib\pack_file.h"
//#include "mesh_structs.h"
#include "..\DirectXTex\DDSTextureLoader\DDSTextureLoader11.h"
#include "render_const_bufers.h"
#include "lib3d.h"
#include "..\QtRME_GUI\render_resources.h"
#include "Scene.h"

void RenderLib::SubMeshData::drawMesh(Scene* _poScene, Shader::Shader* _poShader)
{
	UINT stride = sizeof(ModelVertex);
	UINT offset = 0;

	auto _deviceContext = _poScene->getDeviceContext();

	_deviceContext->IASetVertexBuffers(0, 1,
		vertexBufferAddressReadOnly(),
		&stride, &offset);

	_deviceContext->IASetIndexBuffer(
		indexBuffer(),
		this->EIndexFormat,
		0
	);

	_poScene->m_PS_PerMesh_ConstantBuffer.color_no_tex = { 0.4, 0.3, 0.1,  0 };

	memcpy(_poScene->m_PS_PerMesh_ConstantBuffer.PisTextureSRGB, oMaterial.PIsGammaRGB, 64 * 4);

	_poScene->update_PS_PerMeshConstBuffer(oMaterial);

	if (_poShader) //  use shader in mesh object?
	{
		/*
	}_deviceContext->IASetInputLayout(_poShader->getInputLayout());
		_deviceContext->VSSetShader(_poShader->getVertexShader(), nullptr, 0);
		_deviceContext->PSSetShader(_poShader->getPixelShader(), nullptr, 0);*/
	}
	else
	{
		_deviceContext->IASetInputLayout(m_poInputLayout);
		_deviceContext->VSSetShader(m_poVertexShader, nullptr, 0);

		/*if (_poScene->getDefaultPixelShader())
		{
			_deviceContext->PSSetShader(_poScene->getDefaultPixelShader(), nullptr, 0);
		}
		else*/
		{
			_deviceContext->PSSetShader(m_poPixelShader, nullptr, 0);
		}
	}

	//deviceContext->RSSetViewports(1, poD3DWidget->viewPort());
	//_deviceContext->RSSetState(m_poRasterizerStateWireFrame);
	//_deviceContext->RSSetState(NULL);

	if (oMaterial.vecD3D_RawshaderVievResources.size())
	{
		_deviceContext->PSSetShaderResources(
			3,
			oMaterial.vecD3D_RawshaderVievResources.size(),
			&oMaterial.vecD3D_RawshaderVievResources[0]
		);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// draw triangles
	///////////////////////////////////////////////////////////////////////////////////////////
	_deviceContext->IASetPrimitiveTopology(this->ETopology);
	_deviceContext->RSSetState(m_poRasterizerState);

	_deviceContext->DrawIndexed(m_index_count, 0, 0);
	///////////////////////////////////////////////////////////////////////////////////////////
}

const RenderLib::SubMeshes& RenderLib::MeshLodData::operator[](size_t i) const
{
	try
	{
		return vecLODs[i];
	}
	catch (const std::out_of_range& oor)
	{
		int debug_1 = 1;
	}
}

RenderLib::SubMeshes& RenderLib::MeshLodData::operator[](size_t i)
{
	try
	{
		return vecLODs[i];
	}
	catch (const std::out_of_range& oor)
	{
		int debug_1 = 1;
	}
}

void RenderLib::MeshLodData::drawLOD(Scene* _poScene, Shader::Shader* _poShader)
{
	// TODO: change this to take a "Scene*"
	// TODO: make is auto pick the lod level using the camera, for example, the number of bound box sizes from the camera
	if (vecLODs.size())
	{
		for (auto& it : vecLODs[0].vecspoMeshData)
			it.drawMesh(_poScene, _poShader);
	}
}

bool RenderLib::MeshLodData::loadFromRigidModel(
	Scene* _poScene,
	ImporterLib::RigidModel::RigidModelFileCommon* _poRMV2File,
	const VariantMeshInfo* _poMeshMaterialInfo,
	int32_t _bone_index
)
{
	size_t lod_count = _poRMV2File->spovecLODHeaders.size();

	vecLODs.clear();
	vecLODs.resize(lod_count);

	size_t lod = 0;
	//for (lod < lod_count; lod++)
	{
		vecLODs[lod].vecspoMeshData.resize(_poRMV2File->spovecLODHeaders[lod]->dwGroupCount);

		for (size_t group = 0; group < _poRMV2File->spovecLODHeaders[lod]->dwGroupCount; group++)
		{
			//vecLODs[lod].vecspoMeshData.push_back(shared_ptr<SubMeshData>(new SubMeshData)); // TODO: REMOVE maybe

			auto poSubMeshBlock = _poRMV2File->getSubMeshBlock(lod, group);

			size_t vertex_size = poSubMeshBlock->oMesh.vertex_size;

			uint8_t* pV = nullptr;
			switch (poSubMeshBlock->oGroupHeader.VertexFormatId)
			{
				// Convert raw bytes to raw vertex buffer
			case VertexFormat::EVertexType::eCinematicFormat:
			{
				pV = makeVertexBuffer_CINEMATIC(
					&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
					vertex_size,
					poSubMeshBlock->oGroupHeader.uiVertextCount
				);
			}
			break;

			case VertexFormat::EVertexType::eWeightedFormat:
			{
				pV = makeVertexBuffer_WEIGHTED(
					&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
					vertex_size,
					poSubMeshBlock->oGroupHeader.uiVertextCount
				);
			}
			break;

			case VertexFormat::EVertexType::eDefaultFormat:
			{
				pV = makeVertexBuffer_DEFAULT(
					&poSubMeshBlock->oGroupHeader,
					&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
					vertex_size,
					poSubMeshBlock->oGroupHeader.uiVertextCount,
					_bone_index
				);
			}
			break;
			}

			// set bounding box values for submeshes
			vecLODs[lod].vecspoMeshData[group].vBBMin.x = poSubMeshBlock->oGroupHeader.fGroupMinimumX;
			vecLODs[lod].vecspoMeshData[group].vBBMin.y = poSubMeshBlock->oGroupHeader.fGroupMinimumY;
			vecLODs[lod].vecspoMeshData[group].vBBMin.z = poSubMeshBlock->oGroupHeader.fGroupMinimumZ;

			vecLODs[lod].vecspoMeshData[group].vBBMax.x = poSubMeshBlock->oGroupHeader.fGroupMaximumX;
			vecLODs[lod].vecspoMeshData[group].vBBMax.y = poSubMeshBlock->oGroupHeader.fGroupMaximumY;
			vecLODs[lod].vecspoMeshData[group].vBBMax.z = poSubMeshBlock->oGroupHeader.fGroupMaximumZ;

			// update bound box the encloses all submeshes in LOD, using min/max
			vBBMax.x = max(vecLODs[lod].vecspoMeshData[group].vBBMax.x, vBBMax.x);
			vBBMax.y = max(vecLODs[lod].vecspoMeshData[group].vBBMax.y, vBBMax.y);
			vBBMax.z = max(vecLODs[lod].vecspoMeshData[group].vBBMax.z, vBBMax.z);

			vBBMin.x = min(vecLODs[lod].vecspoMeshData[group].vBBMin.x, vBBMin.x);
			vBBMin.y = min(vecLODs[lod].vecspoMeshData[group].vBBMin.y, vBBMin.y);
			vBBMin.z = min(vecLODs[lod].vecspoMeshData[group].vBBMin.z, vBBMin.z);

			vecLODs[lod].vecspoMeshData[group].EIndexFormat = DXGI_FORMAT_R16_UINT;

			vecLODs[lod].vecspoMeshData[group].createVertexBuffer(
				_poScene->getDevice(),
				(size_t)poSubMeshBlock->oGroupHeader.uiVertextCount * sizeof(ModelVertex),
				pV
			);
			delete[] pV;

			uint16_t* pI = new uint16_t[poSubMeshBlock->oGroupHeader.uiIndexCount];
			for (size_t index = 0; index < poSubMeshBlock->oGroupHeader.uiIndexCount / 3; index++)
			{
				pI[index * 3] = poSubMeshBlock->oMesh.vecIndexData16[index * 3];
				pI[index * 3 + 1] = poSubMeshBlock->oMesh.vecIndexData16[index * 3 + 1];
				pI[index * 3 + 2] = poSubMeshBlock->oMesh.vecIndexData16[index * 3 + 2];
			}

			vecLODs[lod].vecspoMeshData[group].createIndexBuffer(
				_poScene->getDevice(),
				(size_t)poSubMeshBlock->oGroupHeader.uiIndexCount * 2,
				pI
			);
			delete[] pI;

			vecLODs[lod].vecspoMeshData[group].m_index_count = (size_t)poSubMeshBlock->oGroupHeader.uiIndexCount;

			vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures.resize(poSubMeshBlock->oGroupHeader.dwTextureCount);
			//vecLODs[lod].vecMeshData[group].oMaterial.vecD3D_RawshaderVieResource.resize(poSubMeshBlock->oHeader.dwTextureCount);

			vecLODs[lod].vecspoMeshData[group].m_poPixelShader =
				_poScene->getResources()->getPixelShader(Resources::EPixelShaders::ePBRSpecGloss);

			vecLODs[lod].vecspoMeshData[group].m_poVertexShader =
				_poScene->getResources()->getVertexShader(Resources::EVertexShader::eWeighted4);

			vecLODs[lod].vecspoMeshData[group].m_poInputLayout =
				_poScene->getResources()->getInputLayout(Resources::EVertexShader::eWeighted4);

			/////////////////////////////////////////
			// read materials (textures)
			/////////////////////////////////////////
			bool bLoadedXMLMaterials = false;

			if (_poMeshMaterialInfo)
			{
				if (_poMeshMaterialInfo->vecLodMaterials.size() > lod)
					if (_poMeshMaterialInfo->vecLodMaterials[lod].size() > group)
						for (size_t t = 0; t < _poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems.size(); t++)
						{
							// set alpha
							vecLODs[lod].vecspoMeshData[group].oMaterial.alpha = (_poMeshMaterialInfo->vecLodMaterials[lod][group].bAlpha) ? 1 : 0;

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType =
								_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].Type;

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath =
								tools::wstring_to_string(_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].strPath);

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].wstrInternalPathh =
								_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].strPath;

							auto poPackFileNode = pack_file::sm_poPathTree->getNodeFromInternalPath
							(
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath
							);

							if (poPackFileNode)
							{
								auto pBuffer = poPackFileNode->extractFileToMemory();

								HRESULT hr = DirectX::CreateDDSTextureFromMemory(
									_poScene->getDevice(),
									pBuffer,
									poPackFileNode->file_size,
									&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_Texture,
									&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource
								);

								// resize to fit textures ID (IDs converted to indexes)
								/*size_t texture_index = vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType;
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(
									texture_index >=
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size() ?
									texture_index + 1 :
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size()
								);*/

								// ---------------------------------------------------------------------------------
								// insert textures at texture type ID index, so the enum value becomes and index
								// in the HLSL shader texture array, for simplicity
								// ---------------------------------------------------------------------------------
								size_t texture_type_index = (size_t)
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType;

								if (texture_type_index + 1 > vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size())
								{
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(texture_type_index + 1);
								}

								// store Shader View Resource
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[texture_type_index] =
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource.Get();

								poPackFileNode->destroy();

								bool bLoadedXMLMaterials = true;
							}
						}
			}
			//else

			if (!bLoadedXMLMaterials)
			{
				vecLODs[lod].vecspoMeshData[group].oMaterial.alpha = _poRMV2File->getSubMeshBlock(lod, group)->oMaterial.alpha;

				for (size_t t = 0; t < poSubMeshBlock->oGroupHeader.dwTextureCount; t++)
				{
					vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType =
						_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].Type;

					vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath =
						_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].szTextureDirAndFileName;

					vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].wstrInternalPathh =
						_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].szwTextureDirAndFileName;

					auto poPackFileNode = pack_file::sm_poPathTree->getNodeFromInternalPath
					(
						_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].szTextureDirAndFileName
					);

					if (poPackFileNode)
					{
						auto pBuffer = poPackFileNode->extractFileToMemory();

						HRESULT hr = DirectX::CreateDDSTextureFromMemory(
							_poScene->getDevice(),
							pBuffer,
							poPackFileNode->file_size,
							&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_Texture,
							&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource
						);

						// resize to fit textures ID (IDs converted to indexes)
						/*vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(
							(size_t)_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].Type >=
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size() ?
							(size_t)_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].Type+1 :
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size()
						);*/

						// ---------------------------------------------------------------------------------
						// insert textures at texture type ID index, so the enum value becomes and index
						// in the HLSL shader texture array, for simplicity
						// ---------------------------------------------------------------------------------
						size_t texture_type_index =
							(size_t)_poRMV2File->getSubMeshBlock(lod, group)->oMaterial.vecTextures[t].Type;

						if (texture_type_index + 1 > vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size())
						{
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(texture_type_index + 1);
						}

						// store Shader View Resource
						vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[texture_type_index] =
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource.Get();

						poPackFileNode->destroy();
					}
				}
			}
		}
	}

	return true;
}

bool RenderLib::MeshLodData::loadFromRigidModel_new(Scene* _poScene, RigidModelV2::Common::CommonFile* _poRMV2File, const VariantMeshInfo* _poMeshMaterialInfo, int32_t _bone_index)

{
	size_t lod_count = _poRMV2File->oFileHeader.dwLodCount;

	vecLODs.clear();
	vecLODs.resize(lod_count);

	size_t lod = 0;
	//for (lod < lod_count; lod++)
	{
		vecLODs[lod].vecspoMeshData.resize(_poRMV2File->oLodHeaderBlock.vecElements[lod].dwGroupCount);

		for (size_t group = 0; group < _poRMV2File->oLodHeaderBlock.vecElements[lod].dwGroupCount; group++)
		{
			//vecLODs[lod].vecspoMeshData.push_back(shared_ptr<SubMeshData>(new SubMeshData)); // TODO: REMOVE maybe

			auto poSubMeshBlock = _poRMV2File->getGroup(lod, group);

			size_t vertex_size = poSubMeshBlock->getVertexSize();

			// attach weapon to bone by assign incluence {bone_index, 1.0} for all vertices
			if (poSubMeshBlock->oSubMeshHeader.VertexFormatId == EVertexFormat::eDefaultFormat)
			{
				poSubMeshBlock->oMeshBlock.unpackAs_Default(32,
					poSubMeshBlock->oSubMeshHeader.vPivot,
					_bone_index
				);
			}

			uint8_t* pV = reinterpret_cast<uint8_t*>(&poSubMeshBlock->oMeshBlock.vecVertices[0]);

			//switch (poSubMeshBlock->oGroupHeader.VertexFormatId)
			//{
			//	// Convert raw bytes to raw vertex buffer
			//case VertexFormat::EVertexType::eCinematicFormat:
			//{
			//	pV = makeVertexBuffer_CINEMATIC(
			//		&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
			//		vertex_size,
			//		poSubMeshBlock->oGroupHeader.uiVertextCount
			//	);
			//}
			//break;

			//case VertexFormat::EVertexType::eWeightedFormat:
			//{
			//	pV = makeVertexBuffer_WEIGHTED(
			//		&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
			//		vertex_size,
			//		poSubMeshBlock->oGroupHeader.uiVertextCount
			//	);
			//}
			//break;

			//case VertexFormat::EVertexType::eDefaultFormat:
			//{
			//	pV = makeVertexBuffer_DEFAULT(
			//		&poSubMeshBlock->oGroupHeader,
			//		&poSubMeshBlock->oMesh.vecVertexDataRAW[0],
			//		vertex_size,
			//		poSubMeshBlock->oGroupHeader.uiVertextCount,
			//		_bone_index
			//	);
			//}
			//break;
			//}

			// set bounding box values for submeshes
			vecLODs[lod].vecspoMeshData[group].vBBMin = poSubMeshBlock->oPreHeader.vMinBB;
			vecLODs[lod].vecspoMeshData[group].vBBMax = poSubMeshBlock->oPreHeader.vMaxBB;

			/*vecLODs[lod].vecspoMeshData[group].vBBMin.x =
				vecLODs[lod].vecspoMeshData[group].vBBMin.y = poSubMeshBlock->oGroupHeader.fGroupMinimumY;
			vecLODs[lod].vecspoMeshData[group].vBBMin.z = poSubMeshBlock->oGroupHeader.fGroupMinimumZ;

			vecLODs[lod].vecspoMeshData[group].vBBMax.x = poSubMeshBlock->oGroupHeader.fGroupMaximumX;
			vecLODs[lod].vecspoMeshData[group].vBBMax.y = poSubMeshBlock->oGroupHeader.fGroupMaximumY;
			vecLODs[lod].vecspoMeshData[group].vBBMax.z = poSubMeshBlock->oGroupHeader.fGroupMaximumZ;*/

			// update bound box the encloses all submeshes in LOD, using min/max
			vBBMax.x = max(vecLODs[lod].vecspoMeshData[group].vBBMax.x, vBBMax.x);
			vBBMax.y = max(vecLODs[lod].vecspoMeshData[group].vBBMax.y, vBBMax.y);
			vBBMax.z = max(vecLODs[lod].vecspoMeshData[group].vBBMax.z, vBBMax.z);

			vBBMin.x = min(vecLODs[lod].vecspoMeshData[group].vBBMin.x, vBBMin.x);
			vBBMin.y = min(vecLODs[lod].vecspoMeshData[group].vBBMin.y, vBBMin.y);
			vBBMin.z = min(vecLODs[lod].vecspoMeshData[group].vBBMin.z, vBBMin.z);

			vecLODs[lod].vecspoMeshData[group].EIndexFormat = DXGI_FORMAT_R16_UINT;

			vecLODs[lod].vecspoMeshData[group].createVertexBuffer(
				_poScene->getDevice(),
				(size_t)poSubMeshBlock->oPreHeader.dwVertexCount * sizeof(ModelVertex),
				pV
			);
			//delete[] pV;

			/*uint16_t* pI = new uint16_t[poSubMeshBlock->oGroupHeader.uiIndexCount];
			for (size_t index = 0; index < poSubMeshBlock->oGroupHeader.uiIndexCount / 3; index++)
			{
				pI[index * 3] = poSubMeshBlock->oMesh.vecIndexData16[index * 3];
				pI[index * 3 + 1] = poSubMeshBlock->oMesh.vecIndexData16[index * 3 + 1];
				pI[index * 3 + 2] = poSubMeshBlock->oMesh.vecIndexData16[index * 3 + 2];
			}*/

			vecLODs[lod].vecspoMeshData[group].createIndexBuffer(
				_poScene->getDevice(),
				(size_t)poSubMeshBlock->oPreHeader.dwIndexCount * 2,
				&poSubMeshBlock->oMeshBlock.vecIndices[0]
			);
			//delete[] pI;

			vecLODs[lod].vecspoMeshData[group].m_index_count = (size_t)poSubMeshBlock->oPreHeader.dwIndexCount;

			vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures.resize(poSubMeshBlock->oSubMeshHeader.dwTextureCount);
			//vecLODs[lod].vecMeshData[group].oMaterialBlock.vecD3D_RawshaderVieResource.resize(poSubMeshBlock->oHeader.dwTextureCount);

			vecLODs[lod].vecspoMeshData[group].m_poPixelShader =
				_poScene->getResources()->getPixelShader(Resources::EPixelShaders::ePBRSpecGloss);

			vecLODs[lod].vecspoMeshData[group].m_poVertexShader =
				_poScene->getResources()->getVertexShader(Resources::EVertexShader::eWeighted4);

			vecLODs[lod].vecspoMeshData[group].m_poInputLayout =
				_poScene->getResources()->getInputLayout(Resources::EVertexShader::eWeighted4);

			/////////////////////////////////////////
			// read materials (textures)
			/////////////////////////////////////////
			bool bLoadedXMLMaterials = false;

			if (_poMeshMaterialInfo)
			{
				if (_poMeshMaterialInfo->vecLodMaterials.size() > lod)
					if (_poMeshMaterialInfo->vecLodMaterials[lod].size() > group)
						for (size_t t = 0; t < _poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems.size(); t++)
						{
							// set alpha
							vecLODs[lod].vecspoMeshData[group].oMaterial.alpha = (_poMeshMaterialInfo->vecLodMaterials[lod][group].bAlpha) ? 1 : 0;

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType =
								_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].Type;

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath =
								tools::wstring_to_string(_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].strPath);

							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].wstrInternalPathh =
								_poMeshMaterialInfo->vecLodMaterials[lod][group].vecTextureItems[t].strPath;

							auto poPackFileNode = pack_file::sm_poPathTree->getNodeFromInternalPath
							(
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath
							);

							if (poPackFileNode)
							{
								auto pBuffer = poPackFileNode->extractFileToMemory();

								HRESULT hr = DirectX::CreateDDSTextureFromMemory(
									_poScene->getDevice(),
									pBuffer,
									poPackFileNode->file_size,
									&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_Texture,
									&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource
								);

								// resize to fit textures ID (IDs converted to indexes)
								/*size_t texture_index = vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType;
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(
									texture_index >=
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size() ?
									texture_index + 1 :
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size()
								);*/

								// ---------------------------------------------------------------------------------
								// insert textures at texture type ID index, so the enum value becomes and index
								// in the HLSL shader texture array, for simplicity
								// ---------------------------------------------------------------------------------

								D3D11_SHADER_RESOURCE_VIEW_DESC desc;
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource->GetDesc(&desc);;
								bool bIsTextureSRGB = lib3d::isSRGB(desc.Format);

								size_t texture_type_index = (size_t)
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType;

								if (texture_type_index + 1 > vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size())
								{
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(texture_type_index + 1);
									//vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextureIsGamma.resize(texture_type_index + 1);
								}

								// store Shader View Resource
								vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[texture_type_index] =
									vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource.Get();

								vecLODs[lod].vecspoMeshData[group].oMaterial.PIsGammaRGB[texture_type_index] = !bIsTextureSRGB;

								poPackFileNode->destroy();

								bool bLoadedXMLMaterials = true;
							}
						}
			}
			//else

			if (!bLoadedXMLMaterials)
			{
				vecLODs[lod].vecspoMeshData[group].oMaterial.alpha = _poRMV2File->getGroup(lod, group)->oMaterialBlock.dwAlphaFlags;

				for (size_t t = 0; t < poSubMeshBlock->oSubMeshHeader.dwTextureCount; t++)
				{
					vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].EType =
						_poRMV2File->getGroup(lod, group)->oMaterialBlock.vecTextures[t].Type;

					vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].strInternalPath =
						_poRMV2File->getGroup(lod, group)->oMaterialBlock.vecTextures[t].szTextureDirAndFileName;

					/*vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].wstrInternalPathh =
						_poRMV2File->getGroup(lod, group)->oMaterialBlock.vecTextures[t].szwTextureDirAndFileName;*/

					auto poPackFileNode = pack_file::sm_poPathTree->getNodeFromInternalPath
					(
						_poRMV2File->getGroup(lod, group)->oMaterialBlock.vecTextures[t].szTextureDirAndFileName
					);

					if (poPackFileNode)
					{
						auto pBuffer = poPackFileNode->extractFileToMemory();

						HRESULT hr = DirectX::CreateDDSTextureFromMemory(
							_poScene->getDevice(),
							pBuffer,
							poPackFileNode->file_size,
							&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_Texture,
							&vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource
						);

						D3D11_SHADER_RESOURCE_VIEW_DESC desc;
						vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource->GetDesc(&desc);;
						bool bIsTextureSRGB = lib3d::isSRGB(desc.Format);

						// ---------------------------------------------------------------------------------
						// insert textures at texture type ID index, so the enum value becomes and index
						// in the HLSL shader texture array, for simplicity
						// ---------------------------------------------------------------------------------
						size_t texture_type_index =
							(size_t)_poRMV2File->getGroup(lod, group)->oMaterialBlock.vecTextures[t].Type;

						if (texture_type_index + 1 > vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.size())
						{
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(texture_type_index + 1);
						}

						// store Shader View Resource
						vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[texture_type_index] =
							vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures[t].poD3D_shaderViewResource.Get();

						vecLODs[lod].vecspoMeshData[group].oMaterial.PIsGammaRGB[texture_type_index] = !bIsTextureSRGB;

						poPackFileNode->destroy();
					}
				}
			}
		}
	}

	return true;
}
bool RenderLib::MeshLodData::loadFromFbxMeshes(Scene* _poScene, std::vector<FBX_Mesh>& _vecMeshes)
{
	size_t lod_count = 1;

	vecLODs.clear();
	vecLODs.resize(lod_count);

	size_t lod = 0;
	//for (lod < lod_count; lod++)
	{
		vecLODs[lod].vecspoMeshData.resize(_vecMeshes.size());

		for (size_t group = 0; group < _vecMeshes.size(); group++)
		{
			_vecMeshes[group].doCleanUp_Float();
			//vecLODs[lod].vecspoMeshData.push_back(shared_ptr<SubMeshData>(new SubMeshData));

			vecLODs[lod].vecspoMeshData[group].vBBMin = _vecMeshes[group].vMin;
			vecLODs[lod].vecspoMeshData[group].vBBMax = _vecMeshes[group].vMax;

			vector<ModelVertex> vecDestVertices(_vecMeshes[group].vecPolygonVertices.size());

			for (size_t v = 0; v < _vecMeshes[group].vecPolygonVertices.size(); v++)
			{
				auto& vertex = _vecMeshes[group].vecPolygonVertices[v];
				vecDestVertices[v].position.x = vertex.position.x;
				vecDestVertices[v].position.y = vertex.position.y;
				vecDestVertices[v].position.z = vertex.position.z;
				//vecDestVertices[v].position.w = 1;

				vecDestVertices[v].uv = vertex.textcords;

				vecDestVertices[v].normal = vertex.normal;
				vecDestVertices[v].bitangent = vertex.bitangent;
				vecDestVertices[v].tangent = vertex.tangent;

				vecDestVertices[v].bone_indices = { 0, 0, 0, 0 };
				vecDestVertices[v].bone_weights = { 1.0f, 0.f, 0.f, 0.f };

				vecDestVertices[v].color = { 0.9, 0.8, 0.8, 1 };
			}

			vecLODs[lod].vecspoMeshData[group].EIndexFormat = DXGI_FORMAT_R32_UINT;
			vecLODs[lod].vecspoMeshData[group].ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

			vecLODs[lod].vecspoMeshData[group].createVertexBuffer(
				_poScene->getDevice(),
				vecDestVertices.size() * sizeof(ModelVertex),
				&vecDestVertices[0]
			);

			vector<uint32_t> pI(_vecMeshes[group].vecIndices32.size());
			for (size_t index = 0; index < _vecMeshes[group].vecIndices32.size() / 3; index++)
			{
				pI[index * 3] = _vecMeshes[group].vecIndices32[index * 3];
				pI[index * 3 + 1] = _vecMeshes[group].vecIndices32[index * 3 + 1];
				pI[index * 3 + 2] = _vecMeshes[group].vecIndices32[index * 3 + 2];
			}

			vecLODs[lod].vecspoMeshData[group].createIndexBuffer(
				_poScene->getDevice(),
				(size_t)_vecMeshes[group].vecIndices32.size() * 4,
				&pI[0]
			);

			vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eDiffuse, _poScene->getResources()->getTexture(Resources::ETextures::eDefaultDiffuse));
			vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eSpecular, _poScene->getResources()->getTexture(Resources::ETextures::eDefaultSpecular));
			vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eNormal, _poScene->getResources()->getTexture(Resources::ETextures::eDefaultNormal));
			vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eGlossMap, _poScene->getResources()->getTexture(Resources::ETextures::eDeFaultGlossMap));
			/*vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eSpecular, _poScene->getResources()->getTexture(Resources::ETextures::eDefaultSpecular));

			vecLODs[lod].vecspoMeshData[group].oMaterial.setTexture(ETextureType::eGlossMap, _poScene->getResources()->getTexture(Resources::ETextures::eDeFaultGlossMap));*/

			vecLODs[lod].vecspoMeshData[group].m_index_count = (size_t)_vecMeshes[group].vecIndices32.size();

			//vecLODs[lod].vecspoMeshData[group].oMaterial.vecTextures.resize(poSubMeshBlock->oHeader.dwTextureCount);
			vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources.resize(30);

			vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[ETextureType::eDiffuse] = _poScene->getResources()->getTexture(Resources::ETextures::eDefaultDiffuse);
			vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[ETextureType::eSpecular] = _poScene->getResources()->getTexture(Resources::ETextures::eDefaultSpecular);
			vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[ETextureType::eNormal] = _poScene->getResources()->getTexture(Resources::ETextures::eDefaultNormal);
			vecLODs[lod].vecspoMeshData[group].oMaterial.vecD3D_RawshaderVievResources[ETextureType::eGlossMap] = _poScene->getResources()->getTexture(Resources::ETextures::eDeFaultGlossMap);

			vecLODs[lod].vecspoMeshData[group].m_poPixelShader =
				_poScene->getResources()->getPixelShader(Resources::EPixelShaders::ePBRSpecGloss);

			vecLODs[lod].vecspoMeshData[group].m_poVertexShader =
				_poScene->getResources()->getVertexShader(Resources::EVertexShader::eWeighted4);

			vecLODs[lod].vecspoMeshData[group].m_poInputLayout =
				_poScene->getResources()->getInputLayout(Resources::EVertexShader::eWeighted4);
		}
	}

	return true;
}

uint8_t* RenderLib::makeVertexBuffer_CINEMATIC(uint8_t* pInput, size_t vertex_size, size_t vertex_count)
{
	//read mesh
	ModelVertex* pV = new ModelVertex[vertex_count];

	for (size_t vertex = 0; vertex < vertex_count; vertex++)
	{
		RMV2_Vertex_Raw_Cinematic* pO =
			reinterpret_cast<RMV2_Vertex_Raw_Cinematic*>(&pInput[vertex * vertex_size]);

		pV[vertex].position.x = pO->pos_x;
		pV[vertex].position.y = pO->pos_y;
		pV[vertex].position.z = pO->pos_z;
		//pV[vertex].position.w = pO->pos_w;

		float u = pO->u;
		float v = pO->v;

		pV[vertex].uv.x = u;
		pV[vertex].uv.y = v;

		pV[vertex].normal.x = lib3d::unorm8_to_float(pO->norm_x);
		pV[vertex].normal.y = lib3d::unorm8_to_float(pO->norm_y);
		pV[vertex].normal.z = lib3d::unorm8_to_float(pO->norm_z);

		pV[vertex].tangent.x = lib3d::unorm8_to_float(pO->tangent_x);
		pV[vertex].tangent.y = lib3d::unorm8_to_float(pO->tangent_y);
		pV[vertex].tangent.z = lib3d::unorm8_to_float(pO->tangent_z);

		pV[vertex].bitangent.x = lib3d::unorm8_to_float(pO->bitangent_x);
		pV[vertex].bitangent.y = lib3d::unorm8_to_float(pO->bitangent_y);
		pV[vertex].bitangent.z = lib3d::unorm8_to_float(pO->bitangent_z);

		//SimpleMath::Vector3 normal = pV[vertex].normal;
		//SimpleMath::Vector3 tangent = pV[vertex].tangent;
		//SimpleMath::Vector3 bitangent = pV[vertex].bitangent;

		//
		//XMVECTOR xmScalar = DirectX::XMVector3Dot(DirectX::XMVector3Cross(normal, tangent), bitangent);

		//float scalar1;
		//XMStoreFloat(&scalar1, xmScalar);
		//float scalar = (normal.Cross(tangent)).Dot(bitangent);
		//if (scalar < 0.0f)
		//{
		//	tangent *= -1;
		//	scalar = (normal.Cross(tangent)).Dot(bitangent);
		//	pV[vertex].tangent = tangent;
		//	int debug_1 = 1;
		//}

		pV[vertex].bone_indices.x = pO->joint_id1;
		pV[vertex].bone_indices.y = pO->joint_id2;
		pV[vertex].bone_indices.z = pO->joint_id3;
		pV[vertex].bone_indices.w = pO->joint_id4;

		pV[vertex].bone_weights.x = ((float)pO->weight1) / 255.0f;
		pV[vertex].bone_weights.y = ((float)pO->weight2) / 255.0f;
		pV[vertex].bone_weights.z = ((float)pO->weight3) / 255.0f;
		pV[vertex].bone_weights.w = ((float)pO->weight4) / 255.0f;
	}

	return reinterpret_cast<uint8_t*>(pV);
}

uint8_t* RenderLib::makeVertexBuffer_WEIGHTED(uint8_t* pInput, size_t vertex_size, size_t vertex_count)
{
	//read mesh
	ModelVertex* pV = new ModelVertex[vertex_count];

	for (size_t vertex = 0; vertex < vertex_count; vertex++)
	{
		RMV2_Vertex_Raw_Weighted* pO =
			reinterpret_cast<RMV2_Vertex_Raw_Weighted*>(&pInput[vertex * vertex_size]);

		pV[vertex].position.x = pO->pos_x;
		pV[vertex].position.y = pO->pos_y;
		pV[vertex].position.z = pO->pos_z;
		//pV[vertex].position.w = pO->pos_w;

		float u = pO->u;
		float v = pO->v;

		pV[vertex].uv.x = u;
		pV[vertex].uv.y = v;

		pV[vertex].normal.x = lib3d::unorm8_to_float(pO->normal_x);
		pV[vertex].normal.y = lib3d::unorm8_to_float(pO->normal_y);
		pV[vertex].normal.z = lib3d::unorm8_to_float(pO->normal_z);

		pV[vertex].tangent.x = lib3d::unorm8_to_float(pO->tangent_x);
		pV[vertex].tangent.y = lib3d::unorm8_to_float(pO->tangent_y);
		pV[vertex].tangent.z = lib3d::unorm8_to_float(pO->tangent_z);

		pV[vertex].bitangent.x = lib3d::unorm8_to_float(pO->bitangent_x);
		pV[vertex].bitangent.y = lib3d::unorm8_to_float(pO->bitangent_y);
		pV[vertex].bitangent.z = lib3d::unorm8_to_float(pO->bitangent_z);

		pV[vertex].bone_indices.x = pO->joint_id1;
		pV[vertex].bone_indices.y = pO->joint_id2;
		pV[vertex].bone_indices.z = 0;
		pV[vertex].bone_indices.w = 0;

		pV[vertex].bone_weights.x = ((float)pO->weight1) / 255.0f;
		pV[vertex].bone_weights.y = 1.0 - pV[vertex].bone_weights.x;
		pV[vertex].bone_weights.z = 0.0;
		pV[vertex].bone_weights.w = 0.0;
	}

	return reinterpret_cast<uint8_t*>(pV);
}

uint8_t* RenderLib::makeVertexBuffer_DEFAULT(ImporterLib::RigidModel::SubMeshHeader* _poSubmeshHeader, uint8_t* pInput, size_t vertex_size, size_t vertex_count, int32_t bone_index)
{
	//read mesh
	ModelVertex* pV = new ModelVertex[vertex_count];

	for (size_t vertex = 0; vertex < vertex_count; vertex++)
	{
		RMV2_Vertex_Raw_Default* pO =
			reinterpret_cast<RMV2_Vertex_Raw_Default*>(&pInput[vertex * vertex_size]);

		pV[vertex].position.x = (pO->pos_x + _poSubmeshHeader->pivot.x);
		pV[vertex].position.y = pO->pos_y + _poSubmeshHeader->pivot.y;
		pV[vertex].position.z = (pO->pos_z + _poSubmeshHeader->pivot.z);

		/*
		pV[vertex].position.x = (pO->pos_x + _poSubmeshHeader->pivot.x);
		pV[vertex].position.y = pO->pos_y + _poSubmeshHeader->pivot.y;
		pV[vertex].position.z = (pO->pos_z + _poSubmeshHeader->pivot.z);*/

		//DirectX::SimpleMath::Matrix m = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(0*XM_PI, XM_PI, XM_PI/2);

		//pV[vertex].position = DirectX::SimpleMath::Vector3::Transform(pV[vertex].position, m);

		float u = pO->u;
		float v = pO->v;

		pV[vertex].uv.x = u;
		pV[vertex].uv.y = v;

		pV[vertex].normal.x = -lib3d::unorm8_to_float(pO->normal_x);
		pV[vertex].normal.y = lib3d::unorm8_to_float(pO->normal_y);
		pV[vertex].normal.z = lib3d::unorm8_to_float(pO->normal_z);

		pV[vertex].tangent.x = -lib3d::unorm8_to_float(pO->tangent_x);
		pV[vertex].tangent.y = lib3d::unorm8_to_float(pO->tangent_y);
		pV[vertex].tangent.z = lib3d::unorm8_to_float(pO->tangent_z);

		pV[vertex].bitangent.x = -lib3d::unorm8_to_float(pO->bitangent_x);
		pV[vertex].bitangent.y = lib3d::unorm8_to_float(pO->bitangent_y);
		pV[vertex].bitangent.z = lib3d::unorm8_to_float(pO->bitangent_z);

		pV[vertex].bone_indices.x = 0;
		pV[vertex].bone_indices.y = 0;
		pV[vertex].bone_indices.y = 0;
		pV[vertex].bone_indices.w = 0;

		pV[vertex].bone_weights.x = 0;
		pV[vertex].bone_weights.y = 0;
		pV[vertex].bone_weights.z = 0;
		pV[vertex].bone_weights.w = 0;

		if (bone_index > -1)
		{
			// TODO: remove debugging code
			//pV[vertex].bone_indices.w = 0;  // bone_index;
			pV[vertex].bone_indices.w = bone_index;
			pV[vertex].bone_weights.w = 1.0f;
		}
	}

	return reinterpret_cast<uint8_t*>(pV);
}