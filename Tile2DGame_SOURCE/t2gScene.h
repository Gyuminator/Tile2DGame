#pragma once
#include "t2gEnums.h"
#include "t2gSafePtr.h"
#include "t2gObject.h"
#include "t2gComponent.h"
#include "t2gTypes.h"

using namespace t2g::enums;

//class Object;
//using t2g::Component;

namespace t2g
{
	class Object;
	class Component;
	class Camera;

	class Scene
	{
	public:
		typedef unordered_map<UINT, unique_ptr<Object>> Objects;
		typedef unordered_set<SafePtr<Component>> Components;

	protected:
		Scene();
	public:
		virtual ~Scene() {}

		template<typename T>
		static unique_ptr<T> CreateScene()
		{
			static_assert(std::is_base_of<Scene, T>::value, "Scene::CreateScene: T must derived Scene");
			unique_ptr<T> uptr(new T);
			return std::move(uptr);
		}

	public:
		void Init(SIZE sceneSize);
		void Enter();
		void Update();
		void Render();
		void Exit();

	public:
		SafePtr<t2g::Object> AddObject(eObjectTag tag);
		SafePtr<t2g::Object> AddTile();

		void PushCamera(SafePtr<t2g::Camera> camera);

		void BindComponent(SafePtr<Component> component);

		void PoolingObject(UINT id) {}

		void BindBackEvent(eEventCallPoint callPoint, function<eDelegateResult()> func) { mEvents[callPoint].push_back(func); }
		void BindFrontEvent(eEventCallPoint callPoint, function<eDelegateResult()> func) { mEvents[callPoint].push_front(func); }
		void EventProc(eEventCallPoint callPoint);

		/*void BindObject(unique_ptr<Object> object);
		void UnBindObject(unique_ptr<Object> object);*/

	public:
		const vector<unique_ptr<Object>>& GetTiles() { return mTiles; }
		const vector<SafePtr<Camera>>& GetCameras() { return mCameras; }
		SafePtr<Camera> GetCurCamera() { return mCurCamera; }
		SIZE GetSize() { return mSize; }

	private:
		virtual void init();
		virtual void enter() {}
		virtual void update() {}
		virtual void render() {}
		virtual void exit() {}

	private:
		void LoadImagesOfScene();

	private:
		Objects mObjects;

		Components mUpdateComponentsLayers[(UINT)eUpdateLayer::EnumEnd];
		Components mRenderComponentsLayers[(UINT)eRenderLayer::EnumEnd];

		Events mEvents;

		vector<unique_ptr<Object>> mTiles;
		vector<SafePtr<Camera>> mCameras;

		SafePtr<Camera> mCurCamera;

		SIZE mSize;

	};
}

