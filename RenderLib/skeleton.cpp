#include "pch.h"
#include "skeleton.h"
#include "BoneNode.h"
#include "math_func.h"

//const RenderLib::bone_node& RenderLib::Skeleton::operator[](size_t index) const
//{
//	if (index >= vecBoneTemp.size())
//		return m_oEmptyNode;// return empty, invalid node
//
//	return vecBoneTemp[index];
//}
//
//RenderLib::bone_node& RenderLib::Skeleton::operator[](size_t index, size_t index2)
//{
//	if (index >= vecBoneTemp.size())
//		return m_oEmptyNode;// return empty, invalid node
//
//	return vecBoneTemp[index+index2];
//}
//
//
//RenderLib::bone_node RenderLib::Skeleton::m_oEmptyNode = RenderLib::bone_node(false);

RenderLib::Skeleton* RenderLib::Skeleton::create(Scene* _poScene, const string _strName)
{
	auto po = new Skeleton();

	_poScene->addObject(po);
	po->init(_poScene);
	po->m_strName = _strName;

	return po;
}

vector<Matrix>& RenderLib::Skeleton::updateMatrices()
{
	// update bones linearly
	m_vecCurentBoneMatrices.resize(m_vecBoneCurves.size());
	for (size_t m = 0; m < m_vecBoneCurves.size(); m++)
	{
		m_vecBoneCurves[m].updateGlobalsIntepolated(m_oClock.animTimePosition(), m_fFrameRate);
		m_vecCurentBoneMatrices[m] = m_vecBoneCurves[m].globalTransformMatrix();
	}

	return m_vecCurentBoneMatrices;
}

vector<Matrix>& RenderLib::Skeleton::calculateInverseBindPoseMatrices()
{
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].updateGlobalsDiscrete(0);
	}

	m_vecInvBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}

	return m_vecInvBindPoseMatrices;
}

void RenderLib::Skeleton::draw(Scene* _poScene, Shader::Shader* _poShader)
{
	BaseNode::draw(_poScene);
}

