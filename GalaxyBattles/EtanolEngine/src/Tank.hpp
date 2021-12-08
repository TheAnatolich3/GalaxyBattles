#include <memory>
#include <EventsManager.hpp>
#include <Sprite.hpp>

class Tank : public EventsManager::Delegate, public Node
{
public:
	explicit Tank(const Engine& engine);

	void handle_event(EventsManager::QuitEvent) override;
	void handle_event(EventsManager::KeyEvent) override;
	/*void handle_event(EventsManager::KeyDownEvent) override;
	void handle_event(EventsManager::KeyUpEvent) override;
	void handle_event(EventsManager::KeyLeftEvent) override;
	void handle_event(EventsManager::KeyRightEvent) override;
	void handle_event(EventsManager::KeySpaceEvent) override;
	void handle_event(EventsManager::KeyAEvent) override;
	void handle_event(EventsManager::KeyDEvent) override;*/
	
	void visitSelf() override;
private:
	bool _isUp = false;
	bool _isDown = false;
	bool _isLeft = false;
	bool _isRight = false;
	bool _isA = false;
	bool _isD = false;
	bool _isSpace = false;

	std::shared_ptr<Sprite> _body, _head;
	float _turnSpeed = 0;
	float _speed = 0;
	float _rotation = 0;
	std::chrono::steady_clock::time_point _prevTime;
};