#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>
#include <SDL.h>
#include <iostream>
#include <stbi/stb_image.h>
#include "Bitmap.hpp"

Bitmap::Bitmap(std::string_view filepath)
{
	SDL_RWops* file = SDL_RWFromFile(filepath.data(), "rb");

	if (file == nullptr)
	{
		throw std::runtime_error("Failed to open file: " + std::string{filepath});
	}

	Sint64 length = SDL_RWsize(file);
	std::vector<unsigned char> data(length);

	SDL_RWread(file, data.data(), length, 1);
	SDL_RWclose(file);

	int width, height;

	stbi_set_flip_vertically_on_load(false);

	auto img = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &width, &height, &_cnt_color_channels, 0);

	std::cout << width << " " << height << " " << _cnt_color_channels <<std::endl;
	if (img == nullptr)
	{
		throw std::runtime_error("unsupported file foramt: " + std::string{ filepath });
	}

	auto imgSize = width * height * _cnt_color_channels;

	_image = { img, img + imgSize };

	_size = glm::vec2(width, height);

	stbi_image_free(img);
}

Bitmap::Bitmap(int count_channels, std::vector<unsigned char> data, glm::vec2 size)
	: _cnt_color_channels(count_channels), _size(size)
{
	_image = std::move(data);
}

const std::vector<unsigned char>& Bitmap::getImage() const
{
	return _image;
}

int Bitmap::getColorCountChannels() const
{
	return _cnt_color_channels;
}

glm::vec2 Bitmap::getSize() const
{
	return _size;
}