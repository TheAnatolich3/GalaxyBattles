#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <memory>
#include <Renderer.hpp>
#include <glm/glm.hpp>
#include <EventsManager.hpp>
#include <MenuItem.hpp>


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
	void handle_event(EventsManager::KeyEvent) override;

	void addMenuItem(std::shared_ptr<Menu::MenuItem> item);
	void removeMenuItem(const std::shared_ptr<Menu::MenuItem>& item);
	bool get_show_flag() const
	{
		return _show_menu;
	}
private:
	const Engine& _engine;

	Renderer::Command _command;

	std::shared_ptr<TextureUniform> _textureUniform;
	std::shared_ptr<Vec2Uniform> _screenSizeUniform;
	std::shared_ptr<Mat3Uniform> _transformUniform;

	bool _show_demo_window = false;
	bool _show_menu = false;

	bool _rMousePressed = false;
	bool _lMousePressed = false;
	bool _mMousePressed = false;

	glm::vec2 _mousePos;

	float _mouseWheel = 0.0f;

	std::vector<std::shared_ptr<Menu::MenuItem>> _menuItems;
};

#endif UIMANAGER_HPP

