#pragma once

#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

#include "lib3d.h"
#include "..\QtRME_GUI\comment_defs.h"

using namespace DirectX;

#define NUMBER_OF_LIGHTS 1

struct LightSourceData
{
	SimpleMath::Vector3 lightDirection = { 0,0, -1 };
	float radiannce = 1.f;
	SimpleMath::Color lightColor;
};

struct PS_PerFrameLightBuffer
{
	LightSourceData LightData[NUMBER_OF_LIGHTS];
	SimpleMath::Color env_color = { 0.9f, 0.9f, 1.f, 1.0f };
	SimpleMath::Matrix mEnv = SimpleMath::Matrix::Identity;
};

struct VS_PerModel_ConstantBuffer {
	VS_PerModel_ConstantBuffer()
	{
		for (size_t i = 0; i < 256; i++)
		{
			DirectX::XMStoreFloat4x4(
				&inverse[i],
				DirectX::XMMatrixTranspose(XMMatrixIdentity())
			);

			DirectX::XMStoreFloat4x4(
				&tranforms[i],
				DirectX::XMMatrixTranspose(XMMatrixIdentity())
			);
		}
	}

	DirectX::SimpleMath::Matrix mWorld = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix mView = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mProjection = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mViewInverse = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mRotEnv = DirectX::SimpleMath::Matrix::Identity;;

	DirectX::SimpleMath::Matrix mWorldInvTransposeView = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mWorldViewProj = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mTexTransform = DirectX::SimpleMath::Matrix::Identity;;

	DirectX::XMFLOAT3 cameraPosition = { 0,0,0 };
	int is_weapon = 0;

	DirectX::XMFLOAT3 cameraLookAt = { 0,0,0 };;
	int static_model = 1;

	DirectX::XMFLOAT4 color = { 1,0,1, 1 };

	DirectX::SimpleMath::Matrix mRotation_X = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix mRotation_Y = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix mRotation_Z = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix mRotationWeapon = DirectX::SimpleMath::Matrix::Identity;

	DirectX::XMFLOAT4 rot = { 0,0,0,0 };

	int show_reflections = 0;
	uint32_t is_diffuse_gamma = 0;
	uint32_t is_specular_gamma = 0;
	int32_t has_alpha = 0;

	// for animation
	DirectX::SimpleMath::Matrix tranforms[256] = {  };
	DirectX::SimpleMath::Matrix inverse[256] = {  };
};

struct PS_PerFrame_ConstBuffer
{
	void reset()
	{
		ambientFactor = 1.0;
		specularFactor = 1.0;
		radiance = 1.0;
	}

	DirectX::SimpleMath::Matrix rotEnv = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Vector3 lightDirection;
	float radiance;

	DirectX::SimpleMath::Vector4 ambientLight;
	DirectX::SimpleMath::Vector4 diffuseColor;
	DirectX::SimpleMath::Vector4 specularColor;

	float ambientFactor;
	float diffuseFactor;
	float specularFactor;
	float roughnessFactor;
};

struct PS_PerMesh_ConstBuffer
{
	float exposure = 1.0;
	float reflectivity = 1.0;
	float ambientlight = 1.0;
	float reserved = 1.0;

	uint32_t alpha_used = 0;
	uint32_t render_fresnel = RENDER_PBR_FRESNEL_FULL;
	uint32_t reversed1;
	uint32_t reversed2;

	DirectX::SimpleMath::Color color_no_tex = { 155., 155., 155., 1. };

	DirectX::SimpleMath::Color color1 = { 1,1,1 };
	DirectX::SimpleMath::Color color2 = { 1,1,1 };
	DirectX::SimpleMath::Color color3 = { 1,1,1 };

	uint32_t PisTextureSRGB[64];
};

struct VS_PerFrame_ConstantBuffer {
	DirectX::SimpleMath::Matrix mView = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mProjection = DirectX::SimpleMath::Matrix::Identity;;
	DirectX::SimpleMath::Matrix mViewInverse = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Vector4 eyePosition;
};

//
//
//struct bone_tranforms
//{
//	DirectX::XMFLOAT4 tranforms[256];
//
//};
//
//struct _2dVertexInputType
//{
//	DirectX::XMFLOAT3 position;
//	DirectX::XMFLOAT2 tex;
//};

struct ModelVertexInputType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 tex;

	DirectX::XMFLOAT3 normal;

	DirectX::XMFLOAT3 tangent;

	DirectX::XMFLOAT3 binormal;

	// for rigging
	DirectX::XMUINT4 bone_ids;
	DirectX::XMFLOAT4 weights;
};

//PS_PerFrame_ConstantBuffer g_PS_PerFrame_ConstantBuffer;

//float g_turntable_rot = 0;
//bool g_bTurnTable = false;

//ID3D11Buffer* g_poConstantBuffer = nullptr, * g_poLightDataBuffer = nullptr;
