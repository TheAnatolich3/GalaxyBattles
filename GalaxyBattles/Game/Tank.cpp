#include <glm/gtx/rotate_vector.hpp>
#include <Sound.hpp>
#include <Engine.hpp>
#include <AudioManager.hpp>
#include <Sprite.hpp>
#include "Tank.hpp"


Tank::Tank(const Engine& engine)
{
	_body = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_body_removed_back.png");
	_head = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_head_removed_back.png");
    this->setPosition(glm::vec2(engine.get_window_width() * 0.2f,
		engine.get_window_height() * 0.5f));
    this->setAnchor(glm::vec2(0.5f));
	_head->setScale(glm::vec2(0.6f, 0.6f));

	glm::vec2 tank_body_size = _body->getSize();

	_head->setPosition(glm::vec2(tank_body_size.x * 0.53, tank_body_size.y * 0.5));

	_head->setAnchor(glm::vec2(0.368f, 0.5f));
	_body->addNode(_head);
	this->addNode(_body);

    _shot = engine.audioManager().createSound("../../../../GalaxyBattles/EtanolEngine/resource/shot_ev.wav", false, 100);
    engine.eventsManager().add_delegate(this);
}

void Tank::handle_event(EventsManager::QuitEvent)
{

}

Tank::~Tank()
{
    _shot->stop();
}

void Tank::handle_event(EventsManager::KeyEvent ev)
{
    if (ev.key == EventsManager::KeyCode::Up)
    {
        _isUp = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::Down)
    {
        _isDown = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::Left)
    {
        _isLeft = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::Right)
    {
        _isRight = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::A)
    {
        _isA = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::D)
    {
        _isD = (ev.type == EventsManager::Action::Down);
    }

    if (ev.key == EventsManager::KeyCode::Space && ev.type == EventsManager::Action::Down)
    {
        _shot->stop();
        _shot->play();
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

    auto vector = glm::rotate(glm::vec2{ 1.0f, 0.0f }, glm::radians(getRotation()));

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

    setRotation(_rotation);
}
