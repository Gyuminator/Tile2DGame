#include "t2gSceneManager.h"
#include "t2gPlayScene.h"

t2g::SceneManager::SceneManager()
	: mCurScene{}
	, mLoadingScene{}
	, mNextScene{}
	, mScenes{}
{
}

void t2g::SceneManager::Init()
{
	//ChangeScene<PlayScene>(eScene::Play);
	AddScene<Scene>(eScene::Play)->Init(SIZE(40, 27));
	ChangeScene<Scene>(eScene::Play);
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
