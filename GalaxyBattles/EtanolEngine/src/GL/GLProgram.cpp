#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/GLTexture.hpp>
#include "GLProgram.hpp"

GLProgram::GLProgram(std::string_view vs_str, std::string_view ps_str)
{
    const char* vs= vs_str.data();
    const char* ps = ps_str.data();
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader, 1, &vs, nullptr);
    glCompileShader(_vertexShader);

    GLint success;
    GLchar infoLog[2048];
    glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(_vertexShader, 2048, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &ps, nullptr);
    glCompileShader(_fragmentShader);

    glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(_fragmentShader, 2048, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _program = glCreateProgram();

    glAttachShader(_program, _vertexShader);
    glAttachShader(_program, _fragmentShader);
    glLinkProgram(_program);

    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_program, 2048, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &count);
    printf("Active Attributes: %d\n", count);

    for (i = 0; i < _program; i++)
    {
        glGetActiveAttrib(_program, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
    }

    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(_program, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
}

GLProgram::~GLProgram()
{
    glDeleteProgram(_program);
    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);
}


void GLProgram::activate()
{
    glUseProgram(_program);
    ShaderProgram::activate();
}

std::shared_ptr<TextureUniform> GLProgram::createTextureUniform(std::string_view name)
{
    auto uniform = std::make_shared<GLTextureUniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

std::shared_ptr<Mat3Uniform> GLProgram::createMat3Uniform(std::string_view name)
{
    auto uniform = std::make_shared<GLMat3Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

std::shared_ptr<Vec2Uniform> GLProgram::createVec2Uniform(std::string_view name)
{
    auto uniform = std::make_shared<GLVec2Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

void GLTextureUniform::activate()
{
    auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);
    if (glTexture)
    {
        //TODO: diff texture slots
        glActiveTexture(GL_TEXTURE0);
        glTexture->active();

        glUniform1i(_location, 0);
    }
}

GLTextureUniform::GLTextureUniform(const std::shared_ptr<GLProgram>& program, std::string_view name)
{
    _location = glGetUniformLocation(program->getProgramId(), name.data());
}

GLMat3Uniform::GLMat3Uniform(const std::shared_ptr<GLProgram>& program, std::string_view name)
{
    _location = glGetUniformLocation(program->getProgramId(), name.data());
}

void GLMat3Uniform::activate()
{
    glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
}

GLVec2Uniform::GLVec2Uniform(const std::shared_ptr<GLProgram>& program, std::string_view name)
{
    _location = glGetUniformLocation(program->getProgramId(), name.data());
}

void GLVec2Uniform::activate()
{
    glUniform2f(_location, value.x, value.y);
}