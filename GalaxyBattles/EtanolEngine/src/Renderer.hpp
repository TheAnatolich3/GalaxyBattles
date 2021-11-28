#pragma once
#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <vector>
#include <string_view>
#include <Engine.hpp>

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void draw() = 0;

    void load_picture(std::vector<Engine::Triangle>);
protected:
    std::vector<Engine::Triangle> _picture;
};

#endif //GAME_RENDERER_HPP
