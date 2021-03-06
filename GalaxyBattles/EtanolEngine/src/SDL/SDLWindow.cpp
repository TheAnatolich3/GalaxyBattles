#include <Engine.hpp>
#include <EventsManager.hpp>
#include <SDL/SDLRenderer.hpp>
#include <SDL.h>
#include <GL/GLRenderer.hpp>
#include "SDLWindow.hpp"


SDLWindow::SDLWindow(const Engine& engine,
				std::string_view name,
				size_t width,
				size_t height,
				RenderMode renderMode) :
	Window(width, height),
	_engine(engine),
	_renderMode(renderMode)
{
    SDL_version version = { 0, 0, 0 };

    SDL_GetVersion(&version);

    printf("SDL Version: %d.%d.%d\n", version.major, version.minor, version.patch);

    SDL_Init(SDL_INIT_EVERYTHING);

    Uint32 windowFlags = SDL_WINDOW_SHOWN;

    if (_renderMode == RenderMode::OpenGL)
    {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        windowFlags |= SDL_WINDOW_OPENGL;
    }

    _window = { SDL_CreateWindow(name.data(),
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             static_cast<int>(width),
                             static_cast<int>(height),
                             windowFlags), SDL_DestroyWindow };
}


void SDLWindow::update()
{
    SDL_Event e;
    const auto& event_manager = _engine.eventsManager();

    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT)
    {
        event_manager.invoke_event(EventsManager::QuitEvent{});
    }
    else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
    {
        EventsManager::Action type = EventsManager::Action::Up;
        if (e.type == SDL_KEYDOWN)
            type = EventsManager::Action::Down;
        EventsManager::KeyCode code = EventsManager::KeyCode::Unknown;

        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            code = EventsManager::KeyCode::Up;
            break;
        case SDLK_DOWN:
            code = EventsManager::KeyCode::Down;
            break;
        case SDLK_LEFT:
            code = EventsManager::KeyCode::Left;
            break;
        case SDLK_RIGHT:
            code = EventsManager::KeyCode::Right;
            break;
        case SDLK_SPACE:
            code = EventsManager::KeyCode::Space;
            break;
        case SDLK_a:
            code = EventsManager::KeyCode::A;
            break;
        case SDLK_d:
            code = EventsManager::KeyCode::D;
            break;
        case SDLK_ESCAPE:
            code = EventsManager::KeyCode::Escape;
            break;
        default: break;
        }
        event_manager.invoke_event(EventsManager::KeyEvent{ code, type });
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        EventsManager::MouseEvent ev;
        switch (e.button.button)
        {
        case SDL_BUTTON_LEFT: ev.type = EventsManager::MouseEvent::Type::LButtonDown; break;
        case SDL_BUTTON_RIGHT: ev.type = EventsManager::MouseEvent::Type::RButtonDown; break;
        case SDL_BUTTON_MIDDLE: ev.type = EventsManager::MouseEvent::Type::MButtonDown; break;
        }

        ev.mousePos = glm::vec2(e.button.x, e.button.y);

        event_manager.invoke_event(ev);
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        EventsManager::MouseEvent ev;
        switch (e.button.button)
        {
        case SDL_BUTTON_LEFT: ev.type = EventsManager::MouseEvent::Type::LButtonUp; break;
        case SDL_BUTTON_RIGHT: ev.type = EventsManager::MouseEvent::Type::RButtonUp; break;
        case SDL_BUTTON_MIDDLE: ev.type = EventsManager::MouseEvent::Type::MButtonUp; break;
        }

        ev.mousePos = glm::vec2(e.button.x, e.button.y);

        event_manager.invoke_event(ev);
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        event_manager.invoke_event(EventsManager::MouseEvent{ EventsManager::MouseEvent::Type::Move, glm::vec2(e.button.x, e.button.y) });
    }
    else if (e.type == SDL_TEXTINPUT)
    {
        event_manager.invoke_event(EventsManager::TextInputEvent{ e.text.text });
    }
    else if (e.type == SDL_MOUSEWHEEL)
    {
        event_manager.invoke_event(EventsManager::MouseWheelEvent{e.wheel.y});
    }
}

std::unique_ptr<Renderer> SDLWindow::createRenderer()
{
    if (_renderMode == RenderMode::OpenGL)
    {
        return std::make_unique<GLRenderer>(_engine, _window.get());
    }
    else 
    {
        return std::make_unique<SDLRenderer>(_engine, _window);
    }
}

void SDLWindow::swap()
{
    if (_renderMode == RenderMode::OpenGL)
    {
        SDL_GL_SwapWindow(_window.get());
    }
}