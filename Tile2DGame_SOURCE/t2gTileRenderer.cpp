#include "stdafx.h"
#include "t2gTileRenderer.h"

#include "t2gObject.h"
#include "t2gScene.h"

//void t2g::TileRenderer::render()
//{
//	SIZE sceneSize = GetOwner()->GetOwner()->GetSize();
//	Rect destRect = {
//		INT(mTileIndex % sceneSize.cx * Application::TileSize),
//		INT(mTileIndex / sceneSize.cx * Application::TileSize),
//		Application::TileSize, Application::TileSize };
//	Point srcPoint = { mSrcRect.X, mSrcRect.Y };
//	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
//		mImageName, destRect, srcPoint);
//}

void t2g::TileRenderer::Init(eImageName eName, INT srcPosX, INT srcPosY, UINT index)
{
	mImageName = eName;
	mSrcRect.X = srcPosX;
	mSrcRect.Y = srcPosY;
	mTileIndex = index;
	BindToRenders(&TileRenderer::cbDrawTile);
}

eDelegateResult t2g::TileRenderer::cbDrawTile()
{
	SIZE sceneSize = GetOwner()->GetOwner()->GetSize();
	Rect destRect = {
		INT(mTileIndex % sceneSize.cx * Application::TileSize),
		INT(mTileIndex / sceneSize.cx * Application::TileSize),
		Application::TileSize, Application::TileSize };
	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
		mImageName, destRect, mSrcRect);

	return eDelegateResult::OK;
}
