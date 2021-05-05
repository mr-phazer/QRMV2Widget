#pragma once

#include <DirectXMath.h>

using namespace DirectX;
namespace math_func
{
	inline XMFLOAT3 ToEulerAnglesDegrees(const XMFLOAT4& q)
	{


		XMFLOAT3 angles;

		// roll (x-axis rotation)
		double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		angles.x = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			angles.y = copysign(XM_PI / 2, sinp); // use 90 degrees if out of range
		else
			angles.y = asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		angles.z = atan2(siny_cosp, cosy_cosp);

		// Conver the angles from radian to degrees
		angles.x = (angles.x / XM_PI) * 180;
		angles.y = (angles.y / XM_PI) * 180;
		angles.z = (angles.z / XM_PI) * 180;

		return angles;
	};




};

