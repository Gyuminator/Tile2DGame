#include "stdafx.h"
#include "t2gInput.h"

#include "t2gApplication.h"

t2g::Input::Input()
	: mKeyInfos{}
{
	Init();
}

t2g::Input::~Input()
{
}

void t2g::Input::Init()
{
	virtualKeyMapping();
}

void t2g::Input::Update()
{
	checkKeyInput();
}

void t2g::Input::Render()
{

}

void t2g::Input::virtualKeyMapping()
{
	UINT16 vKeys[(size_t)eKeys::END] =
	{
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_NUMPAD0,
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_RBUTTON,
		VK_CONTROL, VK_MENU, VK_LSHIFT, VK_TAB, VK_ESCAPE, VK_RETURN
		//END
	};
	for (size_t i = 0; i < (size_t)eKeys::END; ++i)
	{
		mKeyInfos[i].VKey = (UINT16)vKeys[i];
		mKeyInfos[i].State = eKeyState::None;
	}
}

void t2g::Input::checkKeyInput()
{
	if (GetFocus() == GET_SINGLETON(Application).GetHWnd())
	{
		for (KeyInfo& keyInfo : mKeyInfos)
		{
			if (GetAsyncKeyState(keyInfo.VKey) & 0x8000)
			{
				applyKeyDown(keyInfo.State);
			}
			else
			{
				applyKeyUp(keyInfo.State);
			}
		}
	}
	else
	{
		for (KeyInfo& keyInfo : mKeyInfos)
		{
			applyKeyUp(keyInfo.State);
		}
	}
}

void t2g::Input::applyKeyDown(eKeyState& state)
{
	switch (state)
	{
	case eKeyState::None:
		state = eKeyState::Down;
		break;
	case eKeyState::Down:
		state = eKeyState::Pressed;
		break;
	case eKeyState::Up:
		state = eKeyState::Down;
		break;
	}
}

void t2g::Input::applyKeyUp(eKeyState& state)
{
	switch (state)
	{
	case eKeyState::Down:
	case eKeyState::Pressed:
		state = eKeyState::Up;
		break;
	case eKeyState::Up:
		state = eKeyState::None;
		break;
	}
}
