#pragma once
#include "t2gSprite.h"
#include "t2gSingletonBase.h"
#include "t2gApplication.h"
#include "t2gSafePtr.h"

using std::unordered_map;
using namespace Gdiplus;

namespace t2g
{
	class ImageManager : public SingletonBase<ImageManager>
	{
	public:
		ImageManager();
		virtual ~ImageManager();

	public:
		void Update() override {};
		void Render() override {};

	public:
		void Init();
		void Release();

		void Load(const eImageName eName, const std::wstring fileName, INT xCount, INT yCount);
		void UnLoad(const eImageName eName);

		void DrawImage(Graphics& graphics, SafePtr<Sprite> sprite, const Rect& dest, const Rect& srcRect)
		{
			graphics.DrawImage
			(
				&sprite->GetImage(),
				dest,
				srcRect.X, srcRect.Y,
				srcRect.Width, srcRect.Height,
				UnitPixel
			);
		}
		void DrawImage(Graphics& graphics, SafePtr<Sprite> sprite, const Rect& dest, const Point& srcPos)
		{
			graphics.DrawImage
			(
				&sprite->GetImage(),
				dest,
				srcPos.X * sprite->GetFrameWidth(), srcPos.Y * sprite->GetFrameHeight(),
				sprite->GetFrameWidth(), sprite->GetFrameHeight(),
				UnitPixel
			);
		}
		void DrawImage(Graphics& graphics, eImageName eName, const Rect& dest, const Point& srcPos)
		{
			if (mImages.find(eName) == mImages.end())
				return;

			SafePtr<Sprite> sprite = &mImages[eName];
			DrawImage(graphics, sprite, dest, srcPos);
		}

		/*void DrawImage(SafePtr<Sprite> sprite, Rect& dest, const Point& srcPos)
		{
			GetGraphicsOfBackDC().DrawImage
			(
				(Image*)sprite->GetImage().GetKey(),
				dest,
				srcPos.X * sprite->GetFrameWidth(), srcPos.Y * sprite->GetFrameHeight(),
				sprite->GetFrameWidth(), sprite->GetFrameHeight(),
				UnitPixel
			);
		}
		void DrawTile(SafePtr<Sprite> sprite, Rect& dest, const Point& srcPos)
		{
			GetGraphicsOfTileDC().DrawImage
			(
				(Image*)sprite->GetImage().GetKey(),
				dest,
				srcPos.X * sprite->GetFrameWidth(), srcPos.Y * sprite->GetFrameHeight(),
				sprite->GetFrameWidth(), sprite->GetFrameHeight(),
				UnitPixel
			);
		}
		void DrawTile(eImageName eName, Rect& dest, const Point& srcPos)
		{
			if (mImages.find(eName) == mImages.end())
				return;

			SafePtr<Sprite> sprite = &mImages[eName];
			DrawTile(sprite, dest, srcPos);
		}*/

		SafePtr<Sprite> FindImage(const eImageName eName);

		Graphics& GetGraphicsOfBackDC()
		{
			static Graphics g(GET_SINGLETON(Application).GetBackDC());
			return g;
		}
		Graphics& GetGraphicsOfTileDC()
		{
			static Graphics g(GET_SINGLETON(Application).GetTileDC());
			return g;
		}

	private:
		GdiplusStartupInput mGdiplusStartupInput;

		ULONG_PTR mGdiplusToken;

		const std::wstring mPath;

		unordered_map<eImageName, Sprite> mImages;
	};
}

