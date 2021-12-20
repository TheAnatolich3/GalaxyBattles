#ifndef BUTTON_HPP
#define BUTTON_NPP
#include <Node.hpp>
#include <EventsManager.hpp>
#include <string_view>

class Engine;
class Sprite;

class Button : public Node, public EventsManager::Delegate
{
public:
	explicit Button(const Engine& engine, std::string_view file_name);

	void handle_event(EventsManager::MouseEvent) override;
	void visitSelf() override;
	bool getStatus() const;
private:
	bool status = false;
	std::shared_ptr<Sprite> _body;
};
#endif BUTTON_NPP