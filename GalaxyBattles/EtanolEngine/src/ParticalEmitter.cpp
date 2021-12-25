#include <Engine.hpp>
#include <random> 
#include "ParticalEmitter.hpp"

/*ParticalEmitter::ParticalEmitter(const Engine& engine, size_t count, glm::vec2 startPos)
	: Node(engine)
{
	std::vector<ParticalBuffer::ParticalData> particles;

	particles.reserve(count);

	auto startpos = (getTransform() * glm::vec3(1.0)).xy();

	std::mt19937 gen();
	std::uniform_real_distribution<float> angleGen(0, 360);
	std::uniform_real_distribution<float> velGen(1, 5);

	for (size_t i = 0; i < count; ++i)
	{
		ParticalBuffer::ParticalData partical;
		partical.pos = startPos;
	}
}*/