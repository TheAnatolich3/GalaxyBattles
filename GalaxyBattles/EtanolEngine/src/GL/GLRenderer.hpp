#ifndef GL_RENDERER_HPP
#define GL_RENDERER_HPP
#include <Renderer.hpp>
#include <SDL.h>
#include <GL/glew.h>
class SDL_Window;

class GLRenderer : public Renderer
{
public:
	GLRenderer(const Engine& engine, SDL_Window* window);
	void draw() override;
private:
	GLuint _VAO, _program;
	int _uScreenSize;
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _drawContext;

	void read_file(const std::string_view file_name, std::string& text);
	void gen_shader(GLuint id, std::string shader_text);
};
#endif