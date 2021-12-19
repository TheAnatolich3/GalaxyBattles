#include <SDL.h>
#include <Bitmap.hpp>
#include "SdlRenderer.hpp"

SDLRenderer::SDLRenderer(const Engine& engine, std::shared_ptr<SDL_Window> sdlWindow)
    : _engine{ engine }
    , _sdlWindow(std::move(sdlWindow))
    , _sdlRenderer{ SDL_CreateRenderer(_sdlWindow.get(), -1, 0), SDL_DestroyRenderer }
{
}

void SDLRenderer::draw()
{
	int screen_width = static_cast<int>(_engine.get_window_width());
	int screen_height = static_cast<int>(_engine.get_window_height());
    for (auto& tr : _picture)
    {
		int x0 = screen_width - static_cast<int>((tr.v1.x + 1.) * screen_width / 2.);
		int y0 = screen_height- static_cast<int>((tr.v1.y + 1.) * screen_height / 2.);
		int x1 = screen_width - static_cast<int>((tr.v2.x + 1.) * screen_width / 2.);
		int y1 = screen_height - static_cast<int>((tr.v2.y + 1.) * screen_height / 2.);
		int x2 = screen_width - static_cast<int>((tr.v3.x + 1.) * screen_width / 2.);
		int y2 = screen_height - static_cast<int>((tr.v3.y + 1.) * screen_height / 2.);
		draw_triangle({ x0,y0 }, { x1,y1 }, { x2,y2 });
    }
	SDL_RenderPresent(_sdlRenderer.get());
}

void SDLRenderer::draw_line(int x0, int y0, int x1, int y1) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			SDL_RenderDrawPoint(_sdlRenderer.get(), y, x);
		}
		else {
			SDL_RenderDrawPoint(_sdlRenderer.get(), x, y);
		}
		error2 += derror2;

		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void SDLRenderer::draw_triangle(Point t0, Point t1, Point t2) {
	SDL_SetRenderDrawColor(_sdlRenderer.get(), rand() % 256, rand() % 256, rand() % 256, rand() % 256);
	// sort point
	if (t0.y > t1.y) {
		std::swap(t0, t1);
	}
	if (t0.y > t2.y) {
		std::swap(t0, t2);
	}
	if (t1.y > t2.y) {
		std::swap(t1, t2);
	}

	//calc koef lines
	int a1 = t1.y - t0.y;
	int b1 = t0.x - t1.x;
	int c1 = t1.x * t0.y - t1.y * t0.x;

	int a2 = t2.y - t0.y;
	int b2 = t0.x - t2.x;
	int c2 = t2.x * t0.y - t2.y * t0.x;
	if (a1 == 0) {
		draw_line(t0.x, t0.y, t1.x, t1.y);
	}
	else {
		fill_triangle(t0.y, t1.y, a1, b1, c1, a2, b2, c2);
	}

	a1 = t1.y - t2.y;
	b1 = t2.x - t1.x;
	c1 = t1.x * t2.y - t1.y * t2.x;
	if (a1 == 0) {
		draw_line(t1.x, t1.y, t2.x, t2.y);
	}
	else
	{
		fill_triangle(t1.y, t2.y, a1, b1, c1, a2, b2, c2);
	}
}


void SDLRenderer::fill_triangle(int y_start, int y_end, int a1, int b1, int c1, int a2, int b2, int c2) {
	for (int y = y_start; y <= y_end; ++y) {
		int x_start = -(b1 * y + c1) / a1;
		int x_end = -(b2 * y + c2) / a2;
		if (x_start > x_end) {
			std::swap(x_start, x_end);
		}
		for (int j = x_start; j <= x_end; ++j) {
			SDL_RenderDrawPoint(_sdlRenderer.get(), j, y);
		}
	}
}

std::shared_ptr<VertexBuffer> SDLRenderer::createVertexBuffer(MeshData data) const
{
	return nullptr;
}

std::shared_ptr<ShaderProgram> SDLRenderer::createProgram(std::string_view name) const
{
	return nullptr;
}

std::shared_ptr<Texture> SDLRenderer::createTexture(Bitmap bitmap) const
{
	return nullptr;
}

