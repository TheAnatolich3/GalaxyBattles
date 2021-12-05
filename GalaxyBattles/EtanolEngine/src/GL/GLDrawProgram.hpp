#ifndef GAME_GLDRAWPROGRAM_HPP
#define GAME_GLDRAWPROGRAM_HPP

#include <GL/GLProgram.hpp>
#include <GL/GLTexture.hpp>

#include <EventsManager.hpp>

#include <cstdint>

class Engine;

class GLDrawProgram final : public GLProgram
{
public:
    explicit GLDrawProgram();
};


#endif GAME_GLDRAWPROGRAM_HPP