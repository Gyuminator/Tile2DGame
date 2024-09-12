#pragma once

#include "t2gRect.h"
#include "t2gSafePtr.h"

using std::wstring;

namespace t2g
{
	using namespace Gdiplus;

	class Sprite
	{
	public:
		Sprite() : mImage(L""), mFrameSize{} {}
		Sprite(const wstring& path, INT xCount, INT yCount)
			: mImage(path.c_str())
			, mFrameSize{ INT(mImage.GetWidth() / xCount) , INT(mImage.GetHeight() / yCount) }
		{}

	public:
		//SafePtr<Image> GetImage() { return &mImage; }
		Image& GetImage() { return mImage; }
		INT GetFrameWidth() { return mFrameSize.Width; }
		INT GetFrameHeight() { return mFrameSize.Height; }

	private:
		Image mImage;
		Size mFrameSize;
	};
}

