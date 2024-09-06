#pragma once
#include <unordered_map>

#include "t2gRect.h"
#include "t2gSingletonBase.h"
#include "t2gApplication.h"

using namespace Gdiplus;

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

		void Load(const eImageName eName, const std::wstring fileName);
		void UnLoad(const eImageName eName);

		void DrawImage(Image* img, Rect& dest, Rect& src)
		{
			GetGraphics().DrawImage
			(
				img,
				dest,
				src.X, src.Y, src.Width, src.Height,
				UnitPixel
			);
		}

		Image* FindImage(const eImageName eName);

		Graphics& GetGraphics()
		{
			static Graphics g(GET_SINGLETON(Application).GetBackDC());
			return g;
		}

	private:
		GdiplusStartupInput mGdiplusStartupInput;

		ULONG_PTR mGdiplusToken;

		const std::wstring mPath;

		unordered_map<eImageName, Image> mImages;
	};
}

