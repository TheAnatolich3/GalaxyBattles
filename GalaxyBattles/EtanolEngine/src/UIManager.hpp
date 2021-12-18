#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <memory>

class Engine;
class Texture;
class TextureUniform;
class Vec2Uniform;
class ShaderProgram;
class Renderer;
class Mat3Uniform;

class UIManager final
{
public:
	UIManager(const Engine& engine);

	void visit();
private:
	const Engine& _engine;
	//std::shared_ptr<Texture> _tex;

	std::shared_ptr<ShaderProgram> _program;
	std::shared_ptr<TextureUniform> _textureUniform;
	std::shared_ptr<Vec2Uniform> _screenSizeUniform;
	std::shared_ptr<Mat3Uniform> _transformUniform;
};

#endif UIMANAGER_HPP

