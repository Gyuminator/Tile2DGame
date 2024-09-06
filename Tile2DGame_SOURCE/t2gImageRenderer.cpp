#include "t2gImageRenderer.h"
#include "t2gObject.h"
#include "t2gTransform.h"

t2g::ImageRenderer::ImageRenderer()
	: mImageName(eImageName::EnumEnd)
	, mTransform(nullptr)
	, mFrameRect{}
	, mOffset{}
{}

void t2g::ImageRenderer::SyncBindings()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::ImageRenderer::render()
{
	if (mTransform.IsEmpty())
		return;

	Image* image = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (image == nullptr)
		return;

	Vector3 location = mTransform->GetLocation();
	Vector3 rotation = mTransform->GetRotation();
	Vector3 scale = mTransform->GetScale();

	Rect dest{ INT(location.x), INT(location.y), mFrameRect.Width, mFrameRect.Height };


	std::rect::ScalingRect(dest, { 1.5f, 1.5f });
	std::rect::ModifyRectByAnchor(dest, { 0.5f, 0.5f });

	GET_SINGLETON(ImageManager).DrawImage(image, dest, mFrameRect);
}

void t2g::ImageRenderer::Init(eImageName eName)
{
	SetImageName(eName);
}

void t2g::ImageRenderer::SetFrame(INT xCount, INT yCount, INT xPos, INT yPos)
{
	Image* image = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (image == nullptr)
		return;

	mFrameRect.X = xPos;
	mFrameRect.Y = xPos;
	mFrameRect.Width = image->GetWidth() / xCount;
	mFrameRect.Height = image->GetHeight() / yCount;
}
