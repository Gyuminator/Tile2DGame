#pragma once
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <vector>

#include "t2gInterfaces.h"
#include "t2gEnums.h"
#include "t2gFunc.h"
#include "t2gSafePtr.h"
#include "t2gEvent.h"
#include "t2gObject.h"
#include "t2gComponent.h"

using std::unique_ptr;
using std::unordered_set;
using std::unordered_map;
using std::function;
using std::vector;

using namespace t2g::enums;

//class Object;
//using t2g::Component;

namespace t2g
{
	using namespace func;

	class Object;
	class Component;

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
		SafePtr<t2g::Object> AddCamera();

		void BindComponent(SafePtr<Component> component);

	public:
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
		vector<unique_ptr<Object>> mCameras;

		SIZE mSize;

	};
}

