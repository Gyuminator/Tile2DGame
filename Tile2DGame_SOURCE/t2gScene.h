#pragma once
#include "t2gEnums.h"
#include "t2gSafePtr.h"
#include "t2gObject.h"
#include "t2gComponent.h"

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
		// typedef unordered_set<Event> Events;

	private:
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
		void Update();
		void Render();

	public:
		void Init(SIZE sceneSize);
		void Exit();
		void Enter();

	public:
		SafePtr<t2g::Object> AddObject();
		SafePtr<t2g::Object> AddTile();

		void PushCamera(SafePtr<t2g::Camera> camera);

		void BindComponent(SafePtr<Component> component);

	public:
		const vector<SafePtr<Camera>>& GetCameras() { return mCameras; }
		SafePtr<Camera> GetCurCamera() { return mCurCamera; }
		SIZE GetSize() { return mSize; }

	private:
		virtual void init() {}
		virtual void exit() {}
		virtual void enter() {}

	private:
		void LoadImagesOfScene();

	private:
		Objects mObjects;

		Components mUpdateComponentsLayers[(UINT)eUpdateLayer::EnumEnd];
		Components mRenderComponentsLayers[(UINT)eRenderLayer::EnumEnd];

		vector<unique_ptr<Object>> mTiles;
		vector<SafePtr<Camera>> mCameras;

		SafePtr<Camera> mCurCamera;

		SIZE mSize;

	};
}

