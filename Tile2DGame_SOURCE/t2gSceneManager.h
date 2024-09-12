#pragma once
#include "t2gSingletonBase.h"
#include "t2gEnums.h"
#include "t2gSafePtr.h"
#include "t2gFunc.h"
#include "t2gScene.h"

using std::unordered_map;
using std::make_pair;
using std::unique_ptr;

using t2g::SafePtr;

using namespace t2g::func;

class Scene;

namespace t2g
{
	using namespace enums;

	class SceneManager : public SingletonBase<SceneManager>
	{
	public:
		typedef unordered_map<eScene, unique_ptr<Scene>> Scenes;

	public:
		SceneManager();
		virtual ~SceneManager() {};

	public:
		void Update() override;
		void Render() override;
	public:
		void Init();

	public:
		template<typename T>
		SafePtr<Scene> AddScene(const eScene eName)
		{
			mScenes.emplace(eName, Scene::CreateScene<T>());
			return mScenes.find(eName)->second.get();
		};
		template<typename T>
		void ChangeScene(const eScene eName)
		{
			// 씬 찾기
			auto iter = mScenes.find(eName);
			if (iter == mScenes.end())
				return;

			ExitCurScene();
			EnterNextScene(iter->second.get());
		}


	public:
		SafePtr<Scene> GetCurScene() { return mCurScene; }

	private:
		void ExitCurScene();
		void EnterNextScene(SafePtr<Scene> nextScene);

	private:
		SafePtr<Scene> mCurScene;
		SafePtr<Scene> mLoadingScene;
		SafePtr<Scene> mNextScene;

		Scenes mScenes;

	};
}
