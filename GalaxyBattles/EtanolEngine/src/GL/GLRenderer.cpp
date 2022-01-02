#include <Engine.hpp>
#include <string_view>
#include <GL/GLVertexBuffer.hpp>
#include <GL/GLDrawProgram.hpp>
#include <GL/GLTexture.hpp>
#include <GL/GLParticalBuffer.hpp>
#include <GL/glew.h>
#include <stdexcept>
#include "GLRenderer.hpp"

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

void GLRenderer::draw()
{
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_CULL_FACE);
    glClearColor(0.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);

	for (const auto& command : _commands)
	{
		auto glVertexBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(command.vertexBuffer);

		if (glVertexBuffer)
		{
			auto glProgram = std::dynamic_pointer_cast<GLProgram>(command.program);
			if (glProgram)
			{
				glProgram->activate();
				if (command.scissor)
				{
					glScissor(
						static_cast<GLint>(command.scissor->x),
						static_cast<GLint>(_engine.get_window_height() - command.scissor->w),
						static_cast<GLsizei>(command.scissor->z - command.scissor->x),
						static_cast<GLsizei>(command.scissor->w - command.scissor->y));
				}
				else
				{
					glScissor(
						0, 
						0, 
						static_cast<GLsizei>(_engine.get_window_width()), 
						static_cast<GLsizei>(_engine.get_window_height()));
				}

				if (command.ren)
				{
					glVertexBuffer->draw(command.ren->count, command.ren->offset);
				}
				else
				{
					glVertexBuffer->draw(6, 0);
				}
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

std::shared_ptr<ParticalBuffer> GLRenderer::createrParticalBuffer(std::vector<ParticalBuffer::ParticalData> data)
{
	return std::make_shared<GLParticalBuffer>(std::move(data));
}