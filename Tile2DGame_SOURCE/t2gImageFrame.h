#pragma once
#include "t2gImageManager.h"
#include "t2gEnums.h"

using namespace t2g::enums;
using namespace t2g::rect;
using t2g::ImageManager;

namespace t2g
{
	class ImageFrame
	{
	public:
		ImageFrame() : mImageName(eImageName::EnumEnd), FrameRect{} {}

	public:
		void SetFrame(INT xPos, INT yPos);

		eImageName GetName() { return mImageName; }
		const Rect& GetFrame() { return FrameRect; }
		INT GetWidth() { return FrameRect.Width; }
		INT GetHeight() { return FrameRect.Height; }

	private:
		eImageName mImageName;

		Rect FrameRect;

	public:
		//인라인
		void Init(eImageName eName, INT xPos, INT yPos)
		{
			mImageName = eName;
			SetFrame(xPos, yPos);
		}
		void SetPos(INT xPos, INT yPos)
		{
			FrameRect.X = FrameRect.Width * xPos;
			FrameRect.Y = FrameRect.Height * yPos;
		}
	};
}

