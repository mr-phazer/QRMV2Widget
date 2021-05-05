#pragma once

#include <DirectXMath.h>

#include "IDrawAbleNode.h"
#include "..\ImporterLib\AnimFIle.h"
#include "..\SystemLib\system_clock.h"

#include "BoneNode.h"
#include "Animation.h"

using namespace DirectX::SimpleMath;

namespace RenderLib
{
	//struct Animation
	//{
	//	/// <summary>
	//	/// Blends two animation
	//	/// the local translation and quaterion in 'this' are set to interpolated value between 'this' and '_poAnm'
	//	/// </summary>
	//	/// <param name="_poAnim">Animation to blend with</param>
	//	/// <param name="weight">interpolation weight. 0.0 = 100% 'this' animations, 1.0 = 100% the '_poAnim' one</param>
	//	void Blend(const Animation* _poAnim, float weight);

	//	// create the bones nodes properpely
	//	void resize(size_t _frames, size_t _bones)
	//	{
	//		vecBoneCurves.resize(_bones);

	//		// TODO: FINISH
	//		/*for (auto& itBone : vecBoneCurves)
	//			itBone*/
	//	}

	//	vector<BoneNode> vecBoneCurves;
	//};

	struct Skeleton : public BoneNode //IDrawableNode
	{
		friend struct BoneNode;

		Skeleton() {};
	public:
		static Skeleton* create(Scene* _poScene, const string _strName);

		virtual void render(Scene* _poScene, Shader::Shader* _poShader = nullptr) override;
		virtual void renderBindPose(Scene* _poScene, Shader::Shader* _poShader = nullptr);
		virtual void renderAnimation(Scene* _poScene, Shader::Shader* _poShader = nullptr);
		virtual void renderBoneLines(Scene* _poScene, Shader::Shader* _poShader = nullptr);

		bool isAnimationsLoaded();
		bool isBindPoseLoaded();

		void setToBindPose(bool _bState);

		virtual vector<Matrix>& updateMatrices();

		virtual vector<Matrix>& calculateInverseBindPoseMatrices();

		virtual void draw(Scene* _poScene, Shader::Shader* _poShader = nullptr) override;

		//XMFLOAT4X4& getGlobalTransFormationMatrix(int _bone_id);
		//vector<Matrix>& getGlobalTransFormationMatrices();

		//void setBones(const vector<BoneNode>& _vecBoneNodes);

		SystemLib::return_value fillSkeletonAndAnimation(Scene* _poScene, const TWAnim::AnimFile* _poBindPose, const TWAnim::AnimFile* _poAnimation);

		SystemLib::return_value fillSkeleton(Scene* _poScene, const TWAnim::AnimFile* _poBindPose);

		//SystemLib::return_value fillAnimation(Scene* _poScene, const TWAnim::AnimFile* _poAnimation);

		void fillAnimation(Scene* _poScene, const TWAnim::AnimFile* _poAnimation);

		const BoneNode& operator[](size_t index) const;
		BoneNode& operator[](size_t index);

	public:
		SystemLib::system_clock& timer() { return m_oClock; };

		uint32_t getIdFromName(const string&) const;

	protected:
		float getFrameRate() const;
		float getLocalTime();

		void updateChildrenBindPose(BoneNode* _poParent);
		void updateChildrenAnimation(BoneNode* _poParent, float _fTime, float _fFrameRate);

	protected:
		void updateLinesBindPose(Scene* _poScene);
		void updateLinesAnimation(Scene* _poScene);

	protected:
		//SkeletonAnimationData oAnimation; // x bones with animation curves
		//SkeletonAnimationData oBindPose; // x bones with animation curves

		shared_ptr<SubMeshData> m_spoBoneLines; // the lines that indicate the actual bones

		vector<Matrix> m_vecCurentBoneMatrices;
		vector<Matrix> m_vecInvBindPoseMatrices;

		vector<BoneNode> m_vecBoneCurves;
		vector<BoneNode> m_vecBonesBindPose;

		float m_fFrameRate = 5.0;
		float m_fAnimLen = 0.0;

	public:
		Animation oAnimation;

	private:
		bool m_bShowSkeleton = true;
		bool m_bInBindPose = false;
		BoneNode m_oEmptyNode;
		Skeleton* m_poBindPose;
		vector<DirectX::XMMATRIX> m_vecInverBindePoseMatricesXM;

		SystemLib::system_clock m_oClock;
	};
};
