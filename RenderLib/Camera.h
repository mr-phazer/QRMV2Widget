#pragma once
#include "..\SystemLib\Object.h"

#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

using namespace DirectX;

class Camera :
	public Object
{
public:
	void setAlltoDefault();
	void setFieldOfView(float _fov);
	void setCameraDefault();
	void setProjectionDefault();

	void moveX(float x);
	void moveY(float y);
	void movez(float z);

	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);

	float viewFactor();

	void setToBB(XMFLOAT3 vMin, XMFLOAT3 vMax);

	XMFLOAT3 TrsnalationVector;
	XMFLOAT3 EulerRotation;

	void calculateMatrices(size_t width, size_t height);

	float fov = 0;

	DirectX::XMFLOAT4X4 WorldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(XM_PI);
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjectMatrixPerspective;

	DirectX::XMFLOAT4X4 InvViewMatrix;
	float zoom = 0;
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

private:
	DirectX::XMFLOAT4X4 ProjectMatrixOrtho;
	float m_FOV = 80.0f;
};
