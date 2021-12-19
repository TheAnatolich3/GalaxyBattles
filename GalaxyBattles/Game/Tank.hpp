#include <memory>
#include <EventsManager.hpp>
#include <chrono>
#include "Sprite.hpp"

class Sound;

class Tank : public EventsManager::Delegate, public Node
{
public:
	explicit Tank(const Engine& engine); 
	~Tank();

	void handle_event(EventsManager::QuitEvent) override;
	void handle_event(EventsManager::KeyEvent) override;
	
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
	std::shared_ptr<Sound> _shot;
};