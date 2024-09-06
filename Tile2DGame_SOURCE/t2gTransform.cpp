#include "t2gTransform.h"
#include "t2gApplication.h"

t2g::Transform::Transform()
	: mLocation(Vector3::Zero())
	, mRotation(Vector3::Zero())
	, mScale(Vector3::One())
	, mUpdate(nullptr)
	, mRender(nullptr)
{
}

void t2g::Transform::SyncBindings()
{
	mRender = (&Transform::showText);
}

void t2g::Transform::update()
{
	
}

void t2g::Transform::render()
{
	((*this).*mRender)();
}

void t2g::Transform::showText()
{
	HDC backDC = GET_SINGLETON(Application).GetBackDC();

	RECT rect(
		(LONG)mLocation.x, (LONG)mLocation.y,
		(LONG)mLocation.x + 300, (LONG)mLocation.y + 20);

	//DrawText(backDC, L"플레이어", lstrlenW(L"플레이어"), &rect, DT_LEFT | DT_TOP);
}

