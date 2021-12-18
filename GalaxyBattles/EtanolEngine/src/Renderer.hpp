#pragma once
#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <Engine.hpp>
#include <MeshData.hpp>
#include <Bitmap.hpp>
#include <optional>
#include <glm/glm.hpp>

class VertexBuffer;
class ShaderProgram;
class Texture;

class Renderer
{
public:
    virtual ~Renderer() = default;

    struct sub_params
    {
        int _count = 0;
        int _offset = 0;
    };


    struct Command
    {
        std::shared_ptr<VertexBuffer>  vertexBuffer;
        std::shared_ptr<ShaderProgram> program;
        std::optional<sub_params> _ren;
        std::optional<glm::vec4> _scissor;
    };

    void addCommand(Command command) const
    {
        _commands.push_back(std::move(command));
    }

    virtual void draw(int count, int pos) = 0;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(MeshData data) const = 0;
    virtual std::shared_ptr<ShaderProgram> createProgram(std::string_view name) const = 0;
    virtual std::shared_ptr<Texture> createTexture(Bitmap bitmap) const = 0;
    void load_picture(std::vector<Engine::Triangle>);
protected:
    mutable std::vector<Command> _commands;
    
    std::vector<Engine::Triangle> _picture;
};

#endif //GAME_RENDERER_HPP
