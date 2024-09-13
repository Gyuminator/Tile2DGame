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

	auto camera = GetOwner()->GetOwner()->GetCurCamera();
	Rect tempRc;
	Rect tempRenderRc = mRenderRect;
	ScalingRect(tempRenderRc, 1.f / camera->GetDistance());

	if (Rect::Intersect(tempRc, camera->GetCameraViewRect(), tempRenderRc))
	{
		Vector3 camLocation = camera->GetTransform()->GetLocation();
		Rect resultRenderRc = tempRc;
		PointF renderAnchor = GetAnchorByPos(camera->GetCameraViewRect(), Point(tempRc.X, tempRc.Y));
		Point renderPos = GetPosByAnchor(camera->GetViewportRect(), renderAnchor);
		resultRenderRc.X = renderPos.X;
		resultRenderRc.Y = renderPos.Y;
		/*resultRenderRc.X += camera->GetViewportRect().X - camera->GetCameraViewRect().X;
		resultRenderRc.Y += camera->GetViewportRect().Y - camera->GetCameraViewRect().Y;*/
		/*resultRenderRc.X -= camera->GetCameraViewRect().X;
		resultRenderRc.Y -= camera->GetCameraViewRect().Y;*/
		PointF ltAnchor = GetAnchorByPos(tempRenderRc,
			Point(tempRc.GetLeft(), tempRc.GetTop()));
		PointF rbAnchor = GetAnchorByPos(tempRenderRc,
			Point(tempRc.GetRight(), tempRc.GetBottom()));
		Rect resultSrcRc = MakeRectByAnchors(mSrcRect, ltAnchor, rbAnchor);

		Graphics graphics(GET_SINGLETON(Application).GetBackDC());
		/*Graphics graphics(camera->GetCameraDC());*/
		GET_SINGLETON(ImageManager).DrawImage(graphics,
			mSprite, resultRenderRc, resultSrcRc);
	}
	/*RECT rcRECT(900, 500, 1000, 550);
	DrawText(camera->GetCameraDC(), L"12345", 5, &rcRECT, 0);*/
	//}

	/*GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfBackDC(),
		mSprite, mRenderRect, mSrcRect);*/

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