SystemLib::return_value RenderLib::Skeleton::fillSkeletonAndAnimation(Scene* _poScene, const TWAnim::AnimFile* _poBindPose, const TWAnim::AnimFile* _poAnimation)
{
	m_fFrameRate = _poAnimation->oFileHeader.fFrameRate;
	m_fAnimLen = _poAnimation->oFileHeader.fEndTime;

	m_oClock.setFrameCount(_poAnimation->dwFrameCount);
	m_oClock.setFrameRate(_poAnimation->oFileHeader.fFrameRate);

	m_vecBoneCurves.resize(_poAnimation->oFileHeader.dwBoneCount);

	m_vecBonesBindPose.resize(_poBindPose->oFileHeader.dwBoneCount);

	// fill bind pose
	for (size_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		m_vecBoneCurves[bone].init(_poScene);
		m_vecBonesBindPose[bone].init(_poScene);

		if (_poBindPose->vecBoneData[bone].parent_id != _poAnimation->vecBoneData[bone].parent_id)
		{
			return { false, "Skeletons do no match: bindpose.parent_id[bone] != anim.parent_id[bone]" };
		}

		if (_poBindPose->vecBoneData[bone].strName != _poAnimation->vecBoneData[bone].strName)
		{
			return { false, "Skeletons do no match: bindpose.name[bone] != anim.name[bone]" };
		}

		m_vecBonesBindPose[bone].m_poParentSkeleton = this;
		m_vecBonesBindPose[bone].resize(1);
		m_vecBonesBindPose[bone].m_strName = _poBindPose->vecBoneData[bone].strName;
		m_vecBonesBindPose[bone].BoneName = _poBindPose->vecBoneData[bone].strName;
		m_vecBonesBindPose[bone].id = bone;
		m_vecBonesBindPose[bone].parent_id = _poBindPose->vecBoneData[bone].parent_id;

		m_vecBoneCurves[bone].m_poParentSkeleton = this;
		m_vecBoneCurves[bone].m_strName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].BoneName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].id = bone;
		m_vecBoneCurves[bone].parent_id = _poAnimation->vecBoneData[bone].parent_id;

		m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = _poBindPose->vecKeyFrames[0].vecTranslations[bone];
		m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = _poBindPose->vecKeyFrames[0].vecQuaternions[bone];

		if (_poAnimation->vecBoneData[bone].strName.find("be_prop") != string::npos)
		{
			//if (_poAnimation->vecBoneData[bone].strName == "be_prop_1")
			{
				Vector3 vEuler = math_func::ToEulerAnglesDegrees((_poBindPose->vecKeyFrames[0].vecQuaternions[bone]));

				m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = Quaternion::Identity;
				int debug_1 = 1;
				m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = Vector3(0.0f, 0.0f, 0.0f);
			}

			//	m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = {0,0,0};
			//	m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = {0,0,0,1};
		}

		int debug_1 = 1;
	}

	// -- BIND POSE --
	// make a map id -> bone node ptr for later to set parent ptrs
	map<int32_t, BoneNode*> mapIdtoPtrBindPose;
	map<int32_t, BoneNode*> mapIdtoPtrAnim;
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		mapIdtoPtrBindPose[bone] = &m_vecBonesBindPose[bone];
		mapIdtoPtrAnim[bone] = &m_vecBoneCurves[bone];
	}

	// assign anim+bind pose parent ptrs
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		auto parent_id = _poBindPose->vecBoneData[bone].parent_id;
		if (parent_id > -1)
		{
			m_vecBoneCurves[bone].m_poParentBone = mapIdtoPtrAnim[parent_id];
			mapIdtoPtrAnim[parent_id]->addChild(&m_vecBoneCurves[bone]);

			m_vecBonesBindPose[bone].m_poParentBone = mapIdtoPtrBindPose[parent_id];
			mapIdtoPtrBindPose[parent_id]->addChild(&m_vecBonesBindPose[bone]);
		}
		else
		{
			m_vecBoneCurves[bone].m_poParentBone = nullptr;
			m_vecBonesBindPose[bone].m_poParentBone = nullptr;
		}
	}

	// fill frames
	for (size_t bone_index = 0; bone_index < _poBindPose->oFileHeader.dwBoneCount; bone_index++)
	{
		// resize the current bone curve to fit frames
		m_vecBoneCurves[bone_index].vecKeyFrames.resize(_poAnimation->dwFrameCount);
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

				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vTranslation =
					_poAnimation->vecKeyFrames[frame].vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in TRASNSLATION data from the bind pose where the animations contain no data for a bone
			{
				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vTranslation =
					m_vecBonesBindPose[bone_index].vecKeyFrames[0].vTranslation;
			}

			// ---- QUATERNION DATA ----

			if (

				_poAnimation->vecMaskRotations[bone_index] < 10000 &&
				_poAnimation->vecMaskRotations[bone_index] != -1
				)
			{
				// using the mask, get the index at where to store this data

				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					_poAnimation->vecKeyFrames[frame].vecQuaternions[bone_index];

				// store the curve data at the new bone index
			}
			else // fill in QUATERNION data from the bind pose where the animations contain no data for a bone
			{
				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					m_vecBonesBindPose[bone_index].vecKeyFrames[0].vQuaternion;
			}
		}
	}

	// fill static frame

	for (size_t bone_index = 0; bone_index < _poBindPose->oFileHeader.dwBoneCount; bone_index++)
	{
		m_vecBoneCurves[bone_index].bMaskStaticTranslation = false;
		// resize the current bone curve to fit frames
		m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame.resize(1);
		//for (size_t frame = 0; frame < _poAnimation->dwFrameCount; frame++)
		{
			// ---- TRANSLATION DATA ----

			if (
				_poAnimation->vecMaskTranslations[bone_index] >= 10000

				)
			{
				m_vecBoneCurves[bone_index].bMaskStaticTranslation = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vTranslation =
					_poAnimation->oStaticFrame.vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in 0 TRASNSLATION
			{
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vTranslation = { 0,0,0 };
			}

			// ---- QUATERNION DATA ----

			if (
				_poAnimation->vecMaskRotations[bone_index] >= 10000
				)
			{
				m_vecBoneCurves[bone_index].bMaskStaticRotation = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vQuaternion =
					_poAnimation->oStaticFrame.vecQuaternions[bone_index]; // store the curve data at the new bone index
			}
			else // fill in indentity quat
			{
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vQuaternion =
				{ 0, 0, 0, 1 };
			}
		}
	}

	//calculateInverseBindPoseMatrices();

	updateChildrenBindPose(&m_vecBonesBindPose[0]);

	m_vecInvBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}

	return true;
}

