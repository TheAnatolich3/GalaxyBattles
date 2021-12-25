#include <GL/glew.h>
#include "GLParticalBuffer.hpp"


GLParticalBuffer::GLParticalBuffer(std::vector<ParticalData> data)
{
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(ParticalData), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticalData), (void*)offsetof(ParticalData, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticalData), (void*)offsetof(ParticalData, velocity));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4,
        GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ParticalData), (void*)offsetof(ParticalData, colour));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _count = data.size();
}

void GLParticalBuffer::draw()
{
    glBindVertexArray(_VAO);
    glDrawArrays(GL_POINTS, 0, _count);
}