/********************************************************************
	created:	2016/09/05
	created:	5:9:2016   0:18
	filename: 	D:\Code\Sims\Sims\core\input_state.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	input_state
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Input State
*********************************************************************/
#include "input_state.h"

namespace sims
{
	static bool s_KeyState[KEY_CODES_COUNT] = { false };
	static bool s_MouseState[MOUSE_BUTTON_COUNT] = { false };

	bool InputState::GetKeyState(int key)
	{
		ASSERT(key < KEY_CODES_COUNT);
		return s_KeyState[key];
	}

	void InputState::SetKeyState(int key, bool pressed)
	{
		ASSERT(key < KEY_CODES_COUNT);
		s_KeyState[key] = pressed;
	}

	void InputState::OnKeyPress(const KeyPressEvent& event)
	{
		int key = event.GetKey();
		SetKeyState(key, true);
	}

	void InputState::OnKeyRelease(const KeyReleaseEvent& event)
	{
		int key = event.GetKey();
		SetKeyState(key, false);
	}

	bool InputState::GetMouseState(MouseButtonType buttonType)
	{
		ASSERT(buttonType < MOUSE_BUTTON_COUNT);
		return s_MouseState[buttonType];
	}

	void InputState::SetMouseState(MouseButtonType buttonType, bool pressed)
	{
		ASSERT(buttonType < MOUSE_BUTTON_COUNT);
		s_MouseState[buttonType] = pressed;
	}

	void InputState::OnMousePress(const MousePressEvent& event)
	{
		MouseButtonType buttonType = event.GetButtonType();
		SetMouseState(buttonType, true);
	}

	void InputState::OnMouseRelease(const MouseReleaseEvent& event)
	{
		MouseButtonType buttonType = event.GetButtonType();
		SetMouseState(buttonType, false);
	}
}