#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <string_view>
#include <EventsManager.hpp>
#include <memory>
#include <AudioManager.hpp>
#include <UIManager.hpp>

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

	explicit Engine();
	~Engine();

	void init(std::string_view name_window, size_t width, size_t height, std::string mode_input);

	void update();
	bool isActive();

	void handle_event(EventsManager::QuitEvent ev);
	void handle_event(EventsManager::KeyEvent ke);

	size_t get_window_width() const;
	size_t get_window_height() const;

	[[nodiscard]] const EventsManager& eventsManager() const;
	[[nodiscard]] const Renderer& renderer() const;
	[[nodiscard]] const Window& window() const;
	[[nodiscard]] const AudioManager& audioManager() const;

	std::shared_ptr<Node> scene();
	std::shared_ptr<UIManager> UI_Manager();
private:
	
	bool _isActive = false;

	std::unique_ptr<Window> _window;
	std::unique_ptr<EventsManager> _eventsManager;
	std::unique_ptr<AudioManager> _audioManager;
	std::unique_ptr<Renderer> _renderer;
	std::shared_ptr<UIManager> _UIManager;
	std::shared_ptr<Node> _scene;
};

#endif ENGINE_HPP