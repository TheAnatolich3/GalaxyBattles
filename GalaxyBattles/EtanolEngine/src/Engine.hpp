#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <string_view>
#include <iostream>
#include <EventsManager.hpp>
#include <memory>
#include <AudioManager.hpp>
#include <imgui.h>

class Window;
class Renderer;
class Node;

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

	explicit Engine(std::shared_ptr<EventsManager> ea);
	~Engine();

	void init(std::string_view name_window, size_t width, size_t height, std::string mode_input);

	void update();
	bool isActive();

	void handle_event(EventsManager::QuitEvent ev);
	void handle_event(EventsManager::KeyEvent);

	void load_picture(std::vector<Triangle> model);

	size_t get_window_width() const;
	size_t get_window_height() const;

	[[nodiscard]] const EventsManager& eventsManager() const;
	[[nodiscard]] const Renderer& renderer() const;
	[[nodiscard]] const Window& window() const;
	[[nodiscard]] const AudioManager& audioManager() const;

	std::shared_ptr<Node> scene();
private:
	
	bool _isActive = false;

	std::unique_ptr<Window> _window;
	std::shared_ptr<EventsManager> _eventsManager;
	std::unique_ptr<AudioManager> _audioManager;
	std::unique_ptr<Renderer> _renderer;
	
	std::shared_ptr<Node> _scene;
};

#endif ENGINE_HPP