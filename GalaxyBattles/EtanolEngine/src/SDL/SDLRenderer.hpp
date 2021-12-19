#ifndef GAME_SDLRENDERER_HPP
#define GAME_SDLRENDERER_HPP
#include <Renderer.hpp>
#include <memory>
#include <string_view>

class Bitmap;
class SDL_Window;
class SDL_Renderer;

class SDLRenderer final : public Renderer
{
public:
    struct Point
    {
        int x, y;
    };

    explicit SDLRenderer(const Engine& engine, std::shared_ptr<SDL_Window> sdlWindow);
    std::shared_ptr<VertexBuffer> createVertexBuffer(MeshData data) const override;
    std::shared_ptr<ShaderProgram> createProgram(std::string_view name) const override;
    std::shared_ptr<Texture> createTexture(Bitmap bitmap) const override;

    void draw() override;
private:
    std::shared_ptr<SDL_Window> _sdlWindow;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _sdlRenderer;
    const Engine& _engine;

    void draw_line(int x0, int y0, int x1, int y1);
    void draw_triangle(Point t0, Point t1, Point t2);
    void fill_triangle(int y_start, int y_end, int a1, int b1, int c1, int a2, int b2, int c2);
};


#endif GAME_SDLRENDERER_HPP
