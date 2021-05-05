#include "Camera.h"

using namespace DirectX::SimpleMath;
float Camera::viewFactor()
{
	Vector3 V1 = { -0.5, 0, 0 };
	Vector3 V2 = { 0.5, 0, 0 };

	// get distance between to points before any transformation
	float distance_before = Vector3::Distance(V1, V2);

	// transform vertices by view and projection
	Matrix mView = ViewMatrix;
	mView.Transpose();

	Matrix mProj = ProjectMatrixPerspective;
	mProj.Transpose();
	V1 = Vector3::Transform(V1, mView);
	V2 = Vector3::Transform(V2, mView);

	//V1 = Vector3::Transform(V1, mProj);
	//V2 = Vector3::Transform(V2, mProj);

	// get distance after transform
	float distance_after = Vector3::Distance(V1, V2);

	// return factor
	return distance_before / distance_after;
}

void Camera::setToBB(XMFLOAT3 vMin, XMFLOAT3 vMax)
{
	Vector3 center = (vMin + vMax) / 2;

	float fov = DirectX::XMConvertToRadians(m_FOV);

	float boxWidth = max(
		vMax.x - vMin.x,
		vMax.y - vMin.y
	);
	zoom = boxWidth / (2.f * tan(fov / 2.f));

	at = center;
}

void Camera::calculateMatrices(size_t width, size_t height)
{
	// Use DirectXMath to create view and perspective matrices.
	using namespace DirectX;

	static float fRotateX = 0.0f;
	static float fRotateY = 0.0f;
	static float fRotateZ = 0.0f;

	//static XMFLOAT3 vTranslate = { 0, 0,0 };

	//SimpleMath::Matrix mWorld = DirectX::XMMatrixTranspose(
	//	/*DirectX::XMMatrixIdentity() * DirectX::XMMatrixRotationY(fRotateY) **/
	//	DirectX::XMMatrixTranslation(vTranslate.x, vTranslate.y, vTranslate.z));

	//WorldMatrix = WorldMatrix; // *mWorld;
	///*DirectX::XMStoreFloat4x4(
	//	&WorldMatrix,

	//)

	//);*/

	////fRotateY += 0.001f;4

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -zoom, 1.0f);

	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(
		&ViewMatrix,
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
		eye,
		at,
		up
	)
	)
	);

	// calculate the INVERSE view matrix to use for camera direction
	DirectX::XMStoreFloat4x4(
		&InvViewMatrix,
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixInverse(NULL,

		DirectX::XMMatrixLookAtLH(
		eye,
		at,
		up
	)

	)
	)
	);

	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	DirectX::XMStoreFloat4x4(
		&ProjectMatrixPerspective,
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_FOV),
		//1.0f,
		aspectRatio,
		0.1f,
		100.0f
	)
	)
	);

	DirectX::XMStoreFloat4x4(
		&ProjectMatrixOrtho,
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixOrthographicLH(
		10,
		10,
		//aspectRatio,
		0.1f,
		100.0f
	)
	)
	);
}

void Camera::setFieldOfView(float _fov)
{
	m_FOV = _fov;
}