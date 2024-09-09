#pragma once
#include <unordered_map>

#include "t2gSprite.h"
#include "t2gSingletonBase.h"
#include "t2gApplication.h"
#include "t2gSafePtr.h"

using std::unordered_map;

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

		void DrawImage(SafePtr<Sprite> sprite, Rect& dest, const Rect& src)
		{
			GetGraphicsOfBackDC().DrawImage
			(
				(Image*)sprite->GetImage().GetKey(),
				dest,
				src.X, src.Y, src.Width, src.Height,
				UnitPixel
			);
		}
		void DrawTile(SafePtr<Sprite> sprite, Rect& dest, const Rect& src)
		{
			GetGraphicsOfTileDC().DrawImage
			(
				(Image*)sprite->GetImage().GetKey(),
				dest,
				src.X, src.Y, src.Width, src.Height,
				UnitPixel
			);
		}
		void DrawTile(eImageName eName, Rect& dest, const Rect& src)
		{
			GetGraphicsOfTileDC().DrawImage
			(
				(Image*)mImages[eName].GetImage().GetKey(),
				dest,
				src.X, src.Y, src.Width, src.Height,
				UnitPixel
			);
		}

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

