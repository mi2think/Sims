/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   23:42
	filename: 	D:\Code\Sims\Sims\core\mouse_event.h
	file path:	D:\Code\Sims\Sims\core
	file base:	mouse_event
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Mouse Event
*********************************************************************/
#ifndef __MOUSE_EVENT_H__
#define __MOUSE_EVENT_H__

#include "event.h"

namespace sims
{
	enum MouseButtonType
	{
		MOUSE_LBUTTON,
		MOUSE_RBUTTON,
		MOUSE_MBUTTON,

		MOUSE_BUTTON_COUNT,
	};

	class MouseMoveEvent : public BaseEvent<Event::MOUSE_MOVE>
	{
	public:
		MouseMoveEvent(int x, int y) : x_(x), y_(y) {}

		string ToString() { return str_format("MouseMoveEvent:(%d,%d)", x_, y_); }
		int GetX() const { return x_; }
		int GetY() const { return y_; }
	private:
		int x_;
		int y_;
	};

	class MousePressEvent : public BaseEvent<Event::MOUSE_PRESS>
	{
	public:
		MousePressEvent(int x, int y, MouseButtonType buttonType)
			: x_(x)
			, y_(y)
			, buttonType_(buttonType)
		{}

		string ToString() { return str_format("MousePressEvent:(%d,%d,%d)", buttonType_, x_, y_); }
		int GetX() const { return x_; }
		int GetY() const { return y_; }
		MouseButtonType GetButtonType() const { return buttonType_; }
	private:
		int x_;
		int y_;
		MouseButtonType buttonType_;
	};

	class MouseReleaseEvent : public BaseEvent<Event::MOUSE_RELEASE>
	{
	public:
		MouseReleaseEvent(int x, int y, MouseButtonType buttonType)
			: x_(x)
			, y_(y)
			, buttonType_(buttonType)
		{}

		string ToString() { return str_format("MouseReleaseEvent:(%d,%d,%d)", buttonType_, x_, y_); }
		int GetX() const { return x_; }
		int GetY() const { return y_; }
		MouseButtonType GetButtonType() const { return buttonType_; }
	private:
		int x_;
		int y_;
		MouseButtonType buttonType_;
	};

	class MouseWheelEvent : public BaseEvent<Event::MOUSE_WHELL>
	{
	public:
		MouseWheelEvent(float wheelDelta) : wheelDelta_(wheelDelta) {}

		string ToString() { return str_format("MouseWheelEvent:(%.2f)", wheelDelta_); }
		float GetWheelDelta() const { return wheelDelta_; }
	private:
		float wheelDelta_;
	};
}

#endif
