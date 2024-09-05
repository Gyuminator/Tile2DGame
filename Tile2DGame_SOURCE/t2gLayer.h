#pragma once
#include <unordered_set>

#include "t2gInterfaces.h"
#include "t2gSafePtr.h"
#include "t2gObject.h"

using std::unordered_set;
using t2g::SafePtr;


namespace t2g
{
	class Layer : public IGameLoop
	{
		typedef unordered_set<SafePtr<Object>> Objects;

	public:
		void Update() override {}
		void Render() override {}

	private:
		Objects mObjects;

	};
}

