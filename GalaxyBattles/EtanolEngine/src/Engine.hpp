#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <string_view>
#include <iostream>
#include <EventsManager.hpp>

class Window;
class Renderer;

class Engine : public EventsManager::Delegate{
public:
	struct Vertex
	{
		float x, y, r, g, b;
	};

	struct Triangle
	{
		Vertex v1, v2, v3;
	};

	explicit Engine();
	~Engine();

	void init(std::string_view name_window, size_t width, size_t height, std::string mode_input);

	void update();
	bool isActive();

	const EventsManager& get_event_manager() const;
	void handle_event(EventsManager::QuitEvent ev);

	void load_picture(std::vector<Triangle> model);

	size_t get_window_width() const;
	size_t get_window_height() const;
private:
	
	bool _isActive = false;

	std::unique_ptr<Window> _window;
	std::unique_ptr<EventsManager> _eventsManager;
	std::unique_ptr<Renderer> _renderer;
};

#endif ENGINE_HPP