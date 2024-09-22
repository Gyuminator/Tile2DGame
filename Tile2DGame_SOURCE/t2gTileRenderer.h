#pragma once
#include "t2gComponent.h"
#include "t2gMath.h"

using namespace t2g::math;

namespace t2g
{
	class TileRenderer : public Component
	{
	public:
		struct TileLayer
		{
			Point SrcPos;
			eImageName ImgName;
		};

	public:
		TileRenderer();
		virtual ~TileRenderer() {}

	public:
		eComponentType GetComponentType() const override { return eComponentType::TileRenderer; }
		void SyncWithOtherComponents() override {};

	public:
		void Init(eImageName eName, INT srcPosX, INT srcPosY, UINT TileIndex);

	public:
		void DrawTileToHDC(HDC hdc, Size sceneSize);

	public:
		UINT GetTileIndex() { return mTileIndex; }
		eImageName GetImageName(INT idx = 0);
		Point GetSrcPos(INT idx = 0);
		INT GetLayerSize() { return (INT)mTileLayers.size(); }

		void SetTileIndex(UINT i) { mTileIndex = i; }
		void SetSrcPos(Point pos, INT idx = 0);
		void SetImageName(eImageName eName, INT idx = 0);

	public:
		eDelegateResult cbDrawTile();

	private:
		void drawLayer(INT idx, INT sceneWidth, HDC targetDC);

	private:
		vector<TileLayer> mTileLayers;

		UINT mTileIndex;

	};
}

