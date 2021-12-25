#ifndef PARTICAL_BUFFER_HPP
#define PARTICAL_BUFFER_HPP

#include <glm/glm.hpp>
#include <VertexBuffer.hpp>

class ParticalBuffer : public VertexBuffer
{
public:
	struct ParticalData
	{
		glm::vec2 pos;
		glm::vec2 velocity;
		
		color colour;
	};
	
	~ParticalBuffer() = default;
};
#endif PARTICAL_BUFFER_HPP// ! PARTICAL_BUFFER_HPP