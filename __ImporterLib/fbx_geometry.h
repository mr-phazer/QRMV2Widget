#pragma once
#include <fbxsdk.h>
#include <vector>
#include <map>

using namespace std;

map<string, vector<fbxsdk::FbxVector2>> LoadUVInformation(fbxsdk::FbxMesh* pMesh);

FbxVector4 multT(FbxNode* node, FbxVector4 vector);
FbxVector4 multT_normal(FbxNode* node, FbxVector4 vector);


vector<FbxVector4> GetVectors(FbxNode* pNode, FbxLayerElementTemplate<FbxVector4>* lNormalElement);
vector<FbxVector4> GetNormals(FbxNode* pNode);
vector<FbxVector4> GetBinormals(FbxNode* pNode);
vector<FbxVector4> GetTangents(FbxNode* pNode);
