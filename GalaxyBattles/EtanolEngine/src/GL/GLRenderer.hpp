#ifndef GL_RENDERER_HPP
#define GL_RENDERER_HPP
#include <Renderer.hpp>
#include <SDL.h>

class GLRenderer : public Renderer
{
public:
	GLRenderer(const Engine& engine, SDL_Window* window);
	void draw() override;
	std::shared_ptr<VertexBuffer> createVertexBuffer(MeshData data) const override;
	std::shared_ptr<ShaderProgram> createProgram(std::string_view name) const override;
	std::shared_ptr<Texture> createTexture(Bitmap bitmap) const override;
private:
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _drawContext;
};
#endif