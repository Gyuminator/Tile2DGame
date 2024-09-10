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
		typedef function<void(AnimationRenderer&)> StateChanger;

	public:
		AnimationRenderer();
		virtual ~AnimationRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::AnimationRenderer; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::Script; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::Middle; }
		void SyncBindings() override;

	private:
		void update() override;
		void render() override;

	public:
		void Init(eImageName eName, FLOAT duration = 0.5f);
		void AddFrame(eAnimState eState, Point srcPos);

	public:
		void SetAnimation(const eAnimState eState, const Animation anim) { mAnimations[eState] = anim; }
		void SetCurState(const eAnimState eState) { mCurState = eState; }

	private:
		bool IsIndexOver() { return mCurIndex == mAnimations[mCurState].size(); }

	private:
		unordered_map<eAnimState, Animation> mAnimations;
		unordered_map<eAnimState, StateChanger> mStateChangers;

		FLOAT mDuration;
		FLOAT mAccTime;

		UINT16 mCurIndex;

		eAnimState mCurState;

	};
}

