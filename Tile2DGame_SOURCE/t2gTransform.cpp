#include "stdafx.h"
#include "t2gTransform.h"

#include "t2gApplication.h"
#include "t2gCamera.h"
#include "t2gObject.h"


t2g::Transform::Transform()
	: mLocation(Vector3::Zero())
	, mRotation(Vector3::Zero())
	, mScale(Vector3::One())
	, mUpdate(nullptr)
	, mRender(nullptr)
{
}

void t2g::Transform::SyncWithOtherComponents()
{
	//mRender = (&Transform::showText);
}

void t2g::Transform::Init(const Vector3& location, const Vector3& rotation, const Vector3& scale)
{
	mLocation = location;
	mRotation = rotation;
	mScale = scale;

	//BindToScene
}

eDelegateResult t2g::Transform::cbTest()
{
	SafePtr<Camera> camera = GetOwner()->GetComponent<Camera>(eComponentType::Camera);
	if (camera.IsEmpty())
		return eDelegateResult::OK;



	return eDelegateResult::OK;
}

void t2g::Transform::showText()
{
	HDC backDC = GET_SINGLETON(Application).GetBackDC();

	RECT rect(
		(LONG)mLocation.x, (LONG)mLocation.y,
		(LONG)mLocation.x + 300, (LONG)mLocation.y + 20);

	//DrawText(backDC, L"플레이어", lstrlenW(L"플레이어"), &rect, DT_LEFT | DT_TOP);
}

