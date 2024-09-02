#pragma once
#include <Windows.h>
#include "t2gSingletonBase.h"
#include "t2gMacro.h"
#include <string>

namespace t2g
{
	class Time : public t2g::SingletonBase<Time>
	{
	public:
		Time();
		virtual ~Time() {};
	public:
		virtual void Init();
		virtual void Update();
		virtual void Render();

	public:
		float GetDT() { return mDeltaTime; }

	private:
		LARGE_INTEGER mFrequencyPerSecond;
		LARGE_INTEGER mCurCount;
		LARGE_INTEGER mPrevCount;
		float mDeltaTime;

		std::wstring mTextToDraw;
		RECT mRectToDraw;
	};
}

