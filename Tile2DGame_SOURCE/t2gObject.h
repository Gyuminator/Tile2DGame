#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "t2gInterfaces.h"
#include "t2gEnums.h"
#include "t2gSafePtr.h"

using std::wstring;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

using t2g::enums::eComponentType;
using t2g::enums::eLayerTag;
using t2g::SafePtr;

class Component;

namespace t2g
{
	class Object : public IGameLoop, public INamed
	{
	public:
		typedef unordered_map<eComponentType, unique_ptr<IComponent>> Components;

	public:
		Object();
		~Object();

	public:
		void Init() override;
		void Update() override;
		void Render() override;
		void Release() override;

	public:
		const wstring& GetName() override { return mName; }
		void SetName(const wstring& name) { mName = name; }

	private:
		wstring mName;

		eLayerTag mLayerTag;
		
		Components mComponents;
		vector<eComponentType> mComponentKeys;

		SafePtr<Object> mAttacher;
		vector<SafePtr<Object>> mAttachedObjects;
	};
}

