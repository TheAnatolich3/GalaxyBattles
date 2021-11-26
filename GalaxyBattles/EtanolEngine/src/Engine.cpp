#define TINYOBJLOADER_IMPLEMENTATION
#define GLEW_STATIC
#include <SDL_version.h>
#include <SDL.h>
#include <GL/glew.h>
#include <istream>
//#include <SDL_opengles.h>
#include "Engine.hpp"
#include "windows.h"

void check_errors(std::string file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MERORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: error = "UNKNOWN"; break;
		}
		std::cerr << "GL_" << error << " : 0x" << std::hex << err << std::dec << " - " << file << ": " << line << std::endl;
		err = glGetError();
	}
}

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

GLuint _VAO;
int _uScreenSize;
GLuint _program;

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


void Engine::init(std::string name_window)
{
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	_pmp->window = SDL_CreateWindow(name_window.c_str(), SDL_WINDOWPOS_UNDEFINED ,
								SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	auto context = SDL_GL_CreateContext(_pmp->window);
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		throw std::runtime_error("Glew error");
	}

	_pmp->screen_surface = SDL_GetWindowSurface(_pmp->window);
	_pmp->ren = SDL_CreateRenderer(_pmp->window, -1, SDL_RENDERER_ACCELERATED);

	struct Vertex
	{
		float x, y;
	};

	struct Triangle
	{
		Vertex v1, v2, v3;
	};

	Triangle t{ {700, 100}, {200, 300}, {400, 400} };

	GLuint _VBO, _IBO;
	uint32_t indexes[3] = { 0, 1, 2 };
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	check_errors("Engine.cpp", 79);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), &t, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));
	check_errors("Engine.cpp", 86);


	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	check_errors("Engine.cpp", 92);

	std::ifstream in("../../../../GalaxyBattles/EtanolEngine/src/shader.vert");
	std::ifstream in_frag("../../../../GalaxyBattles/EtanolEngine/src/frag_shader.vert");
	std::string shader_text, frag_shader_text, line;

	if (in.is_open())
	{
		while (!in.eof())
		{
			getline(in, line);
			shader_text += line;
			shader_text += '\n';
		}
	}
	in.close();

	if (in_frag.is_open())
	{
		while (!in_frag.eof())
		{
			getline(in_frag, line);
			frag_shader_text += line;
			frag_shader_text += '\n';
		}
	}
	in_frag.close();

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* s_text = shader_text.c_str();
	glShaderSource(_vertexShader, 1, &s_text, nullptr);
	glCompileShader(_vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(_vertexShader, 512, nullptr, infoLog);
		std::cerr << shader_text << "\nVertex error\n:" << infoLog;
	}

	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* f_text = frag_shader_text.c_str();
	glShaderSource(_fragmentShader, 1, &f_text, nullptr);
	glCompileShader(_fragmentShader);

	
	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(_fragmentShader, 512, nullptr, infoLog);
		std::cerr << frag_shader_text << "\Fragment error\n:" << infoLog;
	}

	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragmentShader);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(_program, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(_program);
	_uScreenSize = glGetUniformLocation(_program, "screenSize");
}

void Engine::update() {
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT) {
		is_active = false;
	}
	

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_program);
	glUniform2f(_uScreenSize, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

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