#pragma once
#include "EventsManager.hpp"


void EventsManager::add_delegate(Delegate* delegate)
{
	_delegates.push_back(delegate);
}