#include "BoneAnimCurve.h"

void RenderLib::BoneAnimCurve::updateGlobalsDiscrete(uint32_t _frame_index)
{
	BoneAnimCurve* parent = poParentBone;
	if (parent != nullptr)
	{
		m_quaternionGlobal = vecKeyFrames[_frame_index].vQuaternion * parent->m_quaternionGlobal;

		m_translationGlobal = parent->m_translationGlobal +
			Vector3::Transform(vecKeyFrames[_frame_index].vTranslation, parent->m_quaternionGlobal);
	}
	else
	{
		m_translationGlobal = vecKeyFrames[_frame_index].vTranslation;
		m_quaternionGlobal = vecKeyFrames[_frame_index].vQuaternion;
	}
}

void RenderLib::BoneAnimCurve::updateGlobalsIntepolated(float _fTime, float _fFrameRate)
{
	BoneAnimCurve* parent = poParentBone;
	if (parent != nullptr)
	{
		m_quaternionGlobal = getLocalRotationQuaternionInterpolated(_fTime, _fFrameRate)
			* parent->m_quaternionGlobal;

		m_translationGlobal = parent->m_translationGlobal +
			Vector3::Transform(getLocalTranslationInterpolated(_fTime, _fFrameRate), parent->m_quaternionGlobal);
	}
	else
	{
		m_translationGlobal = getLocalTranslationInterpolated(_fTime, _fFrameRate);
		m_quaternionGlobal = getLocalRotationQuaternionInterpolated(_fTime, _fFrameRate);

		int debug_1 = 1;
	}
}

Matrix RenderLib::BoneAnimCurve::globalTransformMatrix()
{
	DirectX::SimpleMath::Matrix translationMatrix;
	DirectX::SimpleMath::Matrix rotationMatrix;

	translationMatrix = Matrix::CreateTranslation(m_translationGlobal);
	//Matrix transposedTranslationMatrix = translationMatrix.Transpose();

	rotationMatrix = Matrix::CreateFromQuaternion(m_quaternionGlobal);
	auto length = m_quaternionGlobal.Length();

	Matrix matrixResult = (rotationMatrix * translationMatrix);

	return matrixResult;
}

Vector3& RenderLib::BoneAnimCurve::getLocalTranslationInterpolated(float _fTime, float _fFrameRate)

{
	//if (bMaskStaticTranslation)
	//{
	//	m_oTempLocalTranslation = vecKeyFramesStaticFrame[0].vTranslation;
	//	return m_oTempLocalTranslation;
	//}

	// ---------------------------------
	// get frame index integer and T
	// ---------------------------------
	//double len = ( 1.0 / _fFrameRate ) * ( double ) vecFrames.size(); // lenght of animations in seconds
	//double position = _fTime / len; // position in the animations, unit less
	float position = _fFrameRate * _fTime; // position in the animations, unit "frame"
	size_t frame_index = position; // discrete frame index by trunacting (frame/sec) * time

	float t = position - (float)frame_index;

	// ---------------------------------
	// Interpoolate
	// ---------------------------------
	if (frame_index == vecKeyFrames.size() - 1) // if the last frame interpolate with the first frame
	{
		// ------- Track Blending -------
		//Vector3 trans_frame1 = Vector3::Lerp(vecKeyFrames[frame_index].vTranslation,
		//							   _poBlendTrack->vecKeyFrames[frame_index].vTranslation, m_fBlendWeight);
		//Vector3 trans_frame2 = Vector3::Lerp(vecKeyFrames[0].vTranslation,
		//							   _poBlendTrack->vecKeyFrames[0].vTranslation, m_fBlendWeight);

		//m_oTempLocalTranslation = Vector3::Lerp(trans_frame1, trans_frame2, t);

		m_oTempLocalTranslation = Vector3::Lerp(vecKeyFrames[frame_index].vTranslation, vecKeyFrames[0].vTranslation, t); // interpolate wio
	}
	else
	{
		m_oTempLocalTranslation = Vector3::Lerp(vecKeyFrames[frame_index].vTranslation, vecKeyFrames[frame_index + 1].vTranslation, t);
	}

	return m_oTempLocalTranslation;
}

Quaternion& RenderLib::BoneAnimCurve::getLocalRotationQuaternionInterpolated(float _fTime, float _fFrameRate)
{
	//if (bMaskStaticRotation)
	//{
	//	m_oTempLocalRotationQuaternion = vecKeyFramesStaticFrame[0].vQuaternion;
	//	return m_oTempLocalRotationQuaternion;
	//}

	// ---------------------------------
	// get frame index integer and T
	// ---------------------------------
	//double len = ( 1.0 / frame_rate ) * ( double ) vecFrames.size(); // lenght of animations in seconds
	//double position = _fTime / len; // position in the animations, unit less
	float position = _fFrameRate * _fTime; // position in the animations, unit "frame"
	size_t frame_index = position; // discrete frame index

	float t = position - (float)frame_index;

	// ---------------------------------
	// Interpoolate
	// ---------------------------------
	if (frame_index == vecKeyFrames.size() - 1) // if the last frame interpolate with the first frame
	{
		m_oTempLocalRotationQuaternion = Quaternion::Slerp(vecKeyFrames[frame_index].vQuaternion, vecKeyFrames[0].vQuaternion, t); // interpolate wio
	}
	else
	{
		m_oTempLocalRotationQuaternion = Quaternion::Slerp(vecKeyFrames[frame_index].vQuaternion, vecKeyFrames[frame_index + 1].vQuaternion, t);
	}

	return m_oTempLocalRotationQuaternion;
}