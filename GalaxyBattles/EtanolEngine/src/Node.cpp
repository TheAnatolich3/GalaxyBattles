#include "Node.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

void Node::addNode(std::shared_ptr<Node> node)
{
	if (node != nullptr)
	{
		node->_parent = this;
		_nodes.push_back(std::move(node));
	}
}

void Node::removeNode(std::shared_ptr<Node> node)
{
	auto it = std::find(_nodes.begin(), _nodes.end(), node);
	_nodes.erase(it);
}

void Node::removeFromParent()
{
	_parent->removeNode(shared_from_this());
}

Node* Node::getParent()
{
	return _parent;
}

void Node::visit()
{
	this->visitSelf();
	for (auto node : _nodes)
	{
		node->visit();
	}
}

const glm::vec2& Node::getPosition() const
{
	return _position;
}

void Node::setPosition(const glm::vec2& position)
{
	_transform = std::nullopt;
	_position = position;
}

const glm::vec2& Node::getScale() const
{
	return _scale;
}

void Node::setScale(const glm::vec2& scale)
{
	_transform = std::nullopt;
	_scale = scale;
}

const glm::float32 Node::getRotation() const
{
	return _rotation;
}

void Node::setRotation(float rotation)
{
	_transform = std::nullopt;
	_rotation = rotation;
}

const glm::vec2& Node::getAnchor() const
{
	return _anchor;
}

void Node::setAnchor(const glm::vec2& anchor)
{
	_transform = std::nullopt;
	_anchor = anchor;
}

glm::mat3 Node::getTransform()
{
	if (_transform)
	{
		return _parent ? (getParent()->getTransform() * *_transform) : *_transform;
	}
	else
	{
		glm::mat3 trans(1.0f);

		trans = glm::translate(trans, _position);
		trans = glm::scale(trans, _scale);
		trans = glm::rotate(trans, glm::radians(_rotation));

		trans = glm::translate(trans, -glm::vec2(_anchor.x * _contentSize.x,
			_anchor.y * _contentSize.y));

		_transform = trans;
		return _parent ? (getParent()->getTransform() * trans) : trans;
	}
}

const glm::vec2& Node::getSize() const
{
	return _contentSize;
}

std::vector<std::shared_ptr<Node>> Node::getChilds()
{
	return _nodes;
}