#include "t2gTime.h"
#include "t2gApplication.h"

t2g::Time::Time()
	: mFrequencyPerSecond{}
	, mCurCount{}
	, mPrevCount{}
	, mTextToDraw{}
	, mAccTime(0.f)
{
	Init();
}

void t2g::Time::Init()
{
	BOOL result = QueryPerformanceFrequency(&mFrequencyPerSecond);
	assert(result && "Time::Init: Falid Get mFrequencyPerSecond");

	QueryPerformanceCounter(&mPrevCount);
	mRectToDraw = RECT(0, 0, 150, 20);
}

void t2g::Time::Update()
{
	QueryPerformanceCounter(&mCurCount);
	mDeltaTime = (float)(mCurCount.QuadPart - mPrevCount.QuadPart) / (float)mFrequencyPerSecond.QuadPart;
	mPrevCount = mCurCount;
	mAccTime += mDeltaTime;
	if (mAccTime > 1.f)
	{
		mTextToDraw = L"FPS: " + std::to_wstring(1.f / mDeltaTime);
		mAccTime = 0.f;
	}
}

void t2g::Time::Render()
{
	HDC hBackDC = GET_SINGLETON(Application).GetBackDC();
	DrawText(hBackDC, mTextToDraw.c_str(), (int)mTextToDraw.size(), &mRectToDraw, DT_LEFT | DT_TOP);
}
