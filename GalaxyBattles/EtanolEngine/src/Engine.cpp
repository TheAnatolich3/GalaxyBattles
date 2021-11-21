#define TINYOBJLOADER_IMPLEMENTATION
#include <SDL_version.h>
#include <SDL.h>

#include "Engine.hpp"
#include "windows.h"

struct Engine::Pimp {
	SDL_Surface* screen_surface;
	SDL_Window* window;
	SDL_Renderer* ren;
};

Engine::Engine() {
	_pmp = std::make_unique<Engine::Pimp>();
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
	_pmp->window = SDL_CreateWindow(name_window.c_str(), SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN);

	_pmp->screen_surface = SDL_GetWindowSurface(_pmp->window);
	_pmp->ren = SDL_CreateRenderer(_pmp->window, -1, SDL_RENDERER_ACCELERATED);
}

void Engine::update() {
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
		is_active = false;
	}
}

void Engine::read_file(const std::string& file_name, tinyobj::ObjReader& reader) {
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

void Engine::draw_figure(const std::string& file_name)
{
	tinyobj::ObjReader reader;
	this->read_file(file_name, reader);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	SDL_SetRenderDrawColor(_pmp->ren, 0xFF, 0xFF, 0x00, 0x00);

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				size_t v_next = (v + 1) % 3;

				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::index_t idx_next = shapes[s].mesh.indices[index_offset + v_next];

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];

				tinyobj::real_t vx_next = attrib.vertices[3 * size_t(idx_next.vertex_index) + 0];
				tinyobj::real_t vy_next = attrib.vertices[3 * size_t(idx_next.vertex_index) + 1];

				int x0 = (vx + 1.) * SCREEN_WIDTH / 2.;
				int y0 = (vy + 1.) * SCREEN_HEIGHT / 2.;
				int x1 = (vx_next + 1.) * SCREEN_WIDTH / 2.;
				int y1 = (vy_next + 1.) * SCREEN_HEIGHT / 2.;

				draw_line(SCREEN_WIDTH - x0, SCREEN_HEIGHT - y0, SCREEN_WIDTH - x1, SCREEN_HEIGHT - y1);
			}
			index_offset += fv;
		}
	}
	SDL_RenderPresent(_pmp->ren);
}

void Engine::draw_line(int x0, int y0, int x1, int y1) {
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
			SDL_RenderDrawPoint(_pmp->ren, y, x);
		}
		else {
			SDL_RenderDrawPoint(_pmp->ren, x, y);
		}
		error2 += derror2;

		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}