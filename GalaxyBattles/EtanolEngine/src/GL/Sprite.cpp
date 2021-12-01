#include "Sprite.hpp"


void Sprite::add_element(Engine::Triangle tr)
{
	_data.push_back(tr);
}

void Sprite::transform(float angle, float scale, float x_offset, float y_offset)
{
	for (Engine::Triangle& tr : _data)
	{
		tr.v1.x = scale * cos(angle) * tr.v1.x - sin(angle)*tr.v1.y + x_offset;
		tr.v1.y = sin(angle) * tr.v1.x + scale * tr.v1.y * cos(angle) + y_offset;

		tr.v2.x = scale * cos(angle) * tr.v2.x - sin(angle) * tr.v2.y + x_offset;
		tr.v2.y = sin(angle) * tr.v2.x + scale * tr.v2.y * cos(angle) + y_offset;

		tr.v3.x = scale * cos(angle) * tr.v3.x - sin(angle) * tr.v3.y + x_offset;
		tr.v3.y = sin(angle) * tr.v3.x + scale * tr.v3.y * cos(angle) + y_offset;
	}
}


const Engine::Triangle* Sprite::get_data() const
{
	return _data.data();
}

size_t Sprite::get_size() const
{
	return _data.size();
}