#include "t2gImageRenderer.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gImageManager.h"

t2g::ImageRenderer::ImageRenderer()
	: mImageFrame{}
	, mTransform(nullptr)
	, mRenderRect{}
	, mOffset{}
	, mAnchor{}
{}

void t2g::ImageRenderer::SyncBindings()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::ImageRenderer::render()
{
	if (mTransform.IsEmpty())
		return;

	SafePtr<Sprite> sprite = GET_SINGLETON(ImageManager).FindImage(mImageFrame.GetName());
	if (sprite.IsEmpty())
		return;

	SyncRenderPos(mTransform->GetLocation());

	Vector3 scale = mTransform->GetScale();
	std::rect::ScalingRectbyScale(mRenderRect, mTransform->GetScale());

	std::rect::PositioningRectByAnchor(mRenderRect, mAnchor);

	GET_SINGLETON(ImageManager).DrawImage(sprite, mRenderRect, mImageFrame.GetFrame());
}

void t2g::ImageRenderer::Init(eImageName eName, INT xPos, INT yPos)
{
	mImageFrame.Init(eName, xPos, yPos);
	SyncRenderSize();
}

void t2g::ImageRenderer::SyncRenderSize()
{
	mRenderRect.Width = mImageFrame.GetWidth();
	mRenderRect.Height = mImageFrame.GetHeight();
}

void t2g::ImageRenderer::SyncRenderPos(Vector3 location)
{
	mRenderRect.X = INT(location.x);
	mRenderRect.Y = INT(location.y + location.z);
}


