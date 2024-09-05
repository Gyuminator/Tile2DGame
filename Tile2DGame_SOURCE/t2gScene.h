#pragma once
#include <memory>
#include <unordered_set>
#include <functional>

#include "t2gInterfaces.h"
#include "t2gEnums.h"
#include "t2gLayer.h"
#include "t2gFunc.h"
#include "t2gSafePtr.h"
#include "t2gEvent.h"

using std::unique_ptr;
using std::unordered_set;
using std::function;

using t2g::enums::eUpdateLayer;

class Object;

namespace t2g
{
	using namespace func;

	class Scene
	{
	public:
		typedef unordered_set<unique_ptr<Object>> Objects;
		typedef unordered_set<SafePtr<Component>> Components;
		// typedef unordered_set<Event> Events;

	private:
		Scene();
	public:
		virtual ~Scene() {};

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
		void Init();
		void Exit() {};
		void Enter() {};

	public:
		void AddObject(eObjectType type);
		void BindComponent(SafePtr<Component> component);

	private:
		virtual void init() {}
		virtual void exit() {}
		virtual void enter() {}

	private:
		Components mUpdateComponentsLayers[(UINT)eUpdateLayer::EnumEnd];
		Components mRenderComponentsLayers[(UINT)eRenderLayer::EnumEnd];

		Objects mObjects;

	};
}