SystemLib::return_value RenderLib::Skeleton::fillSkeleton(Scene* _poScene, const TWAnim::AnimFile* _poBindPose)
{
	m_vecBonesBindPose.resize(_poBindPose->oFileHeader.dwBoneCount);
	m_vecBoneCurves.resize(_poBindPose->oFileHeader.dwBoneCount);

	// fill bind pose
	for (size_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		//		m_vecBoneCurves[bone].init(_poScene);
		m_vecBonesBindPose[bone].init(_poScene);
		m_vecBoneCurves[bone].init(_poScene);

		//if (_poBindPose->vecBoneData[bone].parent_id != _poAnimation->vecBoneData[bone].parent_id)
		//{
		//	return { false, "Skeletons do no match: bindpose.parent_id[bone] != anim.parent_id[bone]" };
		//}

		//if (_poBindPose->vecBoneData[bone].strName != _poAnimation->vecBoneData[bone].strName)
		//{
		//	return { false, "Skeletons do no match: bindpose.name[bone] != anim.name[bone]" };
		//}

		m_vecBonesBindPose[bone].m_poParentSkeleton = this;
		m_vecBonesBindPose[bone].resize(1);
		m_vecBonesBindPose[bone].m_strName = _poBindPose->vecBoneData[bone].strName;
		m_vecBonesBindPose[bone].BoneName = _poBindPose->vecBoneData[bone].strName;
		m_vecBonesBindPose[bone].id = bone;
		m_vecBonesBindPose[bone].parent_id = _poBindPose->vecBoneData[bone].parent_id;

		/*m_vecBoneCurves[bone].m_poParentSkeleton = this;
		m_vecBoneCurves[bone].m_strName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].BoneName = _poAnimation->vecBoneData[bone].strName;
		m_vecBoneCurves[bone].id = bone;
		m_vecBoneCurves[bone].parent_id = _poAnimation->vecBoneData[bone].parent_id;*/

		m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = _poBindPose->vecKeyFrames[0].vecTranslations[bone];
		m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = _poBindPose->vecKeyFrames[0].vecQuaternions[bone];

		if (
			_poBindPose->vecBoneData[bone].strName.find("be_prop") != string::npos ||
			_poBindPose->vecBoneData[bone].strName.find("bn_weapon") != string::npos

			)
		{
			Vector3 vEuler = math_func::ToEulerAnglesDegrees((_poBindPose->vecKeyFrames[0].vecQuaternions[bone]));

			m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = Quaternion::Identity;
			m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = Vector3(0.0f, 0.0f, 0.0f);
		}
	}

	// -- BIND POSE --
	// make a map id -> bone node ptr for later to set parent ptrs
	map<int32_t, BoneNode*> mapIdtoPtrBindPose;
	map<int32_t, BoneNode*> mapIdtoPtrAnim;
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		mapIdtoPtrBindPose[bone] = &m_vecBonesBindPose[bone];
		mapIdtoPtrAnim[bone] = &m_vecBoneCurves[bone];
	}

	// assign anim+bind pose parent ptrs
	for (int32_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	{
		auto parent_id = _poBindPose->vecBoneData[bone].parent_id;
		if (parent_id > -1)
		{
			m_vecBoneCurves[bone].m_poParentBone = mapIdtoPtrAnim[parent_id];
			mapIdtoPtrAnim[parent_id]->addChild(&m_vecBoneCurves[bone]);

			m_vecBonesBindPose[bone].m_poParentBone = mapIdtoPtrBindPose[parent_id];
			mapIdtoPtrBindPose[parent_id]->addChild(&m_vecBonesBindPose[bone]);
		}
		else
		{
			m_vecBoneCurves[bone].m_poParentBone = nullptr;
			m_vecBonesBindPose[bone].m_poParentBone = nullptr;
		}
	}

	updateChildrenBindPose(&m_vecBonesBindPose[0]);

	m_vecInvBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}

	return true;
}

