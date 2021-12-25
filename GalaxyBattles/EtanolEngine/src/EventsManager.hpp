#pragma once
#ifndef EVENTS_MANAGER_HPP
#define EVENTS_MANAGER_HPP
#include <vector>
#include <glm/glm.hpp>

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

	enum class Action
	{
		Down,
		Up
	};

	struct KeyEvent
	{
		KeyCode key;
		Action type;
	};

	struct MouseEvent
	{
		enum class Type
		{
			Move,
			LButtonDown,
			LButtonUp,
			RButtonDown,
			RButtonUp,
			MButtonDown,
			MButtonUp
		} type;
		glm::vec2 mousePos;
	};

	struct QuitEvent { };

	struct Delegate
	{
		virtual void handle_event(QuitEvent) {};
		virtual void handle_event(KeyEvent) {};
		virtual void handle_event(MouseEvent) {};
	};

	template<typename T>
	void invoke_event(T event) const
	{
		for (auto delegate : _delegates)
		{
			delegate->handle_event(event);
		}
	}

	void add_delegate(Delegate* delegate) const;
private:
	mutable std::vector<Delegate*> _delegates;
};

#endif EVENTS_MANAGER_HPP