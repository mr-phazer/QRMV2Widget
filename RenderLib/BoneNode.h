#pragma once

#include <DirectXMath.h>
#include "..\DirectXTK\Inc\SimpleMath.h"

#include "IDrawAbleNode.h"
#include "..\ImporterLib\AnimFIle.h"
#include "..\SystemLib\system_clock.h"
#include "BoneAnimCurve.h"

using namespace DirectX::SimpleMath;

namespace RenderLib
{
	struct FrameData {
		DirectX::SimpleMath::Vector3 vTranslation;
		Quaternion vQuaternion;
	};

	//XMVECTOR SmoothStep(XMVECTOR V0, XMVECTOR V1, float t)
	//{
	//	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	//	t = t * t * (3.f - 2.f * t);
	//	return XMVectorLerp(V0, V1, t);
	//}

	class Skeleton;

	class BoneNode : public BaseNode
	{
		friend class Skeleton;

		int32_t id = -2;
		string BoneName = "n/a";
		int32_t parent_id = -2;

	public:
		BoneNode() {};
		static BoneNode* create(Skeleton* _poParentSkeleton, const string& _strName = "");

		//virtual void draw(ID3D11DeviceContext* _poContext) override;
		virtual void render(Scene* _poScene, Shader::Shader* _poShader = nullptr) override;

		void resize(size_t _frames)
		{
			vecKeyFrames.resize(_frames);
		}

		// blends two track, 0.0 = this track.. 1.0 = the '_poBone' track
		// take the interpolated value in each and return the LERPed and SLERPed values (using blendWeight as t)
		BoneNode blendWith(BoneNode* _poBone2, float _fBlendWeight);

		Skeleton* getParentSkeleton();
		BoneNode* getParentBone();

		// Global transformatices
		Matrix globalTransformMatrix();
		Matrix getGlobalTransformInverse();

		// Update Global
		void updateGlobalsIntepolated();
		void updateGlobalsIntepolated(float _fTime, float _fFrameRate);
		void updateGlobalsDiscrete(uint32_t _frame_index);

	protected:
		// used in track blenbing
		Vector3& getGlobalTranslationInterpolated();
		Quaternion& getLocalGlobalQuaternionInterpolated();

	private:
		// Local translation/Quaternion
		Vector3& getLocalTranslationInterpolated();
		Quaternion& getLocalRotationQuaternionInterpolated();

		Vector3& getLocalTranslationInterpolated(float _fTime, float _fFrameRate);
		Quaternion& getLocalRotationQuaternionInterpolated(float _fTime, float _fFrameRate);

		Vector3& getLocalTranslationBlended();
		Quaternion& getLocalRotationBlended();

		/*	Vector3& getLocalTranslation(float _fTime, float _fFrameRate);
			Quaternion& getLocalRotation(float _fTime, float _fFrameRate);*/

		Skeleton* m_poParentSkeleton = nullptr;
		BoneNode* m_poParentBone = nullptr;

	public:
		// bind pose data for thie bone
		KeyFrameData oBindPoseFrame;

		// list of curves to be summed (assuming they affect different part of the skeleton).
		vector<BoneAnimCurve*> m_vecpoAnimCurves = vector<BoneAnimCurve*>(1);

		// the 2 curver behing blended using interpolation
		BoneAnimCurve* m_ppBlendCurver[2];

		// Blending interpolation weight, 0.0 = curve 1, 1.0 = curve 2
		float m_fBlendWeight = 1.0;

		// Blend in time (frames or seconds?)
		float m_fBlendInTime = 10.0f;

		bool m_bIsBlending = false;

		vector<FrameData> vecKeyFrames;
		vector<FrameData> vecKeyFramesStaticFrame;
		bool bMaskStaticTranslation = false;
		bool bMaskStaticRotation = false;;

		//Vector3 m_translationLocal;
		//Quaternion m_quaternionLocal;

		Vector3 m_translationGlobal = { 0,0,0 };
		Quaternion  m_quaternionGlobal = { 0,0,0,1 };
		Quaternion  m_quaternionOrientation = { 0,0,0,1 };;

	private:
		void updateFrameIndexAndT();

	private:
		uint32_t m_frame_index = 0;
		float m_t = 0.f;
	private:
		Vector3 m_oTempLocalTranslation;
		Quaternion m_oTempLocalRotationQuaternion;

		Matrix m_oTempGlobalTransform;
		Matrix m_oTempGlobalTransformInverse;

		//BoneNode* _poBlendTrack = nullptr;
		//float m_fBlendWeight = 0;
	};

	struct SkeletonAnimationData
	{
		void resize(size_t _bones, size_t _frames)
		{
			vecBones.resize(_bones);
			for (auto& it : vecBones)
			{
				it.resize(_frames);
			}
		}

		vector<BoneNode> vecBones;
	};
}