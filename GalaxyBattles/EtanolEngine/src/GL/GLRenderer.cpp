#include <string>
#include <iostream>
#include <Bitmap.hpp>
#include <GL/GLVertexBuffer.hpp>
#include <GL/GLDrawProgram.hpp>
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


GLRenderer::GLRenderer(const Engine& engine, SDL_Window* window)
    : _engine(engine)
    , _drawContext{ SDL_GL_CreateContext(window), SDL_GL_DeleteContext }
{
    printf("OpenGL version supported by this platform (%s): \n",
        glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        throw std::runtime_error("Glew error");
    }

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void GLRenderer::draw(int count, int pos)
{
	glDisable(GL_CULL_FACE);
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& command : _commands)
	{
		auto glVertexBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(command.vertexBuffer);

		if (glVertexBuffer)
		{
			auto glProgram = std::dynamic_pointer_cast<GLProgram>(command.program);
			if (glProgram)
			{
				glProgram->activate();
				glVertexBuffer->draw(count, pos);
			}
		}
	}
	_commands.clear();
}


std::shared_ptr<VertexBuffer> GLRenderer::createVertexBuffer(MeshData data) const
{
	return std::make_shared<GLVertexBuffer>(_engine, data);
}

std::shared_ptr<ShaderProgram> GLRenderer::createProgram(std::string_view name) const
{
	if (name == "draw")
	{
		return std::make_shared<GLDrawProgram>();
	}

	return nullptr;
}

std::shared_ptr<Texture> GLRenderer::createTexture(Bitmap bitmap) const
{
	return std::make_shared<GLTexture>(std::move(bitmap));
}