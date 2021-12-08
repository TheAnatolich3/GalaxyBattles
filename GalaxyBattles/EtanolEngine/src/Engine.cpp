#include <algorithm>
#include <SDL/SDLWindow.hpp>
#include <Renderer.hpp>
#include <Node.hpp>
#include <memory>
#include "Engine.hpp"

Engine::Engine(std::shared_ptr<EventsManager> ea) {
	_eventsManager = ea;
	_eventsManager->add_delegate(this);
}

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
}

void Engine::update() {
	_window->update();
	_scene->visit();
	_renderer->draw();
	_window->swap();
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

//void Engine::handle_event(EventsManager::KeyDownEvent ev)
//{
//	_scene->setPosition(glm::vec2(_scene->getPosition().x, _scene->getPosition().y + 1));
//}
//
//
//void Engine::handle_event(EventsManager::KeyUpEvent ev)
//{
//	_scene->setPosition(glm::vec2(_scene->getPosition().x, _scene->getPosition().y - 1));
//}
//
//void Engine::handle_event(EventsManager::KeyLeftEvent ev)
//{
//	_scene->setPosition(glm::vec2(_scene->getPosition().x - 1, _scene->getPosition().y));
//}
//
//void Engine::handle_event(EventsManager::KeyRightEvent ev)
//{
//	_scene->setPosition(glm::vec2(_scene->getPosition().x + 1, _scene->getPosition().y));
//}
//
//void Engine::handle_event(EventsManager::KeySpaceEvent ev)
//{
//	std::cout << "BOOM!" << std::endl;
//}
//
//
//void Engine::handle_event(EventsManager::KeyAEvent ev)
//{
//
//}
//
//void Engine::handle_event(EventsManager::KeyDEvent ev)
//{
//
//}

void Engine::handle_event(EventsManager::KeyEvent ev)
{

}


void Engine::load_picture(std::vector<Triangle> model)
{
	_renderer->load_picture(model);
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
