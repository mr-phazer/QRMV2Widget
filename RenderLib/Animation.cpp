#include "Animation.h"

bool RenderLib::Animation::fillSkeleton(Scene* _poScene, const TWAnim::AnimFile* _poBindPose)
{
	vecBindPose.resize(_poBindPose->oFileHeader.dwBoneCount);
	vecBoneAnimCurves.resize(_poBindPose->oFileHeader.dwBoneCount);

	// fill bind pose
	for (size_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		//		m_vecBoneCurves[bone].init(_poScene);
		vecBindPose[bone].vecKeyFrames.resize(_poBindPose->dwFrameCount);
		//m_vecBoneCurves[bone].initController(_poScene);

		//if (_poBindPose->vecBoneData[bone].parent_id != _poAnimation->vecBoneData[bone].parent_id)
		//{
		//	return { false, "Skeletons do no match: bindpose.parent_id[bone] != anim.parent_id[bone]" };
		//}

		//if (_poBindPose->vecBoneData[bone].strName != _poAnimation->vecBoneData[bone].strName)
		//{
		//	return { false, "Skeletons do no match: bindpose.name[bone] != anim.name[bone]" };
		//}

		//vecBindPose[bone].m_poParentSkeleton = this;
		//vecBindPose[bone].resize(1);
		//vecBindPose[bone].m_strName = _poBindPose->vecBoneData[bone].strName;
		vecBindPose[bone].strBoneName = _poBindPose->vecBoneData[bone].strName;
		vecBindPose[bone].id = bone;
		vecBindPose[bone].parent_id = _poBindPose->vecBoneData[bone].parent_id;

		/*m_vecBoneCurves[bone].m_poParentSkeleton = this;
		m_vecBoneCurves[bone].m_strName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].BoneName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].id = bone;
		m_vecBoneCurves[bone].parent_id = _poAnimation->vecBoneData[bone].parent_id;*/

		vecBindPose[bone].vecKeyFrames[0].vTranslation = _poBindPose->vecKeyFrames[0].vecTranslations[bone];
		vecBindPose[bone].vecKeyFrames[0].vQuaternion = _poBindPose->vecKeyFrames[0].vecQuaternions[bone];

		if (_poBindPose->vecBoneData[bone].strName.find("be_prop") != string::npos)
		{
			//	Vector3 vEuler = math_func::ToEulerAnglesDegrees((_poBindPose->vecKeyFrames[0].vecQuaternions[bone]));

			vecBindPose[bone].vecKeyFrames[0].vQuaternion = Quaternion::Identity;
			//int debug_1 = 1;
			vecBindPose[bone].vecKeyFrames[0].vTranslation = Vector3(0.0f, 0.0f, 0.0f);
		}
	}

	// -- BIND POSE --
	// make a map id -> bone node ptr for later to set parent ptrs
	map<int32_t, BoneAnimCurve*> mapIdtoPtrBindPose;
	map<int32_t, BoneAnimCurve*> mapIdtoPtrAnim;
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		mapIdtoPtrBindPose[bone] = &vecBindPose[bone];
		mapIdtoPtrAnim[bone] = &vecBoneAnimCurves[bone];
	}

	// assign anim+bind pose parent ptrs
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		auto parent_id = _poBindPose->vecBoneData[bone].parent_id;
		if (parent_id > -1)
		{
			vecBindPose[bone].poParentBone = mapIdtoPtrBindPose[parent_id];
			//mapIdtoPtrAnim[parent_id]->addChild(&m_vecBoneCurves[bone]);

			vecBoneAnimCurves[bone].poParentBone = mapIdtoPtrAnim[parent_id];
			//mapIdtoPtrBindPose[parent_id]->addChild(&m_vecBonesBindPose[bone]);
		}
		else
		{
			vecBindPose[bone].poParentBone = nullptr;
			vecBoneAnimCurves[bone].poParentBone = nullptr;
		}
	}

	calculateInverseBindPoseMatrices();

	/*updateChildrenBindPose(&m_vecBonesBindPose[0]);

	vecInverseBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}*/

	return true;
}

void RenderLib::Animation::UpdateGlobalsDiscrete(vector<BoneAnimCurve>& vecIn, size_t _frame)
{
	for (auto& it : vecIn)
	{
		it.updateGlobalsDiscrete(_frame);
	}
}

void RenderLib::Animation::UpdateGlobalsInterPolated(vector<BoneAnimCurve>& vecIn)
{
	for (auto& it : vecIn)
	{
		//it.updateGlobalsIntepolated(m_oClock.animTimePosition(), m_fFrameRate);  // TODO: this is correct code
		it.updateGlobalsIntepolated(0.f, m_fFrameRate); // TODO: remove, this is debugging code
	}
}

