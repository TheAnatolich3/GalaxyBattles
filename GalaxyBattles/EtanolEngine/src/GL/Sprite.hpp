#include <vector>
#include <Engine.hpp>
class Sprite
{
public:
	void transform(float angle, float scale, float x_offset, float y_offset);
	void add_element(Engine::Triangle tr);

	const Engine::Triangle* get_data() const;
	size_t get_size() const;
private:
	std::vector<Engine::Triangle> _data;
};