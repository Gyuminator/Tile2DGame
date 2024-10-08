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
		constexpr static INT NumOfTileToolX = 8;
		constexpr static INT NumOfTileToolY = 24;

		constexpr static INT TilesetMax = 4;
		constexpr static SIZE BasicSceneSIZE = { 19, 12 };

	protected:
		TileMapEditingScene();
	public:
		virtual ~TileMapEditingScene() {}

	private:
		void init() override; // 여기서 메인 카메라와 서브 카메라(칠할 타일 렌더용) 생성
		void update() override;

	public:
		void ChangeMapSize(INT x, INT y);
		void SaveMapController();
		void SaveMap(const wstring& filePath);
		void SaveMapOtherName();
		bool LoadMap(const wstring& filePath) override;
		void LoadMapOtherName();
		void SavePrevEditInfo(const wstring& filePath);
		const wstring LoadPrevEditInfo();

	public:
		const vector<unique_ptr<Object>>& GetCurTileset() { return mToolTilesets[mToolTilesetIdx]; }
		const wstring& GetCurFilePath() { return mCurFilePath; }

		void SetCurFilePath(const wstring& path) { mCurFilePath = path; DrawTextFileName(); }

	protected:
		void CameraMoveController();
		void CameraDistanceController();
		void ChangeTilesetController();
		void ChangeLayerController();
		void ChangeModeController();
		void ChangeBlockingController();

	private:
		void CameraSetting();
		void CreateTileToolBuffer();
		void LoadSprites();
		void CreateToolTileset(eImageName eImgName, UINT8 tilesetIdx);
		void CreateToolTilesets();
		void DrawTileToolBuffer();
		void DrawNearToolTiles(SafePtr<TileRenderer> tile);
		void ClickEventToolTileView(SafePtr<Camera> camera);
		void ClickEventMainTileView(SafePtr<Camera> camera);
		void DrawTextFileName();
		void DrawTextCurrentInfo();
		void CreateMarkerObj();
		void CreateBorderObj();
		void SyncBorderObjSize();
		void ChangeTileset(UINT8 idx);
		void DrawBlocking();
		void DrawIndex();
		void TryLoadPrevEditInfo();

		unique_ptr<Object> CreateToolTileObj();

	private:
		wstring mCurFilePath;
		wstring mMapDirPath;

		UINT8 mCurLayer;
		UINT8 mToolTilesetIdx;
		vector<unique_ptr<Object>> mToolTilesets[TilesetMax];

		HDC mTileToolDC;

		SafePtr<TileRenderer> mSelectedTile;
		SafePtr<Object> mWriteTileMarker;
		SafePtr<Object> mBorderObj;

		SafePtr<Camera> mMainViewCamera;
		SafePtr<Camera> mTileViewCamera;

		eEditMode mEditMode;
		bool mCurBlocking;
	};

	INT_PTR CALLBACK TileMapReSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
}

