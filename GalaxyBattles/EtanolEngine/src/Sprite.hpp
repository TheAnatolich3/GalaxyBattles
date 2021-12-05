#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <Node.hpp>
#include <vector>
#include <Engine.hpp>

class Engine;
class Texture;
class VertexBuffer;
class ShaderProgram;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class Sprite : public Node
{
public:
	explicit Sprite(const Engine& engine, std::string_view filepath);

protected:
	void visitSelf() override;
	/*void transform(float angle, float scale, float x_offset, float y_offset);
	void add_element(Engine::Triangle tr);

	const Engine::Triangle* get_data() const;
	size_t get_size() const;*/
private:
	const Engine& _engine;
	std::shared_ptr<ShaderProgram> _program;
	std::shared_ptr<VertexBuffer> _vertexBuffer;

	std::shared_ptr<TextureUniform> _textureUniform;
	std::shared_ptr<Vec2Uniform> _screenSizeUniform;
	std::shared_ptr<Mat3Uniform> _transformUniform;
};
#endif SPRITE_HPP