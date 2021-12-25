#include <algorithm>
#include <SDL/SDLWindow.hpp>
#include <Renderer.hpp>
#include <Node.hpp>
#include <memory>
#include "Engine.hpp"

Engine::Engine() = default;

Engine::~Engine() = default;

bool Engine::isActive() {
	return _isActive;
}

void Engine::init(std::string_view name_window, size_t width, size_t height, std::string mode_input)
{
	std::for_each(mode_input.begin(), mode_input.end(), [](char& c)
		{
			c = toupper(c);
		});

	SDLWindow::RenderMode mode = ((mode_input == "OPENGL") ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

	_isActive = true;
	_window = std::make_unique<SDLWindow>(*this, name_window.data(), width, height, mode);
	_renderer = _window->createRenderer();
	_scene = std::make_shared<Node>();
	_audioManager = std::make_unique<AudioManager>();
	_UIManager = std::make_unique<UIManager>(*this);
	_eventsManager = std::make_unique<EventsManager>();
	_eventsManager->add_delegate(this);
}

void Engine::update() {
	_window->update();
	_scene->visit();
	_UIManager->visit();
	_renderer->draw();
	_window->swap();
	_audioManager->update();
}


const EventsManager& Engine::eventsManager() const
{
	return *_eventsManager;
}

const Renderer& Engine::renderer() const
{
	return *_renderer;
}
[[nodiscard]] const Window& Engine::window() const
{
	return *_window;
}

void Engine::handle_event(EventsManager::QuitEvent ev)
{
	_isActive = false;
}

void Engine::handle_event(EventsManager::KeyEvent ev)
{

}


size_t Engine::get_window_width() const
{
	return _window->getWidth();
}

size_t Engine::get_window_height() const
{
	return _window->getHeight();
}

std::shared_ptr<Node> Engine::scene()
{
	return _scene;
}

const AudioManager& Engine::audioManager() const
{
	return *_audioManager;
}
