#pragma once
#include "t2gScene.h"

#include "t2gSafePtr.h"

namespace t2g
{
	class Camera;
	class TileRenderer;

	class TileMapEditingScene : public Scene
	{
		friend unique_ptr<TileMapEditingScene> Scene::CreateScene<TileMapEditingScene>();

	private:
		constexpr static FLOAT TileViewAnchorX = 0.2f;
		constexpr static INT NumOfTileOfToolX = 8;
		constexpr static INT NumOfTileOfToolY = 24;

		constexpr static INT TilesetMax = 4;

	protected:
		TileMapEditingScene();
	public:
		virtual ~TileMapEditingScene() {}

	private:
		void init() override; // 여기서 메인 카메라와 서브 카메라(칠할 타일 렌더용) 생성

	public:
		void ChangeMapSize(INT x, INT y);
		void SaveMapController();
		void SaveMap(const wstring& fileName);
		void SaveMapOtherName();
		void LoadMap(const wstring& fileName);
		void SavePrevEditInfo(const wstring& fileName);
		const wstring LoadPrevEditInfo();

	public:
		const vector<unique_ptr<Object>>& GetCurTileset() { return mToolTilesets[mToolTilesetIdx]; }
		const wstring& GetCurFilePath() { return mCurFilePath; }

	protected:
		void CameraMoveController();
		void CameraDistanceController();
		void ChangeTilesetController();

	private:
		void CameraSetting();
		void CreateTileToolBuffer();
		void LoadSprites();
		void CreateToolTileset(eImageName eImgName, UINT8 tilesetIdx);
		void DrawTileToolBuffer();
		void DrawNearToolTiles(SafePtr<TileRenderer> tile);
		void ClickEventToolTileView(SafePtr<Camera> camera);
		void ClickEventMainTileView(SafePtr<Camera> camera);

		void ChangeTileset(UINT8 idx);

		unique_ptr<Object> CreateToolTileObj();

	private:
		wstring mCurFilePath;
		wstring mMapPath;

		UINT8 mToolTilesetIdx;

		vector<unique_ptr<Object>> mToolTilesets[TilesetMax];

		HDC mTileToolDC;

		SafePtr<TileRenderer> mSelectedTile;
		SafePtr<Object> mWriteTileMarker;

		SafePtr<Camera> mMainViewCamera;
		SafePtr<Camera> mTileViewCamera;

	};
}

