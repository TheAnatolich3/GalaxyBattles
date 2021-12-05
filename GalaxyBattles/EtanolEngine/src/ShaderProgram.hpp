#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <memory>
#include <vector>
#include <string_view>

#include <glm/glm.hpp>

class Texture;

class Uniform
{
public:
    virtual void activate() = 0;
    virtual ~Uniform() = default;
};

class TextureUniform : public Uniform
{
public:
    std::shared_ptr<Texture> texture;
};

class Mat3Uniform : public Uniform
{
public:
    glm::mat3 value;
};

class Vec2Uniform : public Uniform
{
public:
    glm::vec2 value;
};

class ShaderProgram : public std::enable_shared_from_this<ShaderProgram>
{
public: 
	virtual ~ShaderProgram() = default;
	virtual void activate();
   
    virtual std::shared_ptr<TextureUniform> createTextureUniform(std::string_view name) = 0;
    virtual std::shared_ptr<Mat3Uniform> createMat3Uniform(std::string_view name) = 0;
    virtual std::shared_ptr<Vec2Uniform> createVec2Uniform(std::string_view name) = 0;

protected:
    std::vector<std::shared_ptr<Uniform>> _uniforms;
};

inline void ShaderProgram::activate()
{
    for (const auto& uniform : _uniforms)
    {
        uniform->activate();
    }
}

#endif SHADERPROGRAM_HPP