void RenderLib::Animation::calculateInverseBindPoseMatrices()
{
	for (size_t b = 0; b < vecBindPose.size(); b++)
	{
		vecBindPose[b].updateGlobalsDiscrete(0);
	}

	vecInverseBindPoseMatrices.resize(vecBindPose.size());
	for (size_t b = 0; b < vecBindPose.size(); b++)
	{
		vecInverseBindPoseMatrices[b] = vecBindPose[b].globalTransformMatrix();

		vecInverseBindPoseMatrices[b].Invert();
	}
}

void RenderLib::Animation::fillAnimation(Scene* _poScene, const TWAnim::AnimFile* _poAnimation)

{
	m_fFrameRate = _poAnimation->oFileHeader.fFrameRate;
	m_fAnimLenght = _poAnimation->oFileHeader.fEndTime;

	m_oClock.setFrameCount(_poAnimation->dwFrameCount);
	m_oClock.setFrameRate(_poAnimation->oFileHeader.fFrameRate);

	// fill frames
	for (size_t bone_index = 0; bone_index < vecBoneAnimCurves.size(); bone_index++)
	{
		// resize the current bone curve to fit frames
		vecBoneAnimCurves[bone_index].vecKeyFrames.resize(_poAnimation->dwFrameCount);
		for (size_t frame = 0; frame < _poAnimation->dwFrameCount; frame++)
		{
			// ---- TRANSLATION DATA ----

			if (

				_poAnimation->vecMaskTranslations[bone_index] < 10000 &&
				_poAnimation->vecMaskTranslations[bone_index] != -1

				)
			{
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];

				vecBoneAnimCurves[bone_index].vecKeyFrames[frame].vTranslation =
					_poAnimation->vecKeyFrames[frame].vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in TRASNSLATION data from the bind pose where the animations contain no data for a bone
			{
				vecBoneAnimCurves[bone_index].vecKeyFrames[frame].vTranslation =
					vecBoneAnimCurves[bone_index].vecKeyFrames[0].vTranslation;
			}

			// ---- QUATERNION DATA ----

			if (

				_poAnimation->vecMaskRotations[bone_index] < 10000 &&
				_poAnimation->vecMaskRotations[bone_index] != -1
				)
			{
				// using the mask, get the index at where to store this data

				vecBoneAnimCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					_poAnimation->vecKeyFrames[frame].vecQuaternions[bone_index];

				// store the curve data at the new bone index
			}
			else // fill in QUATERNION data from the bind pose where the animations contain no data for a bone
			{
				vecBoneAnimCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					vecBoneAnimCurves[bone_index].vecKeyFrames[0].vQuaternion;
			}
		}
	}

	// fill static frame
	vecMaskStaticFrame.resize(vecBoneAnimCurves.size(), false);
	for (size_t bone_index = 0; bone_index < vecBoneAnimCurves.size(); bone_index++)
	{
		vecMaskStaticFrame[bone_index] = false;
		// resize the current bone curve to fit frames
		vecStaticFrame[bone_index].vecKeyFrames.resize(1);
		//for (size_t frame = 0; frame < _poAnimation->dwFrameCount; frame++)
		{
			// ---- TRANSLATION DATA ----

			if (
				_poAnimation->vecMaskTranslations[bone_index] >= 10000

				)
			{
				vecMaskStaticFrame[bone_index] = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				vecStaticFrame[bone_index].vecKeyFrames[0].vTranslation =
					_poAnimation->oStaticFrame.vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in 0 TRASNSLATION
			{
				vecStaticFrame[bone_index].vecKeyFrames[0].vTranslation = { 0,0,0 };
			}

			// ---- QUATERNION DATA ----

			if (
				_poAnimation->vecMaskRotations[bone_index] >= 10000
				)
			{
				vecMaskStaticFrame[bone_index] = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				vecStaticFrame[bone_index].vecKeyFrames[0].vQuaternion =
					_poAnimation->oStaticFrame.vecQuaternions[bone_index]; // store the curve data at the new bone index
			}
			else // fill in indentity quat
			{
				vecStaticFrame[bone_index].vecKeyFrames[0].vQuaternion =
				{ 0, 0, 0, 1 };
			}
		}
	}

	//calculateInverseBindPoseMatrices();

	/*updateChildrenBindPose(&m_vecBonesBindPose[0]);

	m_vecInvBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}*/

	//return true;
}