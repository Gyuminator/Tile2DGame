#include "t2gImageFrame.h"

void t2g::ImageFrame::SetFrame(INT xPos, INT yPos)
{
	SafePtr<t2g::Sprite> sprite = GET_SINGLETON(ImageManager).FindImage(mImageName);
	if (sprite.IsEmpty())
		return;

	FrameRect.Width = sprite->GetFrameWidth();
	FrameRect.Height = sprite->GetFrameHeight();

	SetPos(xPos, yPos);
}