void RenderLib::Skeleton::fillAnimation(Scene* _poScene, const TWAnim::AnimFile* _poAnimation)
{
	m_fFrameRate = _poAnimation->oFileHeader.fFrameRate;
	m_fAnimLen = _poAnimation->oFileHeader.fEndTime;

	m_oClock.setFrameCount(_poAnimation->dwFrameCount);
	m_oClock.setFrameRate(_poAnimation->oFileHeader.fFrameRate);

	//m_vecBoneCurves.resize(_poAnimation->oFileHeader.dwBoneCount);

	//m_vecBonesBindPose.resize(_poBindPose->oFileHeader.dwBoneCount);

	// fill bind pose
	//for (size_t bone = 0; bone < _poBindPose->oFileHeader.dwBoneCount; bone++)
	//{
	//	m_vecBoneCurves[bone].initController(_poScene);
	//	m_vecBonesBindPose[bone].initController(_poScene);

	//	if (_poBindPose->vecBoneData[bone].parent_id != _poAnimation->vecBoneData[bone].parent_id)
	//	{
	//		return { false, "Skeletons do no match: bindpose.parent_id[bone] != anim.parent_id[bone]" };
	//	}

	//	if (_poBindPose->vecBoneData[bone].strName != _poAnimation->vecBoneData[bone].strName)
	//	{
	//		return { false, "Skeletons do no match: bindpose.name[bone] != anim.name[bone]" };
	//	}

	//	m_vecBonesBindPose[bone].m_poParentSkeleton = this;
	//	m_vecBonesBindPose[bone].resize(1);
	//	m_vecBonesBindPose[bone].m_strName = _poBindPose->vecBoneData[bone].strName;
	//	m_vecBonesBindPose[bone].BoneName = _poBindPose->vecBoneData[bone].strName;
	//	m_vecBonesBindPose[bone].id = bone;
	//	m_vecBonesBindPose[bone].parent_id = _poBindPose->vecBoneData[bone].parent_id;

	//	m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = _poBindPose->vecKeyFrames[0].vecTranslations[bone];
	//	m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = _poBindPose->vecKeyFrames[0].vecQuaternions[bone];

	//	if (_poAnimation->vecBoneData[bone].strName.find("be_prop") != string::npos)
	//	{
	//		//if (_poAnimation->vecBoneData[bone].strName == "be_prop_1")
	//		{
	//			Vector3 vEuler = math_func::ToEulerAnglesDegrees((_poBindPose->vecKeyFrames[0].vecQuaternions[bone]));

	//			m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = Quaternion::Identity;
	//			int debug_1 = 1;
	//			m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = Vector3(0.0f, 0.0f, 0.0f);
	//		}

	//		//	m_vecBonesBindPose[bone].vecKeyFrames[0].vTranslation = {0,0,0};
	//		//	m_vecBonesBindPose[bone].vecKeyFrames[0].vQuaternion = {0,0,0,1};
	//	}

	//	int debug_1 = 1;
	//}

	// -- BIND POSE --
	// make a map id -> bone node ptr for later to set parent ptrs
	//map<int32_t, BoneNode*> mapIdtoPtrBindPose;
	//map<int32_t, BoneNode*> mapIdtoPtrAnim;
	//for (int32_t bone = 0; bone < m_vecBonesBindPose.size(); bone++)
	//{
	//	mapIdtoPtrBindPose[bone] = &m_vecBonesBindPose[bone];
	//	mapIdtoPtrAnim[bone] = &m_vecBoneCurves[bone];
	//}

	// assign anim+bind pose parent ptrs
	//for (int32_t bone = 0; bone < m_vecBonesBindPose.size(); bone++)
	//{
	//	auto parent_id = _poBindPose->vecBoneData[bone].parent_id;
	//	if (parent_id > -1)
	//	{
	//		m_vecBoneCurves[bone].m_poParentBone = mapIdtoPtrAnim[parent_id];
	//		mapIdtoPtrAnim[parent_id]->addChild(&m_vecBoneCurves[bone]);

	//		m_vecBonesBindPose[bone].m_poParentBone = mapIdtoPtrBindPose[parent_id];
	//		mapIdtoPtrBindPose[parent_id]->addChild(&m_vecBonesBindPose[bone]);
	//	}
	//	else
	//	{
	//		m_vecBoneCurves[bone].m_poParentBone = nullptr;
	//		m_vecBonesBindPose[bone].m_poParentBone = nullptr;
	//	}
	//}

	// fill frames
	for (size_t bone_index = 0; bone_index < m_vecBoneCurves.size(); bone_index++)
	{
		m_vecBoneCurves[bone_index].m_poParentSkeleton = this;
		m_vecBoneCurves[bone_index].m_strName = _poAnimation->vecBoneData[bone_index].strName;
		m_vecBoneCurves[bone_index].BoneName = _poAnimation->vecBoneData[bone_index].strName;
		m_vecBoneCurves[bone_index].id = bone_index;
		m_vecBoneCurves[bone_index].parent_id = _poAnimation->vecBoneData[bone_index].parent_id;

		// resize the current bone curve to fit frames
		m_vecBoneCurves[bone_index].vecKeyFrames.resize(_poAnimation->dwFrameCount);
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

				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vTranslation =
					_poAnimation->vecKeyFrames[frame].vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in TRASNSLATION data from the bind pose where the animations contain no data for a bone
			{
				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vTranslation =
					m_vecBonesBindPose[bone_index].vecKeyFrames[0].vTranslation;
			}

			// ---- QUATERNION DATA ----

			if (

				_poAnimation->vecMaskRotations[bone_index] < 10000 &&
				_poAnimation->vecMaskRotations[bone_index] != -1
				)
			{
				// using the mask, get the index at where to store this data

				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					_poAnimation->vecKeyFrames[frame].vecQuaternions[bone_index];

				// store the curve data at the new bone index
			}
			else // fill in QUATERNION data from the bind pose where the animations contain no data for a bone
			{
				m_vecBoneCurves[bone_index].vecKeyFrames[frame].vQuaternion =
					m_vecBonesBindPose[bone_index].vecKeyFrames[0].vQuaternion;
			}
		}
	}

	// fill static frame

	for (size_t bone_index = 0; bone_index < m_vecBoneCurves.size(); bone_index++)
	{
		m_vecBoneCurves[bone_index].bMaskStaticTranslation = false;
		// resize the current bone curve to fit frames
		m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame.resize(1);
		//for (size_t frame = 0; frame < _poAnimation->dwFrameCount; frame++)
		{
			// ---- TRANSLATION DATA ----

			if (
				_poAnimation->vecMaskTranslations[bone_index] >= 10000

				)
			{
				m_vecBoneCurves[bone_index].bMaskStaticTranslation = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vTranslation =
					_poAnimation->oStaticFrame.vecTranslations[bone_index]; // store the curve data at the new bone index
			}
			else // fill in 0 TRASNSLATION
			{
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vTranslation = { 0,0,0 };
			}

			// ---- QUATERNION DATA ----

			if (
				_poAnimation->vecMaskRotations[bone_index] >= 10000
				)
			{
				m_vecBoneCurves[bone_index].bMaskStaticRotation = true;
				// using the mask, get the index at where to store this data
				//auto new_bone_index = _poAnimation->vecMaskTranslations[bone_index];
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vQuaternion =
					_poAnimation->oStaticFrame.vecQuaternions[bone_index]; // store the curve data at the new bone index
			}
			else // fill in indentity quat
			{
				m_vecBoneCurves[bone_index].vecKeyFramesStaticFrame[0].vQuaternion =
				{ 0, 0, 0, 1 };
			}
		}
	}

	calculateInverseBindPoseMatrices();

	/*updateChildrenBindPose(&m_vecBonesBindPose[0]);

	m_vecInvBindPoseMatrices.resize(m_vecBonesBindPose.size());
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].globalTransformMatrix();

		m_vecInvBindPoseMatrices[b] = m_vecBonesBindPose[b].m_xmGlobalTransform.Invert();
	}*/

	//return true;
}

