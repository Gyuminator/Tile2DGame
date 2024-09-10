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
		/*ImageFrame() : FrameRect{} {}*/
		ImageFrame() : mPos{} {}

	public:
		// eImageName GetName() { return mImageName; }
		//const Rect& GetFrame() { return FrameRect; }
		//INT GetWidth() { return FrameRect.Width; }
		//INT GetHeight() { return FrameRect.Height; }

		//void SetFrame(INT srcPosX, INT srcPosY, INT frameWidth, INT frameHeight);

	private:
		//eImageName mImageName;
		Point mPos;
		//Rect FrameRect;

	public:
		//인라인
		/*void Init(eImageName eName, INT srcPosX, INT srcPosY)
		{
			mImageName = eName;
			SetFrame(srcPosX, srcPosY);
		}*/
		void Init(/*eImageName eName, */INT srcPosX, INT srcPosY, INT srcWidth, INT srcHeight)
		{
			// mImageName = eName;
			/*FrameRect.Width = srcWidth;
			FrameRect.Height = srcHeight;*/
			SetPos(srcPosX, srcPosY);
		}
		void SetPos(INT xPos, INT yPos)
		{
			/*FrameRect.X = FrameRect.Width * xPos;
			FrameRect.Y = FrameRect.Height * yPos;*/
			mPos.X = xPos;
			mPos.Y = yPos;
		}
	};
}

