#include <Engine.hpp>
#include <Sprite.hpp>
#include "Button.hpp"


Button::Button(const Engine& engine, std::string_view file_name)
{
	_body = std::make_shared<Sprite>(engine, file_name.data());
	glm::vec2 button_size = _body->getSize();
	this->setPosition(glm::vec2(button_size.x/2, button_size.y/2));
	this->setAnchor(glm::vec2(0.0f));
	this->addNode(_body);
	engine.eventsManager().add_delegate(this);
}

void Button::visitSelf()
{
	
}


void Button::handle_event(EventsManager::MouseEvent me)
{
	glm::vec2 button_size = _body->getSize();
	glm::vec2 buttom_pos = this->getPosition();
	if (me.mousePos.x > buttom_pos.x - button_size.x / 2
		&& me.mousePos.x < buttom_pos.x + button_size.x / 2
		&& me.mousePos.y > buttom_pos.y - button_size.y / 2
		&& me.mousePos.y < buttom_pos.y + button_size.y / 2
		&& me.type == EventsManager::MouseEvent::Type::LButtonDown)
	{
		this->setScale(glm::vec2(0.8f));
		status = !status;
	}
	else
	{
		this->setScale(glm::vec2(1.0f));
	}
}

bool Button::getStatus() const
{
	return status;
}