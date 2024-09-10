#include "t2gAnimationRenderer.h"
#include "t2gMacro.h"
#include "t2gImageManager.h"
#include "t2gTime.h"

t2g::AnimationRenderer::AnimationRenderer()
	: mAnimations{}
	, mDuration{}
	, mAccTime{}
	, mCurIndex{}
	, mCurState(eAnimState::EnumEnd)
{
}

void t2g::AnimationRenderer::SyncBindings()
{
	__super::SyncBindings();
}

void t2g::AnimationRenderer::update()
{
	if (eAnimState::EnumEnd == mCurState)
		return;

	if (mStateChangers.find(mCurState) != mStateChangers.end())
	{
		
	}

	mAccTime += GET_SINGLETON(Time).GetDT();
	if (mAccTime > mDuration)
	{
		mAccTime = 0.f;
		++mCurIndex;
		if (IsIndexOver())
			mCurIndex = 0;
		SetSrcPos(mAnimations[mCurState][mCurIndex]);
	}
}

void t2g::AnimationRenderer::render()
{
	if (GetTransform().IsEmpty())
		return;

	if (eAnimState::EnumEnd == mCurState)
		return;

	SafePtr<Sprite> sprite = GET_SINGLETON(ImageManager).FindImage(GetImageName());
	if (sprite.IsEmpty())
		return;

	AdjustRenderRect(sprite);

	GET_SINGLETON(ImageManager).DrawImage(
		GET_SINGLETON(ImageManager).GetGraphicsOfBackDC(),
		sprite, GetRenderRect(), GetSrcPos());
}

void t2g::AnimationRenderer::Init(eImageName eName, FLOAT duration)
{
	SetImageName(eName);
	mDuration = duration;
}

void t2g::AnimationRenderer::AddFrame(eAnimState eState, Point srcPos)
{
	if (mAnimations.find(eState) == mAnimations.end())
		mAnimations.insert(make_pair(eState, vector<Point>{}));

	mAnimations[eState].push_back(srcPos);
}
