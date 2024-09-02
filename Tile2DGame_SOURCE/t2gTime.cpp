#include "t2gTime.h"
#include "t2gApplication.h"

t2g::Time::Time()
	: mFrequencyPerSecond{}
	, mCurCount{}
	, mPrevCount{}
	, mTextToDraw{}
{
	Init();
}

void t2g::Time::Init()
{
	assert(QueryPerformanceFrequency(&mFrequencyPerSecond) && "Time::Init: Falid Get mFrequencyPerSecond");
	QueryPerformanceCounter(&mPrevCount);
	mRectToDraw = RECT(0, 0, 500, 20);
}

void t2g::Time::Update()
{
	QueryPerformanceCounter(&mCurCount);
	mDeltaTime = (float)(mCurCount.QuadPart - mPrevCount.QuadPart) / (float)mFrequencyPerSecond.QuadPart;
	mPrevCount = mCurCount;
}

void t2g::Time::Render()
{
	HDC hBackDC = GET_SINGLETON(Application).GetBackDC();
	mTextToDraw = L"FPS: " + std::to_wstring(1.f / mDeltaTime);
	DrawText(hBackDC, mTextToDraw.c_str(), mTextToDraw.size(), &mRectToDraw, DT_LEFT | DT_TOP);
}
