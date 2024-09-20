#include "stdafx.h"
#include "t2gAnimationRenderer.h"

#include "t2gMacro.h"
#include "t2gImageManager.h"
#include "t2gTime.h"
#include "t2gTransform.h"
#include "t2gObject.h"
#include "t2gScene.h"
#include "t2gCamera.h"

t2g::AnimationRenderer::AnimationRenderer()
	: mAnimations{}
	, mDuration{}
	, mAccTime{}
	, mAnimIndex{}
	, mAnimState(eAnimState::EnumEnd)
{
}

void t2g::AnimationRenderer::SyncWithOtherComponents()
{
	__super::SyncWithOtherComponents();
}

void t2g::AnimationRenderer::Init(eImageName eName, FLOAT duration)
{
	SetUpdateLayer(eUpdateLayer::Script);
	SetRenderLayer(eRenderLayer::Mid);

	SetImageName(eName);
	mDuration = duration;

	BindBackToUpdates(&AnimationRenderer::cbCheckImageLoading);
	BindBackToUpdates(&AnimationRenderer::cbCheckTransform);
	BindBackToUpdates(&AnimationRenderer::cbCheckStateValid);

	BindBackToUpdates(&AnimationRenderer::cbProcStateChanger);
	BindBackToUpdates(&AnimationRenderer::cbAnimate);

	BindBackToRenders(&AnimationRenderer::cbCheckImageLoading);
	BindBackToRenders(&AnimationRenderer::cbCheckTransform);
	BindBackToRenders(&AnimationRenderer::cbCheckStateValid);

	DrawBindByObjTag(eRenderLayer::Mid);
}

void t2g::AnimationRenderer::AddFrame(eAnimState eState, Point srcPos)
{
	if (mAnimations.find(eState) == mAnimations.end())
		mAnimations.insert(make_pair(eState, vector<Point>{}));

	mAnimations[eState].push_back(srcPos);
}

eDelegateResult t2g::AnimationRenderer::cbCheckStateValid()
{
	if (mAnimState == eAnimState::EnumEnd)
		return eDelegateResult::Return;
	if (mAnimations.find(mAnimState) == mAnimations.end())
		return eDelegateResult::Return;

	return eDelegateResult::Erase;
}

eDelegateResult t2g::AnimationRenderer::cbProcStateChanger()
{
	auto iter = mStateChangers.find(mAnimState);
	if (iter != mStateChangers.end())
		changeAnimState(iter->second(*this));

	return eDelegateResult::OK;
}

eDelegateResult t2g::AnimationRenderer::cbAnimate()
{
	mAccTime += GET_SINGLETON(Time).GetDT();
	if (mAccTime > mDuration)
	{
		mAccTime = 0.f;
		++mAnimIndex;
		if (isIndexOver())
			mAnimIndex = 0;
		SetSrcPos(mAnimations[mAnimState][mAnimIndex]);
	}

	return eDelegateResult::OK;
}

void t2g::AnimationRenderer::changeAnimState(eAnimState eState)
{
	if (mAnimState == eState)
		return;

	mAnimState = eState;
	mAnimIndex = 0;
	mAccTime = 0.f;
	SetSrcPos(mAnimations[mAnimState][mAnimIndex]);
}

eAnimState t2g::AnimationRenderer::scChangeDirectionByRotation()
{
	SafePtr<Transform> transform = GetTransform();
	if (transform.IsEmpty())
		return mAnimState;

	const Vector3 rotation = transform->GetRotation();
	UINT zABS = abs((INT)rotation.z);
	UINT16 factor = zABS % 360 / 90;
	factor += (UINT16)eAnimState::Idle_Right;
	return (eAnimState)factor;
}
