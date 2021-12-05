#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glm/vec2.hpp>

class Texture
{
public:
    explicit Texture(glm::vec2 size);
    virtual ~Texture() = default;

protected:
    glm::vec2 _size;
};


#endif TEXTURE_HPP