const RenderLib::BoneNode& RenderLib::Skeleton::operator[](size_t index) const
{
	return m_vecBoneCurves[index];
}

RenderLib::BoneNode& RenderLib::Skeleton::operator[](size_t index)
{
	return m_vecBoneCurves[index];
}

uint32_t RenderLib::Skeleton::getIdFromName(const string& _strBoneMame) const
{
	for (uint32_t i = 0; i < m_vecBonesBindPose.size(); i++)
	{
		// RMV2 version > 6
		if (m_vecBonesBindPose[i].BoneName == _strBoneMame)
			return i;

		// RMV2 version <= 6
		std::string strBone = ("bn_" + _strBoneMame);
		if (m_vecBonesBindPose[i].BoneName == strBone)
			return i;
	}

	return -1;
}

float RenderLib::Skeleton::getFrameRate() const
{
	return m_fFrameRate;
}

float RenderLib::Skeleton::getLocalTime()
{
	return m_oClock.animTimePosition();
}

void RenderLib::Skeleton::updateChildrenBindPose(BoneNode* _poParent)
{
	_poParent->updateGlobalsDiscrete(0);

	for (auto& it : _poParent->m_vecChildren)
	{
		BoneNode* poChildBone = dynamic_cast<BoneNode*>(it);

		updateChildrenBindPose(poChildBone);
	}
}

