#include "Sprite.hpp"
#include <Bitmap.hpp>

#include <Engine.hpp>
#include <Renderer.hpp>
#include <Window.hpp>
#include <ShaderProgram.hpp>
#include <MeshData.hpp>

Sprite::Sprite(const Engine& engine, std::string_view filepath) : _engine(engine)
{
	Bitmap bitmap(filepath);

	_contentSize = bitmap.getSize();

	MeshData data;

    data.vertices.emplace_back();
    data.vertices.back().position = { 0.0, 0.0 };
    data.vertices.back().texcoord = { 0.0, 0.0 };
   // data.vertices.back().color = glm::vec4{ 0.0, 0.0 , 1.0, 1.0};

    data.vertices.emplace_back();
    data.vertices.back().position = { 0.0, bitmap.getSize().y};
    data.vertices.back().texcoord = { 0.0, 1.0 };

    data.vertices.emplace_back();
    data.vertices.back().position = { bitmap.getSize().x, bitmap.getSize().y };
    data.vertices.back().texcoord = { 1.0, 1.0 };

    data.vertices.emplace_back();
    data.vertices.back().position = { bitmap.getSize().x, 0.0 };
    data.vertices.back().texcoord = { 1.0, 0.0 };

    data.indexes.emplace_back(0);
    data.indexes.emplace_back(2);
    data.indexes.emplace_back(3);

    data.indexes.emplace_back(0);
    data.indexes.emplace_back(1);
    data.indexes.emplace_back(2);

    _vertexBuffer = engine.renderer().createVertexBuffer(std::move(data));
    _program = engine.renderer().createProgram("draw");

    _textureUniform = _program->createTextureUniform("uTexture");
    _textureUniform->texture = engine.renderer().createTexture(std::move(bitmap));

    _screenSizeUniform = _program->createVec2Uniform("uScreenSize");
    _transformUniform = _program->createMat3Uniform("uTransform");
}

void Sprite::visitSelf()
{
    const auto& win = _engine.window();
    _screenSizeUniform->value.x = win.getWidth();
    _screenSizeUniform->value.y = win.getHeight();

    _transformUniform->value = this->getTransform();

    Renderer::Command command;
    command.program = _program;
    command.vertexBuffer = _vertexBuffer;
    _engine.renderer().addCommand(std::move(command));
}