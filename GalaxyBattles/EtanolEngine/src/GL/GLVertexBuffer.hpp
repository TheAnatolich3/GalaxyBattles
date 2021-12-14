#ifndef GLVERTEXBUFFER_HPP
#define GLVERTEXBUFFER_HPP

#include <VertexBuffer.hpp>
#include <MeshData.hpp>

class Engine;
class GLVertexBuffer final : public VertexBuffer
{
public:
    explicit GLVertexBuffer(const Engine& engine, MeshData data);

    virtual ~GLVertexBuffer();

    void draw(int count, int pos);
private:
    uint32_t _VAO = 0;
    uint32_t _VBO = 0;
    uint32_t _IBO = 0;

    uint32_t _count;

    const Engine& _engine;
};

#endif GLVERTEXBUFFER_HPP