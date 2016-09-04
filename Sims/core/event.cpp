/********************************************************************
	created:	2016/09/04
	created:	4:9:2016   23:20
	filename: 	D:\Code\Sims\Sims\core\event.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	event
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Event
*********************************************************************/
#include "event.h"

namespace sims
{
	const char _BaseEventName[] = "";

	void EventBroadcaster::RegisterEventListener(IEventListener* listener)
	{
		if (listener == nullptr)
			return;

		auto it = std::find(listeners_.begin(), listeners_.end(), listener);
		if (it == listeners_.end())
		{
			listeners_.push_back(listener);
		}
	}

	void EventBroadcaster::UnregisterEventListener(IEventListener* listener)
	{
		if (listener == nullptr)
			return;

		auto it = std::find(listeners_.begin(), listeners_.end(), listener);
		if (it != listeners_.end())
		{
			listeners_.erase(it);
		}
	}

	bool EventBroadcaster::PostEvent(const Event& event)
	{
		bool handled = false;
		for (auto listener : listeners_)
		{
			handled |= listener->OnEvent(event);
		}
		return handled;
	}
}