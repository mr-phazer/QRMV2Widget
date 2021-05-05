#pragma once

#pragma once

#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

#include "IDrawAbleNode.h"
#include "..\ImporterLib\AnimFIle.h"
#include "..\SystemLib\system_clock.h"

using namespace DirectX::SimpleMath;

#include "IAbstractObject.h"

namespace RenderLib
{
	struct KeyFrameData {
		DirectX::SimpleMath::Vector3 vTranslation;
		Quaternion vQuaternion;
	};

	class BoneAnimCurve :
		public IAbstractObject
	{
		friend class Skeleton;

	public:
		int32_t id = -2;
		string strBoneName = "n/a";
		int32_t parent_id = -2;
		BoneAnimCurve* poParentBone = nullptr;

	public:
		//BoneAnimCurve() {};

		void updateGlobalsDiscrete(uint32_t _frame_index);
		void updateGlobalsIntepolated(float _fTime, float _fFrameRate);

		Matrix globalTransformMatrix();

		void resize(size_t _frames)
		{
			vecKeyFrames.resize(_frames);
		}

	protected:
		// used in track blenbing
//		Vector3& getGlobalTranslationInterpolated();
	//	Quaternion& getLocalGlobalQuaternionInterpolated();

	private:
		// Local translation/Quaternion
		// interpolation
		Vector3& getLocalTranslationInterpolated(float _fTime, float _fFrameRate);

		Quaternion& getLocalRotationQuaternionInterpolated(float _fTime, float _fFrameRate);

	public:
		vector<KeyFrameData> vecKeyFrames;

		SimpleMath::Matrix mGlobalTransform = SimpleMath::Matrix::Identity;

	private:
		uint32_t m_frame_index;
		float m_t;
	private:
		Vector3 m_translationGlobal;
		Quaternion m_quaternionGlobal;

		Vector3 m_oTempLocalTranslation;
		Quaternion m_oTempLocalRotationQuaternion;
	};
}
