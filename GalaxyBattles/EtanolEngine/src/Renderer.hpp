#pragma once
#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <vector>
#include <optional>
#include <memory>
#include <MeshData.hpp>
#include <Bitmap.hpp>
#include <glm/glm.hpp>

class VertexBuffer;
class ShaderProgram;
class Texture;
class Uniform;

class Renderer
{
public:
    virtual ~Renderer() = default;

    struct Command
    {
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<ShaderProgram> program;
        std::vector<std::shared_ptr<Uniform>> uniforms;

        std::optional<glm::vec4> scissor;

        struct sub_t
        {
            int count = 0;
            int offset = 0;
        };

        std::optional<sub_t> ren;
    };

    void addCommand(Command command) const
    {
        _commands.push_back(std::move(command));
    }

    virtual void draw() = 0;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(MeshData data) const = 0;
    virtual std::shared_ptr<ShaderProgram> createProgram(std::string_view name) const = 0;
    virtual std::shared_ptr<Texture> createTexture(Bitmap bitmap) const = 0;
protected:
    mutable std::vector<Command> _commands;
};

#endif GAME_RENDERER_HPP
