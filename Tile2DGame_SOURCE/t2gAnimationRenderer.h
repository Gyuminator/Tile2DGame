#pragma once
#include "t2gSprite.h"
#include "t2gImageRenderer.h"
#include "t2gMath.h"

using namespace t2g::math;
using std::vector;
using std::unordered_map;
using std::make_pair;
using std::function;

namespace t2g
{
	class Transform;

	class AnimationRenderer : public ImageRenderer
	{
	public:
		typedef vector<Point> Animation;
		typedef function<eAnimState(AnimationRenderer&)> StateChanger;

	public:
		AnimationRenderer();
		virtual ~AnimationRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::AnimationRenderer; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::Script; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::Mid; }
		void SyncWithOtherComponents() override;

	public:
		void Init(eImageName eName, FLOAT duration = 0.3f);
		void AddFrame(eAnimState eState, Point srcPos);
		void BindStateChanger(eAnimState eState, StateChanger func) { mStateChangers[eState] = func; }

	public:
		void SetAnimation(const eAnimState eState, const Animation anim) { mAnimations[eState] = anim; }
		void SetCurState(const eAnimState eState) { mAnimState = eState; }

	public:
		eDelegateResult cbCheckStateValid();
		eDelegateResult cbProcStateChanger();
		eDelegateResult cbAnimate();

	private:
		bool isIndexOver() { return mAnimIndex == mAnimations[mAnimState].size(); }
		void changeAnimState(eAnimState eState);

	private:
		unordered_map<eAnimState, Animation> mAnimations;
		unordered_map<eAnimState, StateChanger> mStateChangers;

		FLOAT mDuration;
		FLOAT mAccTime;

		UINT16 mAnimIndex;

		eAnimState mAnimState;

	public:
		// 상태 전이 함수들
		eAnimState scChangeDirectionByRotation();
	};
}

