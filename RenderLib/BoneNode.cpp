#include "BoneNode.h"
#include "SKELETON.H"
#include "math_func.h"

Quaternion& RenderLib::BoneNode::getLocalRotationQuaternionInterpolated()
{
	float _fTime = getParentSkeleton()->getLocalTime();
	float frame_rate = getParentSkeleton()->getFrameRate();

	// ---------------------------------
	// get frame index integer and T
	// ---------------------------------
	//double len = ( 1.0 / frame_rate ) * ( double ) vecFrames.size(); // lenght of animations in seconds
	//double position = _fTime / len; // position in the animations, unit less
	float position = frame_rate * _fTime; // position in the animations, unit "frame"
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

Vector3& RenderLib::BoneNode::getLocalTranslationInterpolated(float _fTime, float _fFrameRate)
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

Quaternion& RenderLib::BoneNode::getLocalRotationQuaternionInterpolated(float _fTime, float _fFrameRate)
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

void RenderLib::BoneNode::updateGlobalsIntepolated()
{
	throw bad_function_call();
	/*BoneNode* parent = getParentBone();
	if (parent != nullptr)
	{
		m_quaternionGlobal = parent->m_quaternionGlobal * getLocalRotationQuaternionInterpolated();

		m_translationGlobal = parent->m_translationGlobal +
			Vector3::Transform(getLocalTranslationInterpolated(), parent->m_quaternionGlobal);
	}
	else
	{
		m_quaternionGlobal = getLocalRotationQuaternionInterpolated();
		m_translationGlobal = getLocalTranslationInterpolated();
	}*/
}

void RenderLib::BoneNode::updateGlobalsIntepolated(float _fTime, float _fFrameRate)
{
	BoneNode* parent = getParentBone();
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

void RenderLib::BoneNode::updateGlobalsDiscrete(uint32_t _frame_index)
{
	BoneNode* parent = getParentBone();
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

Vector3& RenderLib::BoneNode::getLocalTranslationInterpolated()
{
	float _fTime = getParentSkeleton()->getLocalTime();
	float frame_rate = getParentSkeleton()->getFrameRate();

	// ---------------------------------
	// get frame index integer and T
	// ---------------------------------
	//double len = ( 1.0 / frame_rate ) * ( double ) vecFrames.size(); // lenght of animations in seconds
	//double position = _fTime / len; // position in the animations, unit less
	float position = frame_rate * _fTime; // position in the animations, unit "frame"
	size_t frame_index = position; // discrete frame index

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

//void RenderLib::BoneNode::draw(ID3D11DeviceContext* _poContext)
//{
//
//}

void RenderLib::BoneNode::render(Scene* _poScene, Shader::Shader* _poShader)
{
	//updateGlobalMatrix();

	//_poScene->m_VS_PerModel_ConstantBuffer.projection =
	//	_poScene->getCamera().ProjectMatrixPerspective;
	_poScene->m_VS_PerModel_ConstantBuffer.mWorld = m_xmGlobalTransform.Transpose();
	_poScene->updateVSPerModelConstBuffer();

	setCubeVisible(true);
	draw(_poScene);
}

RenderLib::Skeleton* RenderLib::BoneNode::getParentSkeleton()
{
	return m_poParentSkeleton;
}

RenderLib::BoneNode* RenderLib::BoneNode::getParentBone()
{
	return m_poParentBone;
}

Matrix RenderLib::BoneNode::globalTransformMatrix()
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

Matrix RenderLib::BoneNode::getGlobalTransformInverse()
{
	Matrix trans;
	Matrix rot;

	trans = Matrix::CreateTranslation(m_translationGlobal);

	rot = Matrix::CreateFromQuaternion(m_quaternionGlobal);

	return (trans * rot).Invert();
	//return DirectX::SimpleMath::operator*(trans, rot).Invert();
}

void RenderLib::BoneNode::updateFrameIndexAndT()
{
	float _fTime = (float)m_poParentSkeleton->m_oClock.getLocalTime();

	// ---------------------------------
	// get frame index integer and T
	// ---------------------------------
	float len = (1.0 / m_poParentSkeleton->m_fFrameRate) * (float)vecKeyFrames.size(); // lenght of animations in seconds
	float position = _fTime / len; // position in the animations, unit less
	m_frame_index = vecKeyFrames.size() * position;

	m_t = position - (float)m_frame_index;
}