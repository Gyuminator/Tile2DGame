#include "stdafx.h"
#include "t2gImageRenderer.h"

#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gImageManager.h"
#include "t2gScene.h"
#include "t2gCamera.h"

t2g::ImageRenderer::ImageRenderer()
	: mRenderRect{}
	, mSrcRect{}
	, mAnchor{}
	, mOffset{}
	, mTransform(nullptr)
	, mSprite(nullptr)
	, mImageName(eImageName::EnumEnd)
{}

void t2g::ImageRenderer::SyncWithOtherComponents()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::ImageRenderer::Init(eImageName eName, INT xPos, INT yPos)
{
	mSrcRect.X = xPos;
	mSrcRect.Y = yPos;

	mImageName = eName;

	BindToRenders(&ImageRenderer::cbCheckTransform);
	BindToRenders(&ImageRenderer::cbCheckImageLoading, true);
	BindToRenders(&ImageRenderer::cbDrawImage);
}

void t2g::ImageRenderer::AdjustRenderRect()
{
	SyncRenderPos(mTransform->GetLocation());
	SyncRenderSize();

	std::rect::ScalingRectbyScale(mRenderRect, mTransform->GetScale());
	std::rect::PositioningRectByAnchor(mRenderRect, mAnchor);
}

eDelegateResult t2g::ImageRenderer::cbCheckTransform()
{
	if (mTransform.IsEmpty())
		return eDelegateResult::Return;
	else
		return eDelegateResult::Erase;
}

eDelegateResult t2g::ImageRenderer::cbCheckImageLoading()
{
	mSprite = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (mSprite.IsEmpty())
		return eDelegateResult::Return;

	mSrcRect.Width = mSprite->GetFrameWidth();
	mSrcRect.Height = mSprite->GetFrameHeight();

	return eDelegateResult::Erase;
}

eDelegateResult t2g::ImageRenderer::cbDrawImage()
{
	AdjustRenderRect();

	auto& cameras = GetOwner()->GetOwner()->GetCameras();
	Rect tempRc;
	for (auto camera : cameras)
	{
		if (Rect::Intersect(tempRc, camera->GetCameraViewRect(), GetRenderRect()))
		{

			GET_SINGLETON(ImageManager).DrawImage(camera->GetGraphics(),
				mSprite, mRenderRect, mSrcRect);
		}
	}

	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfBackDC(),
		mSprite, mRenderRect, mSrcRect);

	return eDelegateResult::OK;
}

void t2g::ImageRenderer::SyncRenderSize()
{
	mRenderRect.Width = mSprite->GetFrameWidth();
	mRenderRect.Height = mSprite->GetFrameHeight();
}

void t2g::ImageRenderer::SyncRenderPos(Vector3 location)
{
	mRenderRect.X = INT(location.x);
	mRenderRect.Y = INT(location.y + location.z);
}


