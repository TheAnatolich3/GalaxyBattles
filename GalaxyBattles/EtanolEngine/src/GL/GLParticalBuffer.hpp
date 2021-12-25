#ifndef GLPARTICAL_BUFFER_HPP
#define GLPARTICAL_BUFFER_HPP

#include <ParticalBuffer.hpp>
#include <vector>

class GLParticalBuffer : public ParticalBuffer
{
public:
	explicit GLParticalBuffer(std::vector<ParticalData> data);
	void draw();

private:
	uint32_t _VAO = 0;
	uint32_t _VBO = 0;

	uint32_t _count;
};
#endif GLPARTICAL_BUFFER_HPP