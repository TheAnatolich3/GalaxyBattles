#pragma once
#include "EventsManager.hpp"


void EventsManager::add_delegate(Delegate* delegate) const
{
	_delegates.push_back(delegate);
}