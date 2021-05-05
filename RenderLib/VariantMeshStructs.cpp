#include "VariantMeshStructs.h"

#define PUGIXML_WCHAR_MODE
#include "..\SystemLib\pugixml.hpp"
#include "..\ImporterLib\pack_file.h"

using namespace pugi;

bool RenderLib::VariantMeshInfo::parseWSModelInternal()
{
	if (strModelPath == "")
		return false;

	vecLodMaterials.clear();

	xml_document oXMLDocumentWSModel;

	pack_file_node* ppfn = pack_file::sm_poPathTree->getNodeFromInternalPath(strModelPath);

	if (ppfn == nullptr)
	{
		return false;
	}

	auto xmlResult = oXMLDocumentWSModel.load_buffer(ppfn->extractFileToMemory(), ppfn->file_size);

	if (xmlResult.status != pugi::status_ok)
		return false;

	xml_node xmlModel = oXMLDocumentWSModel.child(L"model");

	xml_node xmlGeometry = xmlModel.child(L"geometry");

	// get gemeotry path
	strModelPath = tools::wstring_to_string(xmlGeometry.text().as_string());

	xml_node materials = xmlModel.child(L"materials");

	xml_node material_child = materials.first_child();

	while (material_child)
	{
		size_t lod_index = material_child.attribute(L"lod_index").as_uint();
		size_t part_index = material_child.attribute(L"part_index").as_uint();

		MaterialInfo tempMaterial;
		tempMaterial.strPathToXmlMatrial = material_child.text().as_string();

		// resize if needed
		// resize LOD vector
		vecLodMaterials.resize(lod_index + 1 > vecLodMaterials.size() ? lod_index + 1 : vecLodMaterials.size());

		// resize PART vector
		vecLodMaterials[lod_index].resize(part_index + 1 > vecLodMaterials[lod_index].size() ? part_index + 1 : vecLodMaterials[lod_index].size());

		// insert material at proper part and lod index
		vecLodMaterials[lod_index].insert(vecLodMaterials[lod_index].begin() + part_index, tempMaterial);

		// parse the XML.material file
		vecLodMaterials[lod_index][part_index].parseXMLMaterialFile_Internal();

		// get the next sibling
		material_child = material_child.next_sibling();
	}

	if (strModelPath == "")
		return false;

	return true;
}

bool RenderLib::MaterialInfo::parseXMLMaterialFile_Internal()
{
	if (strPathToXmlMatrial == L"")
		return false;

	vecTextureItems.clear();

	xml_document oXMLDocumentWSModel;

	pack_file_node* ppfn = pack_file::sm_poPathTree->getNodeFromInternalPath(tools::wstring_to_string(strPathToXmlMatrial));

	if (ppfn == nullptr)
	{
		return false;
	}

	auto xmlResult = oXMLDocumentWSModel.load_buffer(ppfn->extractFileToMemory(), ppfn->file_size);

	if (xmlResult.status != pugi::status_ok)
		return false;

	xml_node material = oXMLDocumentWSModel.child(L"material");
	oShaderInfo.name = material.child(L"name").text().as_string();
	oShaderInfo.shader = material.child(L"shader").text().as_string();

	if (oShaderInfo.shader.find(L"alpha") != string::npos)
	{
		bAlpha = true;
	}

	xml_node textures = material.child(L"textures");

	xml_node texture_child = textures.first_child();

	while (texture_child)
	{
		ImporterLib::RigidModel::RigidModelTextureItem item;

		xml_node slot = texture_child.child(L"slot");

		wstring wstrSlot = slot.text().as_string();

		if (wstrSlot == L"s_mask")
		{
			item.Type = ETextureType::eMask;
			item.strPath = texture_child.child(L"source").text().as_string();
		}
		else
			if (wstrSlot == L"s_diffuse")
			{
				item.Type = ETextureType::eDiffuse;
				item.strPath = texture_child.child(L"source").text().as_string();
			}
			else
				if (wstrSlot == L"s_specular")
				{
					item.Type = ETextureType::eSpecular;
					item.strPath = texture_child.child(L"source").text().as_string();
				}
				else
					if (wstrSlot == L"s_normal")
					{
						item.Type = ETextureType::eNormal;
						item.strPath = texture_child.child(L"source").text().as_string();
					}
					else
						if (wstrSlot == L"s_gloss")
						{
							item.Type = ETextureType::eGlossMap;
							item.strPath = texture_child.child(L"source").text().as_string();
						}
		/////////////////////////////////////////////////////////////////////////////////
		// 3K Texture Types
		/////////////////////////////////////////////////////////////////////////////////
						else
							if (wstrSlot == L"s_xml_base_colour")
							{
								item.Type = ETextureType::eBaseColor;
								item.strPath = texture_child.child(L"source").text().as_string();
							}
							else
								if (wstrSlot == L"s_xml_normal")
								{
									item.Type = ETextureType::eNormal;
									item.strPath = texture_child.child(L"source").text().as_string();
								}
								else
									if (wstrSlot == L"s_xml_surface_map")
									{
										item.Type = ETextureType::eMaterialMap;
										item.strPath = texture_child.child(L"source").text().as_string();
									}
									else
										if (wstrSlot == L"s_xml_mask")
										{
											item.Type = ETextureType::eMask;
											item.strPath = texture_child.child(L"source").text().as_string();
										}
										else
										{
											// move to next <texture> tag
											texture_child = texture_child.next_sibling();
											continue; // no textures used for "normal" rendering found, ignore this item
										}

		vecTextureItems.push_back(item);
		// move to next <texture> tag
		texture_child = texture_child.next_sibling();
	}

	return true;
}