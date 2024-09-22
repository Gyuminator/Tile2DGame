#include "stdafx.h"
#include "t2gTileRenderer.h"

#include "t2gObject.h"
#include "t2gScene.h"
#include "t2gApplication.h"
#include "t2gImageManager.h"
#include "t2gFunc.h"

//void t2g::TileRenderer::render()
//{
//	SIZE sceneSize = GetOwnerObj()->GetOwnerObj()->GetSize();
//	Rect destRect = {
//		INT(mTileIndex % sceneSize.cx * Application::TileSize),
//		INT(mTileIndex / sceneSize.cx * Application::TileSize),
//		Application::TileSize, Application::TileSize };
//	Point srcPoint = { mSrcPos.X, mSrcPos.Y };
//	GET_SINGLETON(ImageManager).DrawImage(GET_SINGLETON(ImageManager).GetGraphicsOfTileDC(),
//		mImageName, destRect, srcPoint);
//}

t2g::TileRenderer::TileRenderer()
	: mTileLayers{}
	, mTileIndex(0)
{
	mTileLayers.push_back({ {0, 0}, eImageName::EnumEnd });
}

void t2g::TileRenderer::Init(eImageName eName, INT srcPosX, INT srcPosY, UINT index)
{
	ClearMDDs();

	SetUpdateLayer(eUpdateLayer::EnumEnd);
	SetRenderLayer(eRenderLayer::EnumEnd);

	mTileLayers[0].ImgName = eName;
	mTileLayers[0].SrcPos.X = srcPosX;
	mTileLayers[0].SrcPos.Y = srcPosY;
	mTileIndex = index;

	BindBackToRenders(&TileRenderer::cbDrawTile);
}

void t2g::TileRenderer::DrawTileToHDC(HDC hdc, Size sceneSize)
{
	Rect destRect = func::GetTileRectByIndex(sceneSize.Width, mTileIndex);
	BitBlt(hdc, destRect.X, destRect.Y, destRect.Width, destRect.Height,
		func::GetBlackTilePieceDC(), 0, 0, SRCCOPY);
	/*Graphics graphics(hdc);
	GET_SINGLETON(ImageManager).DrawImage(graphics,
		mTileLayers[0].ImgName, destRect, mTileLayers[0].SrcPos);*/
	for (INT i = 0; i < mTileLayers.size(); ++i)
	{
		drawLayer(i, sceneSize.Width, hdc);
	}
}

eImageName t2g::TileRenderer::GetImageName(INT idx)
{
	assert(idx < mTileLayers.size() && "TileRenderer::GetImageName: idx >= mTileLayers.size()");

	return mTileLayers[idx].ImgName;
}

Point t2g::TileRenderer::GetSrcPos(INT idx)
{
	assert(idx < mTileLayers.size() && "TileRenderer::GetImageName: idx >= mTileLayers.size()");

	return mTileLayers[idx].SrcPos;
}

void t2g::TileRenderer::SetSrcPos(Point pos, INT idx)
{
	if (idx >= mTileLayers.size())
		mTileLayers.resize(idx + 1);

	mTileLayers[idx].SrcPos = pos;
}

void t2g::TileRenderer::SetImageName(eImageName eName, INT idx)
{
	if (idx >= mTileLayers.size())
		mTileLayers.resize(idx + 1);

	mTileLayers[idx].ImgName = eName;
}

eDelegateResult t2g::TileRenderer::cbDrawTile()
{
	/*SIZE sceneSize = GetOwnerObj()->GetOwnerScene()->GetSize();
	Rect destRect = {
		INT(mTileIndex % sceneSize.cx * Application::TileSize),
		INT(mTileIndex / sceneSize.cx * Application::TileSize),
		Application::TileSize, Application::TileSize };
	Graphics g(func::GetTileDC());
	GET_SINGLETON(ImageManager).DrawImage(g,
		mTileLayers[0].ImgName, destRect, mTileLayers[0].SrcPos);*/

	SIZE sceneSize = GetOwnerObj()->GetOwnerScene()->GetSize();
	for (INT i = 0; i < mTileLayers.size(); ++i)
	{
		drawLayer(i, sceneSize.cx, func::GetTileDC());
	}

	return eDelegateResult::OK;
}

void t2g::TileRenderer::drawLayer(INT idx, INT sceneWidth, HDC targetDC)
{
	Rect destRect = {
		INT(mTileIndex % sceneWidth * Application::TileSize),
		INT(mTileIndex / sceneWidth * Application::TileSize),
		Application::TileSize, Application::TileSize };
	Graphics g(targetDC);
	GET_SINGLETON(ImageManager).DrawImage(g,
		mTileLayers[idx].ImgName, destRect, mTileLayers[idx].SrcPos);
}

