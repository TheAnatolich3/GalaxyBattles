#ifndef GLPROGRAM_HPP
#define GLPROGRAM_HPP

#include <ShaderProgram.hpp>
#include <string_view>

class GLProgram : public ShaderProgram
{
public:

    GLProgram(std::string_view vs, std::string_view ps);
    ~GLProgram() override;

    std::shared_ptr<TextureUniform> createTextureUniform(std::string_view name) override;
    std::shared_ptr<Mat3Uniform> createMat3Uniform(std::string_view name) override;
    std::shared_ptr<Vec2Uniform> createVec2Uniform(std::string_view name) override;

    void activate() override;

    uint32_t getProgramId() const { return _program; }

protected:
    uint32_t _vertexShader;
    uint32_t _fragmentShader;
    uint32_t _program;
};

class GLTextureUniform : public TextureUniform
{
public:
    explicit GLTextureUniform(const std::shared_ptr<GLProgram>& program, std::string_view name);
    void activate() override;

private:
    int32_t _location;
};

class GLMat3Uniform : public Mat3Uniform
{
public:
    explicit GLMat3Uniform(const std::shared_ptr<GLProgram>& program, std::string_view name);
    void activate() override;

private:
    int32_t _location;
};

class GLVec2Uniform : public Vec2Uniform
{
public:
    explicit GLVec2Uniform(const std::shared_ptr<GLProgram>& program, std::string_view name);
    void activate() override;

private:
    int32_t _location;
};


#endif GLPROGRAM_HPP