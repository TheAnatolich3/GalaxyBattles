#pragma once
#define GLEW_STATIC
#include <algorithm>
#include <SDL/SDLWindow.hpp>
#include <Renderer.hpp>
#include "Engine.hpp"

Engine::Engine() {
	_eventsManager = std::make_unique<EventsManager>();
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
}

void Engine::update() {
	_window->update();
	_renderer->draw();
	_window->swap();
}


const EventsManager& Engine::get_event_manager() const
{
	return *_eventsManager;
}

void Engine::handle_event(EventsManager::QuitEvent ev)
{
	_isActive = false;
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