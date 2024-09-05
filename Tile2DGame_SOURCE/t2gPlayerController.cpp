#include "t2gPlayerController.h"
#include "t2gInput.h"
#include "t2gObject.h"
#include "t2gTransform.h"
#include "t2gTime.h"

using namespace std::enums;

void t2g::PlayerController::SyncBindings()
{
	mTransform = GetOwner()->GetComponent(eComponentType::Transform);
}

void t2g::PlayerController::update()
{
	if (mTransform.IsEmpty())
		return;

	Vector3 location = mTransform->GetLocation();

	if (GET_SINGLETON(Input).CheckKey(eKeys::Left, eKeyState::Pressed))
	{
		location.x -= 100 * GET_SINGLETON(Time).GetDT();
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Right, eKeyState::Pressed))
	{
		location.x += 100 * GET_SINGLETON(Time).GetDT();
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Up, eKeyState::Pressed))
	{
		location.y -= 100 * GET_SINGLETON(Time).GetDT();
	}
	if (GET_SINGLETON(Input).CheckKey(eKeys::Down, eKeyState::Pressed))
	{
		location.y += 100 * GET_SINGLETON(Time).GetDT();
	}
	
	mTransform->SetLocation(location);
}
