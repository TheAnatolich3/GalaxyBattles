#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <string_view>
#include <optional>

class Node : public std::enable_shared_from_this<Node>
{
public:
	virtual ~Node() = default;
	void visit();

	void addNode(std::shared_ptr<Node> node);
	void removeNode(std::shared_ptr<Node> node);
	void removeFromParent();

	Node* getParent();

	const glm::vec2& getPosition() const;
	void setPosition(const glm::vec2& position);

	const glm::vec2& getScale() const;
	void setScale(const glm::vec2& scale);

	const glm::float32 getRotation() const;
	void setRotation(float rotation);

	const glm::vec2& getAnchor() const;
	void setAnchor(const glm::vec2& anchor);

	const glm::vec2& getSize() const;

	glm::mat3 getTransform();
	std::vector<std::shared_ptr<Node>> getChilds();

protected:
	virtual void visitSelf() {};

	Node* _parent;
	std::vector<std::shared_ptr<Node>> _nodes;

	glm::vec2 _position = glm::vec2(0.0f);
	glm::vec2 _scale = glm::vec2(1.0f);
	glm::vec2 _anchor = glm::vec2(0.5f);
	glm::float32 _rotation = 0.0f;
	glm::vec2 _contentSize = glm::vec2(0.0f);
	std::optional<glm::mat3> _transform;
};

#endif NODE_HPP