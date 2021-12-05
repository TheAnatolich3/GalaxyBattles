#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string_view>
#include <vector>
#include <glm/glm.hpp>

class Bitmap
{
public:
	explicit Bitmap(std::string_view filepath);

	const std::vector<unsigned char>& getImage() const;

	int getColorCountChannels() const;
	glm::vec2 getSize() const;

private:
	int _cnt_color_channels;
	std::vector<unsigned char> _image;
	glm::vec2 _size;
};

#endif BITMAP_HPP