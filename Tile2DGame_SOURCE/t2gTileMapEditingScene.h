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

	protected:
		TileMapEditingScene();
	public:
		virtual ~TileMapEditingScene() {}

	private:
		void init() override; // 여기서 메인 카메라와 서브 카메라(칠할 타일 렌더용) 생성


	public:
		void ChangeMapSize(INT x, INT y);
		void SaveMap(const wstring& fileName);
		void LoadMap(const wstring& fileName);
		void SavePrevEditInfo(const wstring& fileName);
		const wstring LoadPrevEditInfo();

	public:
		const vector<unique_ptr<Object>>& GetToolTiles() { return mToolTiles; }

	protected:
		void cbSceneController(); // 편집 씬 조작
		void cbCheckCameras();

	private:
		void CameraSetting();
		void CreateTileToolBuffer();
		void LoadSprites();
		void CreateToolTiles();
		void DrawTileToolBuffer();
		void DrawNearToolTiles(SafePtr<TileRenderer> tile);
		void ClickEventToolTileView(SafePtr<Camera> camera);
		void ClickEventMainTileView(SafePtr<Camera> camera);

		SafePtr<t2g::Object> AddToolTile();

	private:
		wstring mCurFileName;
		wstring mMapPath;

		vector<unique_ptr<Object>> mToolTiles;

		HDC mTileToolDC;

		SafePtr<TileRenderer> mSelectedTile;
		SafePtr<Object> mWriteTileMarker;

		SafePtr<Camera> mMainViewCamera;
		SafePtr<Camera> mTileViewCamera;

	};
}

