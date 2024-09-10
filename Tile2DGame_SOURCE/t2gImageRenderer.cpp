#include "t2gImageRenderer.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gImageManager.h"

t2g::ImageRenderer::ImageRenderer()
	: mRenderRect{}
	, mSrcPos{}
	, mAnchor{}
	, mOffset{}
	, mImageName(eImageName::EnumEnd)
	, mTransform(nullptr)
{}

void t2g::ImageRenderer::SyncBindings()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::ImageRenderer::render()
{
	if (mTransform.IsEmpty())
		return;

	SafePtr<Sprite> sprite = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (sprite.IsEmpty())
		return;

	AdjustRenderRect(sprite);

	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfBackDC(),
		sprite, mRenderRect, mSrcPos);
}

void t2g::ImageRenderer::Init(eImageName eName, INT xPos, INT yPos)
{
	mSrcPos.X = xPos;
	mSrcPos.Y = yPos;

	mImageName = eName;
	SafePtr<Sprite> sprite = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (sprite.IsEmpty())
		return;

	//mImageFrame.Init(xPos, yPos, sprite->GetFrameWidth(), sprite->GetFrameHeight());
}

void t2g::ImageRenderer::AdjustRenderRect(SafePtr<Sprite> sprite)
{
	SyncRenderPos(mTransform->GetLocation());
	SyncRenderSize(sprite);

	std::rect::ScalingRectbyScale(mRenderRect, mTransform->GetScale());
	std::rect::PositioningRectByAnchor(mRenderRect, mAnchor);
}

void t2g::ImageRenderer::SyncRenderSize(SafePtr<Sprite> sprite)
{
	mRenderRect.Width = sprite->GetFrameWidth();
	mRenderRect.Height = sprite->GetFrameHeight();
}

void t2g::ImageRenderer::SyncRenderPos(Vector3 location)
{
	mRenderRect.X = INT(location.x);
	mRenderRect.Y = INT(location.y + location.z);
}


