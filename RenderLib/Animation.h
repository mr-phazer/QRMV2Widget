#pragma once
#include "IAbstractObject.h"

#include "BoneAnimCurve.h"

namespace RenderLib
{

    class Animation :
        public IAbstractObject
    {
    public:
        bool fillSkeleton(Scene* _poScene, const TWAnim::AnimFile* _poBindPose);        
        void fillAnimation(Scene* _poScene, const TWAnim::AnimFile* _poAnimation);

        void UpdateGlobalsDiscrete(vector<BoneAnimCurve>& vecIn, size_t _frame);
        
        void UpdateGlobalsInterPolated(vector<BoneAnimCurve>& vecIn);
        
        
        void update();


		void calculateInverseBindPoseMatrices();
		


        







        vector<bool> vecMaskStaticFrame;
		vector<BoneAnimCurve> vecStaticFrame;
        
		vector<BoneAnimCurve> vecBindPose;
        vector<BoneAnimCurve> vecBoneAnimCurves;
        vector<BoneAnimCurve> vecBoneAnimCurvesWithStaticFrame;


        vector<SimpleMath::Matrix> vecInverseBindPoseMatrices;

		
    //
    //public:
    //    void setFrameRate(float _rate) {
    //        m_fFrameRate = _rate;
    //    }

    //    float getFrameRate() {
    //        return m_fFrameRate;
    //    }


    //pårivate
        float m_fFrameRate = 20.f;
		float m_fAnimLenght = -1;
		SystemLib::system_clock m_oClock;
    };



}
