#include "stdafx.h"
#include "t2gPlayerController.h"

#include "t2gInput.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gTime.h"

using namespace std::enums;

void t2g::PlayerController::SyncWithOtherComponents()
{
	mTransform = GetOwnerObj()->GetComponent(eComponentType::Transform);
}

void t2g::PlayerController::Init()
{
	SetUpdateLayer(eUpdateLayer::Input);
	SetRenderLayer(eRenderLayer::EnumEnd);

	BindBackToUpdates(&PlayerController::cbCheckTransform);
	BindBackToUpdates(&PlayerController::cbProcArrowKeys);
}

eDelegateResult t2g::PlayerController::cbProcArrowKeys()
{
	Vector3 location = mTransform->GetLocation();
	Vector3 rotation = mTransform->GetRotation();

	if (GET_SINGLETON(Input).CheckKey(eKeys::Left, eKeyState::Pressed))
	{
		location.x -= 300 * GET_SINGLETON(Time).GetDT();
		rotation.z = 180.f;
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Right, eKeyState::Pressed))
	{
		location.x += 300 * GET_SINGLETON(Time).GetDT();
		rotation.z = 0.f;
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Up, eKeyState::Pressed))
	{
		location.y -= 300 * GET_SINGLETON(Time).GetDT();
		rotation.z = 270.f;
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Down, eKeyState::Pressed))
	{
		location.y += 300 * GET_SINGLETON(Time).GetDT();
		rotation.z = 90.f;
	}

	mTransform->SetLocation(location);
	mTransform->SetRotation(rotation);

	return eDelegateResult::OK;
}

eDelegateResult t2g::PlayerController::cbCheckTransform()
{
	if (mTransform.IsEmpty())
		return eDelegateResult::Return;
	else
		return eDelegateResult::Erase;
}
