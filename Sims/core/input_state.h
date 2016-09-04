/********************************************************************
	created:	2016/09/05
	created:	5:9:2016   0:15
	filename: 	D:\Code\Sims\Sims\core\input_state.h
	file path:	D:\Code\Sims\Sims\core
	file base:	input_state
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Input State
*********************************************************************/
#ifndef __INPUT_STATE_H__
#define __INPUT_STATE_H__

#include "key_event.h"
#include "mouse_event.h"

namespace sims
{
	class InputState
	{
	public:
		// Key State
		static bool GetKeyState(int key);
		static void SetKeyState(int key, bool pressed);
		static void OnKeyPress(const KeyPressEvent& event);
		static void OnKeyRelease(const KeyReleaseEvent& event);

		// Mouse State
		static bool GetMouseState(MouseButtonType buttonType);
		static void SetMouseState(MouseButtonType buttonType, bool pressed);
		static void OnMousePress(const MousePressEvent& event);
		static void OnMouseRelease(const MouseReleaseEvent& event);
	};
}

#endif