void RenderLib::Skeleton::updateChildrenAnimation(BoneNode* _poParent, float _fTime, float _fFrameRate)
{
	_poParent->updateGlobalsIntepolated(_fTime, _fFrameRate);

	for (auto& it : _poParent->m_vecChildren)
	{
		BoneNode* poChildBone = dynamic_cast<BoneNode*>(it);

		updateChildrenAnimation(poChildBone, _fTime, _fFrameRate);
	}
}

void RenderLib::Skeleton::updateLinesBindPose(Scene* _poScene)
{
	if (!m_spoBoneLines) // if the lines do not exist, create them
	{
		m_spoBoneLines = shared_ptr<SubMeshData>(new SubMeshData);

		m_spoBoneLines->createVertexBuffer(
			_poScene->getDevice(),
			(UINT)(m_vecBonesBindPose.size() * sizeof(ModelVertex))
		);

		m_spoBoneLines->createIndexBuffer(
			_poScene->getDevice(),
			(UINT)(m_vecBonesBindPose.size() * sizeof(uint32_t) * 2));

		m_spoBoneLines->setIndexCount((UINT)m_vecBonesBindPose.size() * 2);

		m_spoBoneLines->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		m_spoBoneLines->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		m_spoBoneLines->setPixelShader(
			_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor)
		);

		m_spoBoneLines->setVertexShader(
			_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid)
		);

		m_spoBoneLines->setInputLayout(
			_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid)
		);
	}

	/*************************************
		for each bone:

		store indexes, to there is a line from each bone to each parent

	***************************************/

	// lock (map) the index VRAM, making is accessible to the CPU
	D3D11_MAPPED_SUBRESOURCE poBufferInfo;
	HRESULT hr = _poScene->getDeviceContext()->Map(m_spoBoneLines->indexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	assert(SUCCEEDED(hr));

	vector<uint32_t> vecIndices;
	for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
	{
		if (m_vecBonesBindPose[b].parent_id < 0) // if bone has no parent,
		{
			vecIndices.push_back(m_vecBonesBindPose[b].id);
			vecIndices.push_back(m_vecBonesBindPose[b].id);
		}
		else
		{
			// make line between bone and its parent
			vecIndices.push_back(m_vecBonesBindPose[b].parent_id);
			vecIndices.push_back(m_vecBonesBindPose[b].id);
		}
	}
	m_spoBoneLines->setIndexCount((UINT)vecIndices.size());
	// copy the INDICES into VRAM and unlock (unmap) that VRAM
	memcpy(poBufferInfo.pData, &vecIndices[0], vecIndices.size() * sizeof(uint32_t));
	_poScene->getDeviceContext()->Unmap(m_spoBoneLines->indexBuffer(), 0);

	/*************************************
		for each bone:

		position the vertex

	***************************************/
	// lock (map) the vertex VRAM, making is accessible to the CPU
	hr = _poScene->getDeviceContext()->Map(m_spoBoneLines->vertexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	assert(SUCCEEDED(hr));

	// update vertices
	vector<ModelVertex> vecVertices; // ( m_vecBonesBindPose.size() );
	int len = m_vecBonesBindPose.size();
	for (int b = 0; b < len; b++)
	{
		Vector3 pos = { 0,0,0 };
		pos = Vector3::Transform(pos, m_vecBonesBindPose[b].m_xmGlobalTransform); // transform vertex into place

		ModelVertex vert;

		vert.position.x = pos.x;
		vert.position.y = pos.y;
		vert.position.z = pos.z;
		//vert.position.w = 0;

		vert.color = { 0.8, 0.8, 1, 1 };

		vecVertices.push_back(vert);
	}
	// copy vertices into VRAM and unlock (unmap) the that VRAM
	memcpy(poBufferInfo.pData, &vecVertices[0], vecVertices.size() * sizeof(ModelVertex));
	_poScene->getDeviceContext()->Unmap(m_spoBoneLines->vertexBuffer(), 0);
}

void RenderLib::Skeleton::updateLinesAnimation(Scene* _poScene)
{
	if (!m_spoBoneLines) // if the lines do not exist, create them
	{
		m_spoBoneLines = shared_ptr<SubMeshData>(new SubMeshData);

		m_spoBoneLines->createVertexBuffer(
			_poScene->getDevice(),
			(UINT)(m_vecBoneCurves.size() * sizeof(ModelVertex))
		);

		m_spoBoneLines->createIndexBuffer(
			_poScene->getDevice(),
			(UINT)(m_vecBoneCurves.size() * sizeof(uint32_t) * 2));

		m_spoBoneLines->setIndexCount((UINT)m_vecBoneCurves.size() * 2);

		m_spoBoneLines->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		m_spoBoneLines->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		m_spoBoneLines->setPixelShader(
			_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor)
		);

		m_spoBoneLines->setVertexShader(
			_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSolid)
		);

		m_spoBoneLines->setInputLayout(
			_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSolid)
		);

		m_spoBoneLines->setRasterizerState(
			_poScene->getResources()->getRasterizerState(Resources::ERasterizers::eSolid));
	}

	/*************************************
		for each bone:

		store indexes, to there is a line from each bone to each parent

	***************************************/

	// lock (map) the index VRAM, making is accessible to the CPU
	D3D11_MAPPED_SUBRESOURCE poBufferInfo;
	HRESULT hr = _poScene->getDeviceContext()->Map(m_spoBoneLines->indexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	assert(SUCCEEDED(hr));

	vector<uint32_t> vecIndices;
	for (size_t b = 0; b < m_vecBoneCurves.size(); b++)
	{
		if (m_vecBoneCurves[b].parent_id < 0) // if bone has no parent,
		{
			vecIndices.push_back(m_vecBoneCurves[b].id);
			vecIndices.push_back(m_vecBoneCurves[b].id);
		}
		else
		{
			// make line between bone and its parent
			vecIndices.push_back(m_vecBoneCurves[b].parent_id);
			vecIndices.push_back(m_vecBoneCurves[b].id);
		}
	}
	m_spoBoneLines->setIndexCount((UINT)vecIndices.size());
	// copy the INDICES into VRAM and unlock (unmap) that VRAM
	memcpy(poBufferInfo.pData, &vecIndices[0], vecIndices.size() * sizeof(uint32_t));
	_poScene->getDeviceContext()->Unmap(m_spoBoneLines->indexBuffer(), 0);

	/*************************************
		for each bone:

		position the vertex

	***************************************/
	// lock (map) the vertex VRAM, making is accessible to the CPU
	hr = _poScene->getDeviceContext()->Map(m_spoBoneLines->vertexBuffer(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &poBufferInfo);
	assert(SUCCEEDED(hr));

	// update vertices
	vector<ModelVertex> vecVertices; // ( m_vecBoneCurves.size() );
	int len = m_vecBoneCurves.size();
	for (int b = 0; b < len; b++)
	{
		Vector3 pos = { 0,0,0 };
		pos = Vector3::Transform(pos, m_vecBoneCurves[b].m_xmGlobalTransform); // transform vertex into place

		ModelVertex vert;

		vert.position.x = pos.x;
		vert.position.y = pos.y;
		vert.position.z = pos.z;
		//vert.position.w = 0;

		vert.color = { 0.8, 0.8, 1, 1 };

		vecVertices.push_back(vert);
	}
	// copy vertices into VRAM and unlock (unmap) the that VRAM
	memcpy(poBufferInfo.pData, &vecVertices[0], vecVertices.size() * sizeof(ModelVertex));
	_poScene->getDeviceContext()->Unmap(m_spoBoneLines->vertexBuffer(), 0);
}

void RenderLib::Skeleton::render(Scene* _poScene, Shader::Shader* _poShader)
{
	if (isAnimationsLoaded() && !m_bInBindPose)
	{
		renderAnimation(_poScene);
	}
	else if (isBindPoseLoaded())
	{
		renderBindPose(_poScene);
	}
}

void RenderLib::Skeleton::renderBindPose(Scene* _poScene, Shader::Shader* _poShader)
{
	updateGlobalMatrix();

	for (size_t boneIndex = 0; boneIndex < m_vecBonesBindPose.size(); boneIndex++)
	{
		m_vecBonesBindPose[boneIndex].setCubeVisible(true);

		// TODO: change this so the "bone line" + "joint cubes" are also using GPU skinning
		m_vecBonesBindPose[boneIndex].m_xmGlobalTransform = m_vecBonesBindPose[boneIndex].globalTransformMatrix();

		_poScene->m_VS_PerModel_ConstantBuffer.tranforms[boneIndex] = m_vecBonesBindPose[boneIndex].m_xmGlobalTransform.Transpose();
		_poScene->m_VS_PerModel_ConstantBuffer.inverse[boneIndex] = m_vecInvBindPoseMatrices[boneIndex].Transpose();
	}

	if (visible())
	{
		updateLinesBindPose(_poScene);

		for (size_t b = 0; b < m_vecBonesBindPose.size(); b++)
		{
			m_vecBonesBindPose[b].m_xmGlobalTransform = m_vecBonesBindPose[b].m_xmGlobalTransform * this->m_xmGlobalTransform;
			m_vecBonesBindPose[b].render(_poScene);
		}
	}

	if (visible())
	{
		renderBoneLines(_poScene);
		m_spoBoneLines->drawMesh(_poScene);
	}
}

void RenderLib::Skeleton::renderAnimation(RenderLib::Scene* _poScene, Shader::Shader* _poShader)
{
	updateGlobalMatrix();

	for (size_t boneIndex = 0; boneIndex < m_vecBoneCurves.size(); boneIndex++)
	{
		m_vecBoneCurves[boneIndex].updateGlobalsIntepolated(m_oClock.animTimePosition(), m_fFrameRate);

		int debug_1 = 1;
	}

	//updateChildrenAnimation(&m_vecBoneCurves[0], m_oClock.animTimePosition(), m_fFrameRate);

	for (size_t boneIndex = 0; boneIndex < m_vecBoneCurves.size(); boneIndex++)
	{
		m_vecBoneCurves[boneIndex].setCubeVisible(true);

		// TODO: change this so the "bone line" + "joint cubes" are also using GPU skinning
		m_vecBoneCurves[boneIndex].m_xmGlobalTransform = m_vecBoneCurves[boneIndex].globalTransformMatrix();

		_poScene->m_VS_PerModel_ConstantBuffer.tranforms[boneIndex] = m_vecBoneCurves[boneIndex].m_xmGlobalTransform.Transpose();
		_poScene->m_VS_PerModel_ConstantBuffer.inverse[boneIndex] = m_vecInvBindPoseMatrices[boneIndex].Transpose();
	}

	if (visible())
	{
		updateLinesAnimation(_poScene);

		for (size_t boneIndex = 0; boneIndex < m_vecBoneCurves.size(); boneIndex++)
		{
			m_vecBoneCurves[boneIndex].setCubeVisible(visible());
			m_vecBoneCurves[boneIndex].m_xmGlobalTransform = m_vecBoneCurves[boneIndex].m_xmGlobalTransform * this->m_xmGlobalTransform;
			m_vecBoneCurves[boneIndex].render(_poScene);
		}
	}

	if (visible())
	{
		renderBoneLines(_poScene);
		m_spoBoneLines->drawMesh(_poScene);
	}
}

void RenderLib::Skeleton::renderBoneLines(RenderLib::Scene* _poScene, Shader::Shader* _poShader)
{
	//_poScene->m_VS_PerModel_ConstantBuffer.mWorld = Matrix::Identity;
	_poScene->m_VS_PerModel_ConstantBuffer.mWorld = m_xmGlobalTransform.Transpose();

	_poScene->updateVSPerModelConstBuffer();
}
bool RenderLib::Skeleton::isAnimationsLoaded()
{
	if (m_vecBoneCurves.size() > 0)
		if (m_vecBoneCurves[0].vecKeyFrames.size() > 0)
			return true;

	return false;
}

bool RenderLib::Skeleton::isBindPoseLoaded()
{
	if (m_vecBonesBindPose.size() > 0)
		if (m_vecBonesBindPose[0].vecKeyFrames.size() > 0)
			return true;

	return false;
}

void RenderLib::Skeleton::setToBindPose(bool _bState)
{
	m_bInBindPose = _bState;
}