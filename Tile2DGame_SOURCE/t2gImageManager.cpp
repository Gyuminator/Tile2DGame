#include "t2gImageManager.h"
#include "t2gApplication.h"

t2g::ImageManager::ImageManager()
	: mGdiplusToken(NULL)
	, mGdiplusStartupInput{}
	, mPath(L"..\\Resource\\Image\\")
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

void t2g::ImageManager::Load(const eImageName eName, const std::wstring fileName)
{
	const auto iter = mImages.find(eName);
	if (iter != mImages.end())
		UnLoad(eName);

	std::wstring completedPath = mPath + fileName;

	mImages.emplace(eName, completedPath.c_str());
}

void t2g::ImageManager::UnLoad(const eImageName eName)
{
}

Image* t2g::ImageManager::FindImage(const eImageName eName)
{
	const auto iter = mImages.find(eName);

	if (iter == mImages.end())
		return nullptr;

	return &iter->second;
}
