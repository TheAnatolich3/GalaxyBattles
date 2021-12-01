#include <fstream>
#include <string>
#include <iostream>
#include "Sprite.hpp"
#include "GLRenderer.hpp"

void check_errors(std::string_view file, int line)
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

void GLRenderer::read_file(const std::string_view file_name, std::string& text)
{
	std::string line;
	std::ifstream in(file_name.data());

	if (in.is_open())
	{
		while (!in.eof())
		{
			getline(in, line);
			text += line;
			text += '\n';
		}
	}
	in.close();
}

void GLRenderer::gen_shader(GLuint id, std::string shader_text)
{
	const char* s_text = shader_text.c_str();
	glShaderSource(id, 1, &s_text, nullptr);
	glCompileShader(id);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cerr << shader_text << "\nVertex error\n:" << infoLog;
	}
}

GLRenderer::GLRenderer(const Engine& engine, SDL_Window* window)
    : _engine(engine)
    , _drawContext{ SDL_GL_CreateContext(window), SDL_GL_DeleteContext }
{
    printf("OpenGL version supported by this platform (%s): \n",
        glGetString(GL_VERSION));

    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        throw std::runtime_error("Glew error");
    }

	/*struct VertexTriangle
	{
		Engine::Vertex arr[3];
	};

	VertexTriangle t {700, 100, 200, 300, 400, 400 };*/

	Sprite spr;
	spr.add_element(Engine::Triangle{ 200, 200, 1, 0, 0, 400, 200, 0, 1, 0, 200, 400, 0, 0, 1 });
	spr.add_element(Engine::Triangle{ 400, 400, 1, 0, 0, 400, 200, 0, 1, 0, 200, 400, 0, 0, 1 });
	//spr.add_element(Engine::Triangle{ 200, 400, 1, 0, 0, 400, 400, 0, 1, 0, 300, 500, 0, 0, 1 });

	unsigned int indices[] = { 0, 1, 2, 1, 2, 3 };
	//spr.transform(1, 1, 300, 0);
	GLuint _EBO, _VBO;
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	check_errors("GL_Renderer.cpp", 54);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	_el_cnt = spr.get_size();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Engine::Triangle) * _el_cnt, spr.get_data(), GL_STATIC_DRAW);


	/*float vertices[] = {
		// координаты  // цвета
		700.0f, 100.0f,   1.0f, 0.0f, 0.0f,   // нижн€€ права€ вершина
	    200.0f, 300.0f,   0.0f, 1.0f, 0.0f,   // нижн€€ лева€ вершина
		400.0f, 400.0f,   0.0f, 0.0f, 1.0f    // верхн€€ вершина
	};*/

	/*GLuint _VBO;
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	check_errors("GL_Renderer.cpp", 54);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	/* оординаты*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	// ÷ветовые атрибуты
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	
	check_errors("GL_Renderer.cpp", 61);

	check_errors("GL_Renderer.cpp", 67);

	std::string shader_text, frag_shader_text;
	read_file("../../../../GalaxyBattles/EtanolEngine/src/shader.vert", shader_text);
	read_file("../../../../GalaxyBattles/EtanolEngine/src/frag_shader.vert", frag_shader_text);

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	gen_shader(_vertexShader, shader_text);
	gen_shader(_fragmentShader, frag_shader_text);

	_program = glCreateProgram();
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragmentShader);
	glLinkProgram(_program);

	GLint success;
	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(_program, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(_program);
	_uScreenSize = glGetUniformLocation(_program, "screenSize"); 
}

void GLRenderer::draw()
{
	glDisable(GL_CULL_FACE);
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_program);
	glUniform2f(_uScreenSize, _engine.get_window_width(), _engine.get_window_height());

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3 * _el_cnt, GL_UNSIGNED_INT, 0);
}
