#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <memory>
#include <Renderer.hpp>
#include <glm/glm.hpp>
#include <EventsManager.hpp>

class Engine;
class Texture;
class TextureUniform;
class Vec2Uniform;
class Mat3Uniform;

class UIManager final : public EventsManager::Delegate
{
public:
	explicit UIManager(const Engine& engine);
	void visit();
	void handle_event(EventsManager::MouseEvent) override;
	void handle_event(EventsManager::TextInputEvent) override;
	void handle_event(EventsManager::MouseWheelEvent) override;
private:
	const Engine& _engine;

	Renderer::Command _command;

	std::shared_ptr<TextureUniform> _textureUniform;
	std::shared_ptr<Vec2Uniform> _screenSizeUniform;
	std::shared_ptr<Mat3Uniform> _transformUniform;

	bool show_demo_window = true;

	bool _rMousePressed = false;
	bool _lMousePressed = false;
	bool _mMousePressed = false;

	glm::vec2 _mousePos;

	float _mouseWheel = 0.0f;
};

#endif UIMANAGER_HPP

