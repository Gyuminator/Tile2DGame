#include "stdafx.h"
#include "t2gImageManager.h"

#include "t2gApplication.h"

t2g::ImageManager::ImageManager()
	: mGdiplusToken(NULL)
	, mGdiplusStartupInput{}
	, mPath(L"..\\Resource\\Image\\")
	, mImages{}
{
	Init();
}

t2g::ImageManager::~ImageManager()
{
	Release();
}

void t2g::ImageManager::Init()
{
	if (GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL) != S_OK)
		assert(false && "failed GdiplusStartup");


}

void t2g::ImageManager::Release()
{
	//GdiplusShutdown(mGdiplusToken);
}

void t2g::ImageManager::Load(const eImageName eName, const std::wstring fileName, INT xCount, INT yCount)
{
	const auto iter = mImages.find(eName);
	if (iter != mImages.end())
		UnLoad(eName);

	const std::wstring completedPath = mPath + fileName;

	//mImages.emplace(eName, Sprite(completedPath, xCount, yCount));

	mImages.emplace(std::piecewise_construct,
		std::forward_as_tuple(eName),
		std::forward_as_tuple(completedPath, xCount, yCount));
}

void t2g::ImageManager::UnLoad(const eImageName eName)
{
}

t2g::SafePtr<t2g::Sprite> t2g::ImageManager::FindImage(const eImageName eName)
{
	const auto iter = mImages.find(eName);

	if (iter == mImages.end())
		return nullptr;

	return &iter->second;
}
