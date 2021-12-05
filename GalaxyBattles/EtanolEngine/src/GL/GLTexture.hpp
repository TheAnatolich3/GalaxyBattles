#ifndef GLTEXTURE_HPP
#define GLTEXTURE_HPP

#include <Bitmap.hpp>
#include <Texture.hpp>

class GLTexture : public Texture
{
public:
    explicit GLTexture(Bitmap bitmap);
    void active();

private:
    uint32_t _id;
};


#endif GLTEXTURE_HPP