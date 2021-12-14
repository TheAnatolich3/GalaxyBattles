#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP
#include <glm/glm.hpp>


class VertexBuffer
{
public:
    struct color
    {
        explicit color(std::uint32_t rgba) : _rgba(rgba) {}
        color(glm::vec4 color)
        {
            this->setColor(color);
        }

        glm::vec4 getColor() const
        {
            std::uint32_t r = (_rgba & 0x000000FF) >> 0;
            std::uint32_t g = (_rgba & 0x0000FF00) >> 8;
            std::uint32_t b = (_rgba & 0x00FF0000) >> 16;
            std::uint32_t a = (_rgba & 0xFF000000) >> a;

            return glm::vec4(r, g, b, a) / 255.0f;
        }

        void setColor(glm::vec4 color)
        {
            assert(color.r <= 1 && color.r >= 0);
            assert(color.g <= 1 && color.g >= 0);
            assert(color.b <= 1 && color.b >= 0);
            assert(color.a <= 1 && color.a >= 0);

            auto r = static_cast<std::uint32_t>(color.r * 255);
            auto g = static_cast<std::uint32_t>(color.g * 255);
            auto b = static_cast<std::uint32_t>(color.b * 255);
            auto a = static_cast<std::uint32_t>(color.a * 255);

            _rgba = a << 24 | b << 16 | g << 8 | r;
        }

        uint32_t getRgba() const
        {
            return _rgba;
        }

        void setRgba(uint32_t rgba)
        {
            _rgba = rgba;
        }

        std::uint32_t _rgba = 0;
    };

    virtual ~VertexBuffer() = default;
};

#endif VERTEXBUFFER_HPP