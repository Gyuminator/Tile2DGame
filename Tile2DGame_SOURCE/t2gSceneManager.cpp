#include "stdafx.h"
#include "t2gSceneManager.h"

#include "t2gTileMapEditingScene.h"
#include "t2gApplication.h"

t2g::SceneManager::SceneManager()
	: mMapDirectoryPath(L"..\\Resource\\Map\\")
	, mCurScene{}
	, mLoadingScene{}
	, mNextScene{}
	, mScenes{}
{

}

void t2g::SceneManager::Init()
{
	//ChangeScene<PlayScene>(eScene::Play);
	/*AddScene<Scene>(eScene::Play)->Init(SIZE(80, 60));
	ChangeScene<Scene>(eScene::Play);*/
	switch (GET_SINGLETON(Application).GetAppType())
	{
	case eApplicationType::Client:
	{
		AddScene<Scene>(eScene::Start)->Init(L"startMap.tlm");
		ChangeScene<Scene>(eScene::Start);
	}
	break;
	case eApplicationType::Engine:
	{
		AddScene<TileMapEditingScene>(eScene::TileMapEditing)->Init(SIZE(0, 0));
		ChangeScene<TileMapEditingScene>(eScene::TileMapEditing);
	}
	break;
	}
}

void t2g::SceneManager::Update()
{
	mCurScene->Update();
}

void t2g::SceneManager::Render()
{
	mCurScene->Render();
}

void t2g::SceneManager::ExitCurScene()
{
	if (mCurScene.IsEmpty())
		return;

	mCurScene->Exit();
}

void t2g::SceneManager::EnterNextScene(SafePtr<Scene> nextScene)
{
	mNextScene = nextScene;
	mNextScene->Enter();

	mCurScene = mNextScene;
	mNextScene = nullptr;
}
