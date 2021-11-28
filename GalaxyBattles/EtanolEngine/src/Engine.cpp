#pragma once
#define GLEW_STATIC
#include <algorithm>
#include <SDL_version.h>
#include <SDL.h>
#include <GL/glew.h>
#include <EventsManager.hpp>
#include <SDL/SDLWindow.hpp>
#include <Window.hpp>
#include <Renderer.hpp>
#include "Engine.hpp"
#include "windows.h"

GLuint _VAO;
int _uScreenSize;
GLuint _program;

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
	
	/*if (mode == SDLWindow::RenderMode::OpenGL)
	{
		auto context = SDL_GL_CreateContext(_window.get());
		glewExperimental = GL_TRUE;
		GLenum res = glewInit();
		if (res != GLEW_OK)
		{
			throw std::runtime_error("Glew error");
		}
	}
	/*

	_pmp->screen_surface = SDL_GetWindowSurface(_pmp->window);
	_pmp->ren = SDL_CreateRenderer(_pmp->window, -1, SDL_RENDERER_ACCELERATED);*/
	//std::ifstream in("../../../../GalaxyBattles/EtanolEngine/src/shader.vert");
}

void Engine::update() {
	_window->update();
	_renderer->draw();
	_window->swap();

	/*
	glUseProgram(_program);
	glUniform2f(_uScreenSize, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	*/
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