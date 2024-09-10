#include "t2gTileRenderer.h"
#include "t2gObject.h"
#include "t2gScene.h"

void t2g::TileRenderer::render()
{
	SIZE sceneSize = GetOwner()->GetOwner()->GetSize();
	Rect destRect = {
		INT(mTileIndex % sceneSize.cx * Application::TileSize),
		INT(mTileIndex / sceneSize.cx * Application::TileSize),
		Application::TileSize, Application::TileSize };
	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
		mImageName, destRect, mSrcPos);
}

void t2g::TileRenderer::Init(eImageName eName, INT srcPosX, INT srcPosY, UINT index)
{
	mImageName = eName;
	mSrcPos.X = srcPosX;
	mSrcPos.Y = srcPosY;
	mTileIndex = index;
}
