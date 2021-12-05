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


//void Sprite::add_element(Engine::Triangle tr)
//{
//	_data.push_back(tr);
//}
//
//void Sprite::transform(float angle, float scale, float x_offset, float y_offset)
//{
//	for (Engine::Triangle& tr : _data)
//	{
//		tr.v1.x = scale * cos(angle) * tr.v1.x - sin(angle)*tr.v1.y + x_offset;
//		tr.v1.y = sin(angle) * tr.v1.x + scale * tr.v1.y * cos(angle) + y_offset;
//
//		tr.v2.x = scale * cos(angle) * tr.v2.x - sin(angle) * tr.v2.y + x_offset;
//		tr.v2.y = sin(angle) * tr.v2.x + scale * tr.v2.y * cos(angle) + y_offset;
//
//		tr.v3.x = scale * cos(angle) * tr.v3.x - sin(angle) * tr.v3.y + x_offset;
//		tr.v3.y = sin(angle) * tr.v3.x + scale * tr.v3.y * cos(angle) + y_offset;
//	}
//}


//
// const Engine::Triangle* Sprite::get_data() const
//{
//	return _data.data();
//}
//
//size_t Sprite::get_size() const
//{
//	return _data.size();
//}