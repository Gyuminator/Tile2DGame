#pragma once
#include "t2gComponent.h"
#include "t2gMath.h"

using namespace t2g::math;

namespace t2g
{
	class TileRenderer : public Component
	{
	public:
		TileRenderer() : mSrcPos{}, mTileIndex(0), mImageName(eImageName::EnumEnd) {}
		virtual ~TileRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::TileRenderer; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncWithOtherComponents() override {};

	public:
		void Init(eImageName eName, INT srcPosX, INT srcPosY, UINT TileIndex);

	public:
		void DrawTileToHDC(HDC hdc, Size sceneSize);

	public:
		Point GetSrcPos() { return mSrcPos; }
		UINT GetTileIndex() { return mTileIndex; }
		eImageName GetImageName() { return mImageName; }

		void SetTileIndex(UINT i) { mTileIndex = i; }
		void SetSrcPos(Point pos) { mSrcPos = pos; }

	public:
		eDelegateResult cbDrawTile();

	private:
		Point mSrcPos;

		UINT mTileIndex;
		eImageName mImageName;

	};
}

