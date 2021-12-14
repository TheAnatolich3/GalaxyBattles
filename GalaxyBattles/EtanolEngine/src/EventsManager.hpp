#pragma once
#ifndef EVENTS_MANAGER_HPP
#define EVENTS_MANAGER_HPP
#include <vector>

class EventsManager {
public:

	enum class KeyCode
	{
		Up,
		Down,
		Left,
		Right,
		A,
		D,
		Space,
		Unknown
	};

	enum class KeyType
	{
		KeyDown,
		KeyUp
	};

	struct KeyEvent
	{
		KeyCode key;
		KeyType type;
	};

	struct MouseEvent
	{
		int x, y;
	};

	struct QuitEvent { };

	struct Delegate
	{
		virtual void handle_event(QuitEvent) = 0;
		virtual void handle_event(KeyEvent) = 0;
		//virtual void handle_event(MouseEvent) = 0;
	};

	template<typename T>
	void invoke_event(T event) const
	{
		for (auto delegate : _delegates)
		{
			delegate->handle_event(event);
		}
	}

	void add_delegate(Delegate* delegate);
private:
	std::vector<Delegate*> _delegates;
};

#endif EVENTS_MANAGER_HPP