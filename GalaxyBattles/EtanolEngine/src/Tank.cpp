#include <chrono>
#include <glm/gtx/rotate_vector.hpp>
#include "Tank.hpp"


Tank::Tank(const Engine& engine)
{
	_body = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_body_removed_back.png");
	_head = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_head_removed_back.png");
	_body->setPosition(glm::vec2(engine.get_window_width() * 0.2f,
		engine.get_window_height() * 0.5f));
	_head->setScale(glm::vec2(0.6f, 0.6f));

	glm::vec2 tank_body_size = _body->getSize();

	_head->setPosition(glm::vec2(tank_body_size.x * 0.53, tank_body_size.y * 0.5));

	_head->setAnchor(glm::vec2(0.368f, 0.5f));
	_body->addNode(_head);
	this->addNode(_body);
}

void Tank::handle_event(EventsManager::QuitEvent)
{

}

//void Tank::handle_event(EventsManager::KeyDownEvent ev)
//{
//	_isDown = ev.f;
//	std::cout << "Down!" << std::endl;
//}
//
//void Tank::handle_event(EventsManager::KeyUpEvent ev)
//{
//	_isUp = ev.f;
//	std::cout << "Up!" << std::endl;
//}
//
//void Tank::handle_event(EventsManager::KeyLeftEvent ev)
//{
//	_isLeft = ev.f;
//	std::cout << "Left!" << std::endl;
//}
//
//void Tank::handle_event(EventsManager::KeyRightEvent ev)
//{
//	_isRight = ev.f;
//	std::cout << "Right!" << std::endl;
//}
//
//void Tank::handle_event(EventsManager::KeySpaceEvent ev)
//{
//	_isSpace = ev.f;
//	std::cout << "BOOM!" << std::endl;
//}
//
//void Tank::handle_event(EventsManager::KeyAEvent ev)
//{
//	_isA = ev.f;
//	std::cout << "AAAAAA!" << std::endl;
//	//_head->setRotation(_head->getRotation() - 5);
//}
//
//void Tank::handle_event(EventsManager::KeyDEvent ev)
//{
//	_isD = ev.f;
//	std::cout << "DDDDDD!" << std::endl;
//	//_head->setRotation(_head->getRotation() + 5);
//}

void Tank::handle_event(EventsManager::KeyEvent ev)
{
    if (ev.key == EventsManager::KeyCode::Up)
    {
        _isUp = (ev.type == EventsManager::KeyType::KeyDown);
    }

    if (ev.key == EventsManager::KeyCode::Down)
    {
        _isDown = (ev.type == EventsManager::KeyType::KeyDown);
    }

    if (ev.key == EventsManager::KeyCode::Left)
    {
        _isLeft = (ev.type == EventsManager::KeyType::KeyDown);
    }

    if (ev.key == EventsManager::KeyCode::Right)
    {
        _isRight = (ev.type == EventsManager::KeyType::KeyDown);
    }

    if (ev.key == EventsManager::KeyCode::A)
    {
        _isA = (ev.type == EventsManager::KeyType::KeyDown);
    }

    if (ev.key == EventsManager::KeyCode::D)
    {
        _isD = (ev.type == EventsManager::KeyType::KeyDown);
    }
}

void Tank::visitSelf()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - _prevTime;
    _prevTime = now;

    if (_isA)
    {
        _head->setRotation(_head->getRotation() - 0.3);
    }
    if (_isD)
    {
        _head->setRotation(_head->getRotation() + 0.3);
    }

    if (_isLeft)
    {
        if (std::abs(_turnSpeed) < 70.0f)
        {
            _turnSpeed += delta.count() * 120.0f + _speed * 0.002f;
        }

        _transform = std::nullopt;
    }
    if (_isRight)
    {
        if (std::abs(_turnSpeed) < 70.0f)
        {
            _turnSpeed -= delta.count() * 120.0f + _speed * 0.002f;
        }

        _transform = std::nullopt;
    }

    if (_turnSpeed > 0.1f)
    {
        _rotation -= delta.count() * _turnSpeed;
        _turnSpeed -= delta.count() * 115.0f;
        _transform = std::nullopt;
        if (_turnSpeed <= -0.1f)
        {
            _turnSpeed = 0.0f;
        }
    }

    if (_turnSpeed < -0.1f)
    {
        _rotation -= delta.count() * _turnSpeed;
        _turnSpeed += delta.count() * 115.0f;
        _transform = std::nullopt;
        if (_turnSpeed >= 0.1f)
        {
            _turnSpeed = 0.0f;
        }
    }

    if (_isUp)
    {
        if (_speed < 500.0f)
        {
            _speed += delta.count() * 460.0f;
        }
    }

    if (_isDown)
    {
        if (_speed > -200.0f)
        {
            _speed -= delta.count() * 460.0f;
        }
    }

    auto vector = glm::rotate(glm::vec2{ 0.0f, -1.0f }, glm::radians(getRotation()));

    if (_speed > 0.1f)
    {
        _position += vector * delta.count() * _speed;
        _speed -= delta.count() * 400.0f;
        _transform = std::nullopt;

        if (_speed <= 0.1f)
        {
            _speed = 0.0f;
        }
    }

    if (_speed < -0.1f)
    {
        _position += vector * delta.count() * _speed;
        _speed += delta.count() * 400.0f;
        _transform = std::nullopt;

        if (_speed >= -0.1f)
        {
            _speed = 0.0f;
        }
    }

    _body->setRotation(_rotation);
}