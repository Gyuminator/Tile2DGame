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
			Point SrcPos = { 0, 0 };
			eImageName ImgName = eImageName::EnumEnd;
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
		void DrawBlocking(INT sceneWidth, HDC targetDC);

	public:
		UINT GetTileIndex() { return mTileIndex; }
		eImageName GetImageName(INT idx = 0);
		Point GetSrcPos(INT idx = 0);
		INT GetLayerSize() { return (INT)mTileLayers.size(); }
		bool GetIsBlocking() { return mIsBlocking; }

		void SetTileIndex(UINT i) { mTileIndex = i; }
		void SetImageName(eImageName eName, INT idx = 0);
		void SetSrcPos(Point pos, INT idx = 0);
		void SetLayerSize(INT size) { mTileLayers.resize(size); }
		void SetIsBlocking(bool isblocking) { mIsBlocking = isblocking; }

	public:
		eDelegateResult cbDrawTile();
		eDelegateResult cbDrawIndex();

	private:
		void drawLayer(INT idx, INT sceneWidth, HDC targetDC);

	private:
		vector<TileLayer> mTileLayers;

		UINT mTileIndex;
		bool mIsBlocking;
	};
}

