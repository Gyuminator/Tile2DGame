#include "stdafx.h"
#include "t2gTileRenderer.h"

#include "t2gObject.h"
#include "t2gScene.h"
#include "t2gApplication.h"
#include "t2gImageManager.h"
#include "t2gFunc.h"

//void t2g::TileRenderer::render()
//{
//	SIZE sceneSize = GetOwner()->GetOwner()->GetSize();
//	Rect destRect = {
//		INT(mTileIndex % sceneSize.cx * Application::TileSize),
//		INT(mTileIndex / sceneSize.cx * Application::TileSize),
//		Application::TileSize, Application::TileSize };
//	Point srcPoint = { mSrcPos.X, mSrcPos.Y };
//	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
//		mImageName, destRect, srcPoint);
//}

void t2g::TileRenderer::Init(eImageName eName, INT srcPosX, INT srcPosY, UINT index)
{
	mImageName = eName;
	mSrcPos.X = srcPosX;
	mSrcPos.Y = srcPosY;
	mTileIndex = index;
	BindBackToRenders(&TileRenderer::cbDrawTile);
}

void t2g::TileRenderer::DrawTileToHDC(HDC hdc, Size sceneSize)
{
	Rect destRect = func::GetTileRectByIndex(sceneSize.Width, mTileIndex);
	BitBlt(hdc, destRect.X, destRect.Y, destRect.Width, destRect.Height,
		func::GetBlackTilePieceDC(), 0, 0, SRCCOPY);
	Graphics graphics(hdc);
	GET_SINGLETON(ImageManager).DrawImage(graphics,
		mImageName, destRect, mSrcPos);
}

eDelegateResult t2g::TileRenderer::cbDrawTile()
{
	SIZE sceneSize = GetOwner()->GetOwner()->GetSize();
	Rect destRect = {
		INT(mTileIndex % sceneSize.cx * Application::TileSize),
		INT(mTileIndex / sceneSize.cx * Application::TileSize),
		Application::TileSize, Application::TileSize };
	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
		mImageName, destRect, mSrcPos);

	return eDelegateResult::OK;
}

