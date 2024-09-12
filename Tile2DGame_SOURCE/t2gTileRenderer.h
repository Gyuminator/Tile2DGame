#pragma once
#include "t2gImageFrame.h"
#include "t2gComponent.h"
#include "t2gMath.h"

using namespace t2g::math;

namespace t2g
{
	class TileRenderer : public Component
	{
	public:
		TileRenderer() : mTileIndex(0), mImageName(eImageName::EnumEnd), mSrcRect{} {}
		virtual ~TileRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::TileRenderer; }
		eUpdateLayer GetUpdateLayer() const override { return eUpdateLayer::EnumEnd; }
		eRenderLayer GetRenderLayer() const override { return eRenderLayer::EnumEnd; }
		void SyncWithOtherComponents() override {};

	public:
		void Init(eImageName eName, INT srcPosX, INT srcPosY, UINT TileIndex);

	public:
		void SetTileIndex(UINT i) { mTileIndex = i; }

	protected:
		eDelegateResult cbDrawTile();

	private:
		UINT mTileIndex;
		eImageName mImageName;

		Point mSrcRect;
	};
}

