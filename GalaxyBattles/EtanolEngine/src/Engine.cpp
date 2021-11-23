#define TINYOBJLOADER_IMPLEMENTATION
#define GLEW_STATIC
#include <SDL_version.h>
#include <SDL.h>
#include <GL/glew.h>
//#include <SDL_opengles.h>
#include "Engine.hpp"
#include "windows.h"

class Render {
public:
	Render();
	Render(SDL_Renderer* ren_);

	void draw_picture(const std::string& file_name);
private:
	struct Point {
		int x;
		int y;
	};

	void read_file(const std::string& file_name, tinyobj::ObjReader& reader);
	void draw_line(int x0, int y0, int x1, int y1);
	void draw_triangle(Point t0, Point t1, Point t2);

	void fill_triangle(int y_start, int y_end, int a1, int b1, int c1, int a2, int b2, int c2);
	SDL_Renderer* ren;
};

Render::Render() {
	ren = nullptr;
}

Render::Render(SDL_Renderer* ren_) {
	ren = ren_;
}


struct Engine::Engine_pimp {
	SDL_Surface* screen_surface;
	SDL_Window* window;
	SDL_Renderer* ren;
};


Engine::Engine() {
	_pmp = std::make_unique<Engine::Engine_pimp>();
	_pmp->screen_surface = NULL;
	_pmp->window = NULL;
	_pmp->ren = NULL;
	is_active = true;
}

Engine::~Engine() = default;

bool Engine::isActive() {
	return is_active;
}

void Engine::init(std::string name_window) {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	_pmp->window = SDL_CreateWindow(name_window.c_str(), SDL_WINDOWPOS_UNDEFINED ,
								SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	auto context = SDL_GL_CreateContext(_pmp->window);
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		throw std::runtime_error("Glew error");
	}

	_pmp->screen_surface = SDL_GetWindowSurface(_pmp->window);
	_pmp->ren = SDL_CreateRenderer(_pmp->window, -1, SDL_RENDERER_ACCELERATED);
}

void Engine::update() {
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
		is_active = false;
	}
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(_pmp->window);
}

void Render::read_file(const std::string& file_name, tinyobj::ObjReader& reader) {
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./";

	if (!reader.ParseFromFile(file_name, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}
	
	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
}

void Render::draw_picture(const std::string& file_name) {
	tinyobj::ObjReader reader;
	this->read_file(file_name, reader);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			Point triangle_vertices[3];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {

				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];

				int x0 = static_cast<int>((vx + 1.) * SCREEN_WIDTH / 2.);
				int y0 = static_cast<int>((vy + 1.) * SCREEN_HEIGHT / 2.);

				triangle_vertices[v].x = SCREEN_WIDTH - x0;
				triangle_vertices[v].y = SCREEN_HEIGHT - y0;
			}

			draw_triangle(triangle_vertices[0], triangle_vertices[1], triangle_vertices[2]);
			index_offset += fv;
		}
	}
	SDL_RenderPresent(ren);
}

void Engine::draw_figure(const std::string& file_name)
{
	Render ren = Render(_pmp->ren);
	ren.draw_picture(file_name);
}

void Render::draw_line(int x0, int y0, int x1, int y1) {
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
			SDL_RenderDrawPoint(ren, y, x);
		}
		else {
			SDL_RenderDrawPoint(ren, x, y);
		}
		error2 += derror2;

		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Render::draw_triangle(Point t0, Point t1, Point t2) {
	SDL_SetRenderDrawColor(ren, rand() % 256, rand() % 256, rand() % 256, rand() % 256);
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


void Render::fill_triangle(int y_start, int y_end, int a1, int b1, int c1, int a2, int b2, int c2) {
	for (int y = y_start; y <= y_end; ++y) {
		int x_start = -(b1 * y + c1) / a1;
		int x_end = -(b2 * y + c2) / a2;
		if (x_start > x_end) {
			std::swap(x_start, x_end);
		}
		for (int j = x_start; j <= x_end; ++j) {
			SDL_RenderDrawPoint(ren, j, y);
		}
	}
}