#pragma once
#ifndef GAME_SDLWINDOW_HPP
#define GAME_SDLWINDOW_HPP

#include <Window.hpp>

#include <SDL.h>
#include <string_view>

class Engine;

class SDLWindow final : public Window
{
public:
	enum class RenderMode
	{
		OpenGL,
		Software
	};

	SDLWindow(const Engine& engine,
		std::string_view name,
		size_t width,
		size_t height,
		RenderMode renderMode);

	void update() override;
	void swap() override;
	std::unique_ptr<Renderer> createRenderer() override;
private:
	std::shared_ptr<SDL_Window> _window;
	RenderMode _renderMode;
	const Engine& _engine;
};

#endif GAME_SDLWINDOW_HPP