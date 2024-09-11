#include "t2gAnimationRenderer.h"
#include "t2gMacro.h"
#include "t2gImageManager.h"
#include "t2gTime.h"
#include "t2gTransform.h"

t2g::AnimationRenderer::AnimationRenderer()
	: mAnimations{}
	, mDuration{}
	, mAccTime{}
	, mAnimIndex{}
	, mAnimState(eAnimState::EnumEnd)
{
}

void t2g::AnimationRenderer::SyncBindings()
{
	__super::SyncBindings();
}

void t2g::AnimationRenderer::update()
{
	if (eAnimState::EnumEnd == mAnimState)
		return;

	auto iter = mStateChangers.find(mAnimState);
	if (iter != mStateChangers.end())
	{
		changeAnimState(iter->second(*this));
	}

	mAccTime += GET_SINGLETON(Time).GetDT();
	if (mAccTime > mDuration)
	{
		mAccTime = 0.f;
		++mAnimIndex;
		if (isIndexOver())
			mAnimIndex = 0;
		SetSrcPos(mAnimations[mAnimState][mAnimIndex]);
	}
}

void t2g::AnimationRenderer::render()
{
	if (GetTransform().IsEmpty())
		return;

	if (eAnimState::EnumEnd == mAnimState)
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

void t2g::AnimationRenderer::changeAnimState(eAnimState eState)
{
	if (mAnimState == eState)
		return;

	mAnimState = eState;
	mAnimIndex = 0;
	mAccTime = 0.f;
	SetSrcPos(mAnimations[mAnimState][mAnimIndex]);
}

eAnimState t2g::AnimationRenderer::ChangeDirectionByRotation()
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
